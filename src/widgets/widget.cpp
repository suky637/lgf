#include "lgf/widgets/widget.h"

void LGF::Widgets::Widget::setAnchor(LGF::Widgets::Anchors anchor) {
    this->anchor = anchor;
}

void LGF::Widgets::Widget::updateBounds(const glm::vec2& pos, const glm::vec2& size) {
    this->bounds.position = pos;
    this->bounds.size = size;
}

void LGF::Widgets::Widget::setResizeDirection(int direction) {
    this->direction = direction;
}

void LGF::Widgets::Widget::addChild(LGF::Widgets::Widget* child) {
    child->parent = this;
    child->onAddChild.trigger();
    this->children.push_back(child);

}

void LGF::Widgets::Widget::setZOrder(int zOrder) {
    this->z_order = zOrder;
}

void LGF::Widgets::Widget::setActive(bool isActive) {
    for (auto child : children) {
        enabled = isActive;
        child->enabled = isActive;
        child->setActive(isActive);
        if (isActive) {
            child->onBoundsResized.trigger();
        }
    }
}