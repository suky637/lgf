#pragma once

#include "lgf/window.h"
#include "lgf/widgets/anchors.h"
#include "lgf/widgets/resizeDirection.h"
#include "lgf/widgets/bounds.h"
#include "lgf/widgets/widget.h"
#include "lgf/events.h"
#include "lgf/text.h"

#include <glm/glm.hpp>
#include <string>

namespace LGF::Widgets {
        class Label: public LGF::Widgets::Widget<Label> {
            public:
            Label(LGF::LGFWindow* window, LGF::Draw::Font* font);
            Label& setPosition(const glm::vec2& position);
            Label& setColour(const int& r, const int& g, const int& b, const int& a);
            Label& setText(const std::string& text);
            std::string text;
            private:
            bool hasChild = false;
            glm::vec2 modified_pos;
            glm::vec4 colour;
            LGF::Draw::Font* font;
            void updateLabel();
        };
};