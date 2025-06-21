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

namespace LGF {
    namespace Widgets {
        class Label: public LGF::Widgets::Widget {
            public:
            Label(LGF::LGFWindow* window, LGF::Draw::Font* font);
            void setPosition(const glm::vec2& position);
            void setColour(const int& r, const int& g, const int& b, const int& a);
            std::string text;
            private:
            glm::vec2 modified_pos;
            glm::vec4 colour;
            LGF::Draw::Font* font;
            void updateLabel();
        };
    };
};