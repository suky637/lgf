#pragma once

#include "lgf/widgets/bounds.h"
#include "lgf/window.h"
#include "lgf/widgets/anchors.h"
#include "lgf/widgets/resizeDirection.h"
#include "lgf/events.h"
#include "lgf/widgets/widget_base.h"
#include <vector>

namespace LGF::Widgets {
        template<typename T>
        class Widget : public WidgetBase {
            public:
            T& setAnchor(LGF::Widgets::Anchors anchor) {
                this->anchor = anchor;
                return static_cast<T&>(*this);
            }
            T& setResizeDirection(int direction) {
                this->direction = direction;
                return static_cast<T&>(*this);
            }
            T& updateBounds(const glm::vec2& pos, const glm::vec2& size) {
                this->bounds.position = pos;
                this->bounds.size = size;
                return static_cast<T&>(*this);
            }
            T& addChild(WidgetBase* child) {
                child->parent = this;
                child->onAddChild.trigger();
                this->children.push_back(child);
                return static_cast<T&>(*this);
            }
        };
};