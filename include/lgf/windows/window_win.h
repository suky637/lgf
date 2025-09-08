#pragma once
#include <Windows.h>
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "lgf/events.h"
#include "lgf/widgets/bounds.h"

namespace LGF {
	class LGFWindow {
	public:
		LGFWindow(size_t width, size_t height, const char* title);
		bool windowShouldClose();
		void pollEvents();
		void render();
		void setFillColour(const int& r, const int& g, const int& b);
		size_t width;
		size_t height;
		size_t original_width;
		size_t original_height;
		glm::mat4 proj;
		glm::mat4 view;

		LGF::Events::Events onRender;
		LGF::Events::Events onRenderBefore;
		LGF::Events::Events onRenderAfter;
		LGF::Events::Events onResize;
		LGF::Events::Events onChar;
		LGF::Events::Events onLeftMouseButtonReleased;
		LGF::Events::Events onMouseMove;
		LGF::Events::Events onEventHandlerBefore;

		Bounds* getBounds();

		int mouseX;
		int mouseY;

		bool isLeftClicked = false;

		void clearTextString();
		std::string getTextString();
		Bounds bounds;
	private:
		glm::vec3 fillColour;

		HINSTANCE hInstance;
		WNDCLASS wc;
		HWND hwnd;

		bool running;
		MSG msg;
		HDC device_ctx = { 0 };

		bool first_launch = true;
	};
};