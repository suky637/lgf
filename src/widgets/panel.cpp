#include "lgf/widgets/panel.h"

float LGF::Widgets::Panel::getCornerRadius() {
    return this->radius;
}

void LGF::Widgets::Panel::setCornerRadius(float radius) {
    this->radius = radius;
}

LGF::Widgets::Panel::Panel(LGF::LGFWindow* window) :
    quad{LGF::Draw::QuadType::ROUNDED}
{
    this->window = window;
    quad.linkWindow(window);
    this->bounds = {glm::vec2(0.f), glm::vec2(0.f), glm::vec2(0.f)};
    window->onRender += [&]() {
        if (!enabled) return;
        glUseProgram(quad.getShaderID());
        glm::vec2 rectSize = {
            glm::length(glm::vec3(this->quad.model[0])),
            glm::length(glm::vec3(this->quad.model[1]))
        };
        glUniform2f(glGetUniformLocation(quad.getShaderID(), "u_dimensions"), rectSize.x, rectSize.y);
        glUniform2f(glGetUniformLocation(quad.getShaderID(), "u_boundsPos"), this->parent->bounds.position.x, this->parent->bounds.position.y);
        glUniform2f(glGetUniformLocation(quad.getShaderID(), "u_boundsSize"), this->parent->bounds.size.x, this->parent->bounds.size.y);
        glUniform1f(glGetUniformLocation(quad.getShaderID(), "u_radius"), radius);
        quad.render();
    };
    this->onAddChild += [&]() {
        this->parent->onBoundsResized += [&]() {
            this->updatePanel();
        };
    };
    this->anchor = LGF::Widgets::Anchors::CENTRE;
}

void LGF::Widgets::Panel::setMinimumSize(const glm::vec2& size) {
    this->minSize = size;
}
void LGF::Widgets::Panel::setMaximumSize(const glm::vec2& size) {
    this->maxSize = size;
}

void LGF::Widgets::Panel::updatePanel() {
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
        }
        case LGF::Widgets::Anchors::TOP_LEFT: {
            newPos = glm::vec2(centre_x - parent->bounds.size.x / 2.f + size_.x / 2.f, centre_y + parent->bounds.size.y / 2.f - size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.setRect(newPos, size_);
        }
        case LGF::Widgets::Anchors::TOP_RIGHT: {
            newPos = glm::vec2(centre_x + parent->bounds.size.x / 2.f - size_.x / 2.f, centre_y + parent->bounds.size.y / 2.f - size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.setRect(newPos, size_);
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

    this->quad.model = glm::translate(this->quad.model, glm::vec3(0.f, 0.f, z_order / 100.f));
    this->onBoundsResized.trigger();
}

void LGF::Widgets::Panel::setRect(const glm::vec2& pos, const glm::vec2& size) {
    this->position = pos;
    this->size = size;
    this->updateBounds(pos, size);
    this->bounds.originalSize = size;
    updatePanel();
}

void LGF::Widgets::Panel::setColour(const int& r, const int& g, const int& b, const int& a) {
    glUniform4f(glGetUniformLocation(quad.getShaderID(), "colour"), r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}