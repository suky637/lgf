#pragma once
#include <vector>
#include "lgf/events.h"
#include "lgf/widgets/bounds.h"
#include "lgf/window.h"
#include "lgf/widgets/anchors.h"

namespace LGF::Widgets {
    class WidgetBase {
        public:
        LGF::Events::Events onAddChild;
        WidgetBase* parent;
        std::vector<WidgetBase*> children{};
        ~WidgetBase() = default;
        bool enabled = false;
        LGF::Events::Events onRender;
        LGF::Events::Events onBoundsResized;
        void setActive(bool isActive);
        Bounds bounds;
        glm::vec2 position;
        LGF::LGFWindow* window;
        LGF::Widgets::Anchors anchor;
        int direction;
    };
}