#include "lgf/linux/window_linux.h"

Bounds* LGF::LGFWindow::getBounds() {
    return &bounds;
}

void LGF::LGFWindow::handleKeypresses(XKeyEvent* event, bool isReleased) {
    auto key_direction = isReleased ? XKB_KEY_UP : XKB_KEY_DOWN;
    xkb_state_update_key(_xbk_state, event->keycode, key_direction);
    if (!isReleased) {
        char buf[64];
        int bytes = xkb_state_key_get_utf8(_xbk_state, event->keycode, buf, sizeof(buf));
        if (bytes > 0) {
            buf[bytes] = '\0';
            this->currentTextString += std::string(buf);
            std::cout << this->currentTextString << "\n";
            this->onChar.trigger();
        }
    }
}
void LGF::LGFWindow::clearTextString() {
    this->currentTextString = "";
}

std::string LGF::LGFWindow::getTextString() {
    return this->currentTextString;
}

LGF::LGFWindow::LGFWindow(size_t width, size_t height, const char* title) {
    #include <cstdlib>

    setenv("XMODIFIERS", "@im=xim", 1);

    fillColour = glm::vec3(1.0f);
    display = XOpenDisplay(NULL);
    if (!display) {
        std::cerr << "Cannot open X display\n";
    }

    int screen = DefaultScreen(display);
    root = RootWindow(display, screen);

    static int visual_attribs[] = {
        GLX_RGBA,
        GLX_DEPTH_SIZE, 24,
        GLX_DOUBLEBUFFER,
        None
    };

    XVisualInfo* vi = glXChooseVisual(display, screen, visual_attribs);
    if (!vi) {
        std::cerr << "No appropriate visual found.\n";
    }

    Colormap cmap = XCreateColormap(display, root, vi->visual, AllocNone);

    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;

    win = XCreateWindow(display, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    wmDeleteMessage = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, win, &wmDeleteMessage, 1);

    
    this->width = width;
    this->height = height;

    this->original_width = width;
    this->original_height = height;
    
    bounds = {glm::vec2(width / 2.f, height / 2.f), glm::vec2(width, height)};

    XStoreName(display, win, title);
    XMapWindow(display, win);
    
    ctx = glXCreateContext(display, vi, NULL, GL_TRUE);
    if (!ctx) {
        std::cerr << "Failed to create GLX context.\n";
    }
    glXMakeCurrent(display, win, ctx);
    
    auto xcb_conn = XGetXCBConnection(display);

    _xbk_ctx = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    int device_id = xkb_x11_get_core_keyboard_device_id(xcb_conn);

    _xbk_keymap = xkb_x11_keymap_new_from_device(_xbk_ctx, xcb_conn, device_id, XKB_KEYMAP_COMPILE_NO_FLAGS);
    _xbk_state = xkb_x11_state_new_from_device(_xbk_keymap, xcb_conn, device_id);
    
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD\n";
    }
    running = true;

    glDepthFunc(GL_NEVER);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Creating matrices
    proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.f);
    view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));
}

bool LGF::LGFWindow::windowShouldClose() {
    return !running;
}

void LGF::LGFWindow::setFillColour(const int& r, const int& g, const int& b) {
    fillColour = glm::vec3(r / 255.f, g / 255.f, b / 255.f);
}

void LGF::LGFWindow::pollEvents() {
    
    onEventHandlerBefore.trigger();

    isLeftClicked = false;

    while (XPending(display)) {
        XEvent xev;
        XNextEvent(display, &xev);

        switch (xev.type)
        {
        case Expose:
            break;
        case ConfigureNotify:
            width = xev.xconfigure.width;
            height = xev.xconfigure.height;
            glViewport(0, 0, width, height);
            proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.f);
            bounds = {glm::vec2(width / 2.f, height / 2.f), glm::vec2(width, height)};
            this->onResize.trigger();
            break;
        case MotionNotify: {
            mouseX = xev.xmotion.x;
            mouseY = xev.xmotion.y;
            onMouseMove.trigger();
            break;
        }
        case ButtonRelease: {
            if (xev.xbutton.button == 1) {
                isLeftClicked = true;
                onLeftMouseButtonReleased.trigger();
            }
            break;
        }
        case ClientMessage:
            if ((Atom)xev.xclient.data.l[0] == wmDeleteMessage) {
                running = false;  // User clicked close button
            }
            break;
        case KeyPress: {
            handleKeypresses(&xev.xkey, false);
            break;
        }
        case KeyRelease: {
            handleKeypresses(&xev.xkey, true);
        }
        default:
            break;
        }
    }
}

void LGF::LGFWindow::render() {
    glClearColor(fillColour.r, fillColour.g, fillColour.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // calling render shit
    this->onRenderBefore.trigger();
    this->onRender.trigger();
    this->onRenderAfter.trigger();

    glXSwapBuffers(display, win);
}

LGF::LGFWindow::~LGFWindow() {
    xkb_state_unref(this->_xbk_state);
    xkb_keymap_unref(this->_xbk_keymap);
    xkb_context_unref(this->_xbk_ctx);
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, ctx);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
}