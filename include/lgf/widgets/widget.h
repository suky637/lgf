#pragma once

#include "lgf/widgets/bounds.h"
#include "lgf/window.h"
#include "lgf/widgets/anchors.h"
#include "lgf/widgets/resizeDirection.h"
#include "lgf/events.h"
#include <vector>

namespace LGF::Widgets {
        class Widget {
            public:
            void setAnchor(LGF::Widgets::Anchors anchor);
            void setResizeDirection(int direction);
            void updateBounds(const glm::vec2& pos, const glm::vec2& size);
            void addChild(Widget* child);
            void setActive(bool isActive);
            LGF::Events::Events onAddChild;
            LGF::Events::Events onBoundsResized;
            Widget* parent;
            Bounds bounds;
            std::vector<Widget*> children{};
            glm::vec2 position;
            LGF::LGFWindow* window;
            LGF::Widgets::Anchors anchor;
            int direction;
            bool enabled = false;
            LGF::Events::Events onRender;
        };
};