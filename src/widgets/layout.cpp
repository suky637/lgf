#include "lgf/widgets/layout.h"

LGF::Widgets::Layout::Layout(LGF::LGFWindow* window) {
    this->window = window;
    this->bounds = {glm::vec2(0.f), glm::vec2(window->width, window->height), glm::vec2(window->width, window->height)};
    this->window->onResize += [&]() {
        this->updateBounds(this->window->getBounds()->position, this->window->getBounds()->size);
        this->onBoundsResized.trigger();
    };
    this->window->onRender += [&]{
        onRender.trigger();
    };

}