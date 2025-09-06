#pragma once
#include "lgf/quad.h"
#include "lgf/window.h"
#include "lgf/widgets/anchors.h"
#include "lgf/widgets/resizeDirection.h"
#include "lgf/widgets/bounds.h"
#include "lgf/widgets/widget.h"
#include "lgf/widgets/label.h"
#include "lgf/events.h"
#include <any>

namespace LGF::Widgets {

        class TextBox : public LGF::Widgets::Widget<TextBox> {
            public:
            TextBox(LGF::LGFWindow* window);
            TextBox& setRect(const glm::vec2& pos, const glm::vec2& size);
            TextBox& setColour(const int& r, const int& g, const int& b, const int& a);
            TextBox& setMinimumSize(const glm::vec2& size);
            TextBox& setMaximumSize(const glm::vec2& size);
            float getCornerRadius();
            TextBox& linkContentLabel(LGF::Widgets::Label* label);
            
            TextBox& setCornerRadius(float radius);
            ~TextBox();
            bool focused = false;
            private:
            LGF::Widgets::Label* text_content;
            float radius;
            void updatePanel();
            glm::vec2 size;
            glm::vec2 maxSize;
            glm::vec2 minSize;
            LGF::Draw::Quad quad;
        };
};
