#include "lgf/linux/window_linux.h"

Bounds* LGF::LGFWindow::getBounds() {
    return &bounds;
}

LGF::LGFWindow::LGFWindow(size_t width, size_t height, const char* title) {
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

    XStoreName(display, win, "LGF Test window");
    XMapWindow(display, win);

    ctx = glXCreateContext(display, vi, NULL, GL_TRUE);
    if (!ctx) {
        std::cerr << "Failed to create GLX context.\n";
    }
    glXMakeCurrent(display, win, ctx);

    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD\n";
    }
    running = true;

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
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
        case ClientMessage:
            if ((Atom)xev.xclient.data.l[0] == wmDeleteMessage) {
                running = false;  // User clicked close button
            }
            break;
        default:
            break;
        }
    }
}

void LGF::LGFWindow::render() {
    glClearColor(fillColour.r, fillColour.g, fillColour.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // calling render shit
    this->onRenderBefore.trigger();
    this->onRender.trigger();
    this->onRenderAfter.trigger();

    glXSwapBuffers(display, win);
}

LGF::LGFWindow::~LGFWindow() {
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, ctx);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
}