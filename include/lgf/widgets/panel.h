#pragma once
#include "lgf/quad.h"
#include "lgf/window.h"
#include "lgf/widgets/anchors.h"
#include "lgf/widgets/resizeDirection.h"

#include <any>

namespace LGF {
    namespace Widgets {
        class Panel {
            public:
            Panel(LGF::LGFWindow* window);
            void setAnchor(LGF::Widgets::Anchors anchor);
            void setRect(const glm::vec2& pos, const glm::vec2& size);
            void setColour(const int& r, const int& g, const int& b, const int& a);
            void setResizeDirection(int direction);
            void setMinimumSize(const glm::vec2& size);
            void setMaximumSize(const glm::vec2& size);
            float getCornerRadius();
            void setCornerRadius(float radius);
            private:
            float radius = 0.f;
            int direction;
            void updatePanel();
            glm::vec2 position;
            glm::vec2 size;
            glm::vec2 maxSize;
            glm::vec2 minSize;
            LGF::LGFWindow* window;
            LGF::Draw::Quad quad;
            LGF::Widgets::Anchors anchor;
        };
    };
};
