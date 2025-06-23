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

        private:
        Bounds bounds;
        glm::vec3 fillColour;
        Display* display;
        Window root;
        Window win;
        GLXContext ctx;
        Atom wmDeleteMessage;
        bool running;
        XIM xim;
        XIC xic;
        std::string lastChar;
    };
};