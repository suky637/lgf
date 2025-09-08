#include "lgf/windows/window_win.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Bounds* LGF::LGFWindow::getBounds() {
	return &bounds;
}

LGF::LGFWindow::LGFWindow(size_t width, size_t height, const char* title) :
height{height},
width{width},
mouseX{0},
mouseY{0}
{
	hInstance = GetModuleHandle(NULL);
	
	std::string class_name = "LGF-" + (std::string)title;

	PIXELFORMATDESCRIPTOR pfd = { 0 };
	HGLRC render_ctx = { 0 };

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;

	wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = class_name.c_str();

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		0,
		class_name.c_str(),
		title,
		WS_OVERLAPPEDWINDOW,

		0, 0, width, height,

		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (hwnd == NULL) {
		std::cerr << "Failed to create window, leaving.\n";
		exit(1);
	}

	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	device_ctx = GetDC(hwnd);

	int pixel_format = ChoosePixelFormat(device_ctx, &pfd);

	if (!SetPixelFormat(device_ctx, pixel_format, &pfd)) {
		std::cerr << "Failed to set pixel format.\n";
		exit(1);
	}

	render_ctx = wglCreateContext(device_ctx);
	wglMakeCurrent(device_ctx, render_ctx);

	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize GLAD\n";
	}

	ShowWindow(hwnd, SW_SHOWNORMAL);
	running = true;

	glViewport(0, 0, width, height);

	glDepthFunc(GL_NEVER);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.f);
	view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.f, 0.f, -3.f));

	bounds = { glm::vec2(width / 2.f, height / 2.f), glm::vec2(width, height) };
}

void LGF::LGFWindow::setFillColour(const int& r, const int& g, const int& b) {
	fillColour = glm::vec3(r / 255.f, g / 255.f, b / 255.f);
}

bool LGF::LGFWindow::windowShouldClose() {
	return !running;
}

void LGF::LGFWindow::pollEvents() {
	onEventHandlerBefore.trigger();
	running = GetMessage(&msg, NULL, 0, 0);
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	
	if (first_launch) {
		onResize.trigger();
		first_launch = false;
	}
}

void LGF::LGFWindow::render() {
	glClearColor(fillColour.r, fillColour.g, fillColour.b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	// calling render shit
	this->onRenderBefore.trigger();
	this->onRender.trigger();
	this->onRenderAfter.trigger();

	SwapBuffers(device_ctx);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LGF::LGFWindow* window = reinterpret_cast<LGF::LGFWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (window == nullptr) {
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case WM_SIZE: {
		int newWidth = LOWORD(lParam);
		int newHeight = HIWORD(lParam);
		window->width = newWidth;
		window->height = newHeight;
		glViewport(0, 0, newWidth, newHeight);
		window->proj = glm::ortho(0.0f, (float)newWidth, 0.0f, (float)newHeight, 0.1f, 100.f);
		window->bounds = { glm::vec2(newWidth / 2.f, newHeight / 2.f), glm::vec2(newWidth, newHeight) };
		window->onResize.trigger();
		break;
	}
	case WM_MOUSEMOVE: {
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		window->mouseX = mouseX;
		window->mouseY = mouseY;
		break;
	}
	case WM_LBUTTONUP: {
		window->onLeftMouseButtonReleased.trigger();
		break;
	}
	case WM_CHAR: {
		// This is where we get the character corresponding to the key press
		char character = static_cast<char>(wParam);  // wParam contains the character
		std::cout << "Character pressed: " << character << std::endl;
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}