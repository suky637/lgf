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
        class Panel : public LGF::Widgets::Widget<Panel> {
            public:
            explicit Panel(LGF::LGFWindow* window);
            Panel& setRect(const glm::vec2& pos, const glm::vec2& size);
            Panel& setColour(const int& r, const int& g, const int& b, const int& a);
            Panel& setMinimumSize(const glm::vec2& size);
            Panel& setMaximumSize(const glm::vec2& size);
            float getCornerRadius();
            Panel& setCornerRadius(float radius);
            ~Panel();
            private:
            float radius;
            void updatePanel();
            glm::vec2 size;
            glm::vec2 maxSize;
            glm::vec2 minSize;
            LGF::Draw::Quad quad;
        };
};
