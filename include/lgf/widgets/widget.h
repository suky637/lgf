#pragma once

#include "lgf/widgets/bounds.h"
#include "lgf/window.h"
#include "lgf/widgets/anchors.h"
#include "lgf/widgets/resizeDirection.h"
#include "lgf/events.h"
#include <vector>

namespace LGF {
    namespace Widgets {
        class Widget {
            public:
            void setAnchor(LGF::Widgets::Anchors anchor);
            void setResizeDirection(int direction);
            void updateBounds(const glm::vec2& pos, const glm::vec2& size);
            void addChild(Widget* child);

            LGF::Events::OnAddChildEvents onAddChild;
            LGF::Events::OnBoundsResizedEvents onBoundsResized;
            Widget* parent;
            Bounds bounds;
            std::vector<Widget*> children{};
            glm::vec2 position;
            LGF::LGFWindow* window;
            LGF::Widgets::Anchors anchor;
            int direction;
        };
    };
};