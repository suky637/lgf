#include "lgf/widgets/widget_base.h"

void LGF::Widgets::WidgetBase::setActive(bool isActive) {
    for (auto child : children) {
        enabled = isActive;
        child->enabled = isActive;
        child->setActive(isActive);
        if (isActive) {
            child->onBoundsResized.trigger();
        }
    }
}