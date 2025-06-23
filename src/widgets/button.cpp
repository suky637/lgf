#include "lgf/widgets/button.h"

float LGF::Widgets::Button::getCornerRadius() {
    return this->radius;
}

LGF::Widgets::Button& LGF::Widgets::Button::setCornerRadius(float radius) {
    this->radius = radius;
    return *this;
}

LGF::Widgets::Button::Button(LGF::LGFWindow* window) :
    quad{LGF::Draw::QuadType::BUTTON},
    radius(0.f),
    minSize(glm::vec2(0.f)),
    maxSize(window->getBounds()->size)
{
    this->window = window;
    quad.linkWindow(window);
    this->bounds = {glm::vec2(0.f), glm::vec2(0.f), glm::vec2(0.f)};
    this->onAddChild += [&]() {
        this->parent->onBoundsResized += [&]() {
            this->updatePanel();
        };
        this->parent->onRender += [&]() {
            if (!enabled) return;
            glUseProgram(quad.getShaderID());
            glm::vec2 rectSize = {
                glm::length(glm::vec3(this->quad.model[0])),
                glm::length(glm::vec3(this->quad.model[1]))
            };
            glUniform2f(glGetUniformLocation(quad.getShaderID(), "u_dimensions"), rectSize.x, rectSize.y);
            glUniform2f(glGetUniformLocation(quad.getShaderID(), "u_boundsPos"), this->parent->bounds.position.x, this->parent->bounds.position.y);
            glUniform2f(glGetUniformLocation(quad.getShaderID(), "u_boundsSize"), this->parent->bounds.size.x, this->parent->bounds.size.y);

            glUniform2f(glGetUniformLocation(quad.getShaderID(), "u_mousePosition"), this->window->mouseX, this->window->height - this->window->mouseY);
            glUniform2f(glGetUniformLocation(quad.getShaderID(), "u_bounds_size"), this->bounds.size.x, this->bounds.size.y);
            glUniform2f(glGetUniformLocation(quad.getShaderID(), "u_bounds_pos"), this->bounds.position.x, this->bounds.position.y);

            glUniform1f(glGetUniformLocation(quad.getShaderID(), "u_radius"), radius);
            quad.render();
            this->onRender.trigger();
        };
    };
    this->window->onEventHandlerBefore += [&] {
        if (!this->enabled) return;
        clicked = false;
    };
    this->window->onLeftMouseButtonReleased += [&] {
        if (!this->enabled) return;
        glm::vec2 world_pos = glm::vec2(this->window->mouseX, this->window->getBounds()->size.y - this->window->mouseY);

        if (world_pos.x > this->bounds.position.x - this->bounds.size.x / 2.f && world_pos.x < this->bounds.position.x + this->bounds.size.x / 2.f && world_pos.y > this->bounds.position.y - this->bounds.size.y / 2.f && world_pos.y < this->bounds.position.y + this->bounds.size.y / 2.f) {
            this->onButtonClicked.trigger();
            clicked = true;
        }
    };
    this->anchor = LGF::Widgets::Anchors::CENTRE;
}

LGF::Widgets::Button& LGF::Widgets::Button::setMinimumSize(const glm::vec2& size) {
    this->minSize = size;
    return *this;
}
LGF::Widgets::Button& LGF::Widgets::Button::setMaximumSize(const glm::vec2& size) {
    this->maxSize = size;
    return *this;
}

void LGF::Widgets::Button::updatePanel() {
    if (!enabled) return;
    if (this->parent == nullptr) {
        std::cout << "Error: Panel doesn't have a parent; please assign a Layout to the panel; skipping the function updatePanel()\n**WARNING** This can lead the widgets not resizing which is not what you want. Like I said, the panel must be a child of a widget.\n";
        return;
    }
    glm::vec2 newPos;
    glm::vec2 newSize = glm::vec2(0.f);
    float widthProportion = this->parent->bounds.size.x / this->parent->bounds.originalSize.x;
    float heightProportion = this->parent->bounds.size.y / this->parent->bounds.originalSize.y;
    if (direction & LGF::Widgets::ResizeDirection::VERTICAL) {
        newSize += glm::vec2(0, (size.y * heightProportion - size.y));
    }
    if (direction & LGF::Widgets::ResizeDirection::HORIZONTAL) {
        newSize += glm::vec2(size.x * widthProportion - size.x, 0.f);
    }
    glm::vec2 _size = size + newSize;

    float centre_x = this->parent->bounds.position.x + position.x;
    float centre_y = this->parent->bounds.position.y + position.y;
    glm::vec2 size_ = glm::vec2(
        std::max(minSize.x, std::min(_size.x, maxSize.x)), 
        std::max(minSize.y, std::min(_size.y, maxSize.y))
    );

    switch (this->anchor)
    {
        case LGF::Widgets::Anchors::CENTRE: {
            newPos = glm::vec2(centre_x, centre_y);
            this->quad.setRect(newPos, size_);
            this->updateBounds(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::LEFT: {
            newPos = glm::vec2(centre_x - parent->bounds.size.x / 2.f + size_.x / 2.f, centre_y);
            this->updateBounds(newPos, size_);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::RIGHT: {
            newPos = glm::vec2(centre_x + parent->bounds.size.x / 2.f - size_.x / 2.f, centre_y);
            this->updateBounds(newPos, size_);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::BOTTOM: {
            newPos = glm::vec2(centre_x, centre_y - parent->bounds.size.y / 2.f + size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::TOP: {
            newPos = glm::vec2(centre_x, centre_y + parent->bounds.size.y / 2.f - size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::TOP_LEFT: {
            newPos = glm::vec2(centre_x - parent->bounds.size.x / 2.f + size_.x / 2.f, centre_y + parent->bounds.size.y / 2.f - size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::TOP_RIGHT: {
            newPos = glm::vec2(centre_x + parent->bounds.size.x / 2.f - size_.x / 2.f, centre_y + parent->bounds.size.y / 2.f - size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::BOTTOM_LEFT: {
            newPos = glm::vec2(centre_x - parent->bounds.size.x / 2.f + size_.x / 2.f, centre_y - parent->bounds.size.y / 2.f + size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::BOTTOM_RIGHT: {
            newPos = glm::vec2(centre_x + parent->bounds.size.x / 2.f - size_.x / 2.f, centre_y - parent->bounds.size.y / 2.f + size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.setRect(newPos, size_);
            break;
        }
        default: {
            break;
        }
    }
    this->onBoundsResized.trigger();
}

LGF::Widgets::Button& LGF::Widgets::Button::setRect(const glm::vec2& pos, const glm::vec2& size) {
    this->position = pos;
    this->size = size;
    this->updateBounds(pos, size);
    this->bounds.originalSize = size;
    updatePanel();
    return *this;
}

LGF::Widgets::Button& LGF::Widgets::Button::setColour(const int& r, const int& g, const int& b, const int& a) {
    glUniform4f(glGetUniformLocation(quad.getShaderID(), "colour"), r / 255.f, g / 255.f, b / 255.f, a / 255.f);
    return *this;
}

LGF::Widgets::Button::~Button() {
    this->children.clear();
    this->quad.~Quad();
}