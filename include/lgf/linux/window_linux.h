#pragma once
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xatom.h>
#include <glad/glad.h>
#include <GL/glx.h>
#include <iostream>
#include <glm/glm.hpp>
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
        ~LGFWindow();
        size_t width;
        size_t height;
        size_t original_width;
        size_t original_height;
        glm::mat4 proj;
        glm::mat4 view;

        /* Events :) */
        LGF::Events::OnRenderEvents onRender;
        LGF::Events::OnRenderBeforeEvents onRenderBefore;
        LGF::Events::OnRenderAfterEvents onRenderAfter;
        LGF::Events::OnResizeEvents onResize;

        Bounds* getBounds();

        private:
        Bounds bounds;
        glm::vec3 fillColour;
        std::vector<std::function<void()>> beforeDrawCalls{};
        std::vector<std::function<void()>> drawCalls{};
        std::vector<std::function<void()>> afterDrawCalls{};
        std::vector<std::function<void()>> onResizeCalls{};
        Display* display;
        Window root;
        Window win;
        GLXContext ctx;
        Atom wmDeleteMessage;
        bool running;
    };
};