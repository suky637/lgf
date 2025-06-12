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
    window->addOnRenderEvent([&]() {
        glUseProgram(quad.getShaderID());
        glm::vec2 rectSize = {
            glm::length(glm::vec3(this->quad.model[0])),
            glm::length(glm::vec3(this->quad.model[1]))
        };
        glUniform2f(glGetUniformLocation(quad.getShaderID(), "u_dimensions"), rectSize.x, rectSize.y);
        glUniform1f(glGetUniformLocation(quad.getShaderID(), "u_radius"), radius);
        quad.render();
    });
    window->addOnResizeEvent([&]() {
        this->updatePanel();
    });
    this->anchor = LGF::Widgets::Anchors::CENTRE;
}

void LGF::Widgets::Panel::setAnchor(LGF::Widgets::Anchors anchor) {
    this->anchor = anchor;
}

void LGF::Widgets::Panel::setMinimumSize(const glm::vec2& size) {
    this->minSize = size;
}
void LGF::Widgets::Panel::setMaximumSize(const glm::vec2& size) {
    this->maxSize = size;
}

void LGF::Widgets::Panel::updatePanel() {
    glm::vec2 newPos;
    glm::vec2 offset = glm::vec2(0.f);
    glm::vec2 newSize = glm::vec2(0.f);
    float widthProportion = (float)window->width / (float)window->original_width;
    float heightProportion = (float)window->height / (float)window->original_height;
    if (direction & LGF::Widgets::ResizeDirection::VERTICAL) {
        newSize += glm::vec2(0, (size.y * heightProportion - size.y));
        offset += glm::vec2(0.f, (size.y * heightProportion - size.y) / 2.f);
    }
    if (direction & LGF::Widgets::ResizeDirection::HORIZONTAL) {
        newSize += glm::vec2(size.x * widthProportion - size.x, 0.f);
        offset += glm::vec2((size.x * widthProportion - size.x) / 2.f, 0.f);
    }
    glm::vec2 _size = size + newSize;

    float centre_x = (float)(window->width) / 2.f + position.x;
    float centre_y = (float)(window->height) / 2.f + position.y;
    glm::vec2 size_ = glm::vec2(
        std::max(minSize.x, std::min(_size.x, maxSize.x)), 
        std::max(minSize.y, std::min(_size.y, maxSize.y))
    );

    switch (this->anchor)
    {
        case LGF::Widgets::Anchors::CENTRE: {
            newPos = glm::vec2(centre_x, centre_y);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::LEFT: {
            newPos = glm::vec2(size_.x / 2.f, centre_y);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::RIGHT: {
            newPos = glm::vec2(window->width - size_.x / 2.f, centre_y);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::BOTTOM: {
            newPos = glm::vec2(centre_x, size_.y / 2.f);
            this->quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::TOP: {
            newPos = glm::vec2(centre_x, window->height - size_.y / 2.f);
            this->quad.setRect(newPos, size_);
        }
        default: {
            break;
        }
    }
}

void LGF::Widgets::Panel::setRect(const glm::vec2& pos, const glm::vec2& size) {
    this->position = pos;
    this->size = size;
    updatePanel();
}

void LGF::Widgets::Panel::setColour(const int& r, const int& g, const int& b, const int& a) {
    glUniform4f(glGetUniformLocation(quad.getShaderID(), "colour"), r / 255.f, g / 255.f, b / 255.f, a / 255.f);
}

void LGF::Widgets::Panel::setResizeDirection(int direction) {
    this->direction = direction;
}