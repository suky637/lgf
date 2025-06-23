#pragma once
#include "lgf/quad.h"
#include "lgf/window.h"
#include "lgf/widgets/anchors.h"
#include "lgf/widgets/resizeDirection.h"
#include "lgf/widgets/bounds.h"
#include "lgf/widgets/widget.h"
#include "lgf/events.h"
#include <any>

namespace LGF::Widgets {
        enum class ButtonState {
            NONE,
            HOVERED
        };

        class Button : public LGF::Widgets::Widget<Button> {
            public:
            Button(LGF::LGFWindow* window);
            Button& setRect(const glm::vec2& pos, const glm::vec2& size);
            Button& setColour(const int& r, const int& g, const int& b, const int& a);
            Button& setMinimumSize(const glm::vec2& size);
            Button& setMaximumSize(const glm::vec2& size);
            float getCornerRadius();
            Button& setCornerRadius(float radius);
            ~Button();
            ButtonState buttonState;
            LGF::Events::Events onButtonClicked;
            bool clicked = false;
            private:
            float radius;
            void updatePanel();
            glm::vec2 size;
            glm::vec2 maxSize;
            glm::vec2 minSize;
            LGF::Draw::Quad quad;
        };
};
