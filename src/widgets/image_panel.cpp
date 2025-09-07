#include "lgf/widgets/image_panel.h"

float LGF::Widgets::ImagePanel::getCornerRadius() {
    return this->radius;
}

LGF::Widgets::ImagePanel& LGF::Widgets::ImagePanel::setCornerRadius(float radius) {
    this->radius = radius;
    return *this;
}

LGF::Widgets::ImagePanel::ImagePanel(LGF::LGFWindow* window, LGF::Draw::Image* image) :
    quad{image},
    radius(0.f),
    minSize(glm::vec2(0.f)),
    maxSize(window->getBounds()->size)
{
    UV_modifier = glm::vec2(1.f);
    original_size = glm::vec2(0.f);
    this->window = window;
    quad.quad.linkWindow(window);
    this->bounds = {glm::vec2(0.f), glm::vec2(0.f), glm::vec2(0.f)};
    this->onAddChild += [&]() {
        this->parent->onBoundsResized += [&]() {
            this->updatePanel();
        };
        this->parent->onRender += [&]() {
            if (!enabled) return;
            glUseProgram(quad.quad.getShaderID());
            glm::vec2 rectSize = {
                glm::length(glm::vec3(this->quad.quad.model[0])),
                glm::length(glm::vec3(this->quad.quad.model[1]))
            };
            glUniform2f(glGetUniformLocation(quad.quad.getShaderID(), "u_dimensions"), rectSize.x, rectSize.y);
            glUniform2f(glGetUniformLocation(quad.quad.getShaderID(), "u_boundsPos"), this->parent->bounds.position.x, this->parent->bounds.position.y);
            glUniform2f(glGetUniformLocation(quad.quad.getShaderID(), "u_boundsSize"), this->parent->bounds.size.x, this->parent->bounds.size.y);
            glUniform2f(glGetUniformLocation(quad.quad.getShaderID(), "u_uvmod"), this->UV_modifier.x, this->UV_modifier.y);
            glUniform2f(glGetUniformLocation(quad.quad.getShaderID(), "u_uvoffset"), this->UV_offset.x, this->UV_offset.y);
            glUniform1f(glGetUniformLocation(quad.quad.getShaderID(), "u_radius"), radius);

            quad.render();
            this->onRender.trigger();
        };
    };
    this->anchor = LGF::Widgets::Anchors::CENTRE;
}

LGF::Widgets::ImagePanel& LGF::Widgets::ImagePanel::setMinimumSize(const glm::vec2& size) {
    this->minSize = size;
    return *this;
}
LGF::Widgets::ImagePanel& LGF::Widgets::ImagePanel::setMaximumSize(const glm::vec2& size) {
    this->maxSize = size;
    return *this;
}

void LGF::Widgets::ImagePanel::updatePanel() {
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
            this->quad.quad.setRect(newPos, size_);
            this->updateBounds(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::LEFT: {
            newPos = glm::vec2(centre_x - parent->bounds.size.x / 2.f + size_.x / 2.f, centre_y);
            this->updateBounds(newPos, size_);
            this->quad.quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::RIGHT: {
            newPos = glm::vec2(centre_x + parent->bounds.size.x / 2.f - size_.x / 2.f, centre_y);
            this->updateBounds(newPos, size_);
            this->quad.quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::BOTTOM: {
            newPos = glm::vec2(centre_x, centre_y - parent->bounds.size.y / 2.f + size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::TOP: {
            newPos = glm::vec2(centre_x, centre_y + parent->bounds.size.y / 2.f - size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::TOP_LEFT: {
            newPos = glm::vec2(centre_x - parent->bounds.size.x / 2.f + size_.x / 2.f, centre_y + parent->bounds.size.y / 2.f - size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::TOP_RIGHT: {
            newPos = glm::vec2(centre_x + parent->bounds.size.x / 2.f - size_.x / 2.f, centre_y + parent->bounds.size.y / 2.f - size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::BOTTOM_LEFT: {
            newPos = glm::vec2(centre_x - parent->bounds.size.x / 2.f + size_.x / 2.f, centre_y - parent->bounds.size.y / 2.f + size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.quad.setRect(newPos, size_);
            break;
        }
        case LGF::Widgets::Anchors::BOTTOM_RIGHT: {
            newPos = glm::vec2(centre_x + parent->bounds.size.x / 2.f - size_.x / 2.f, centre_y - parent->bounds.size.y / 2.f + size_.y / 2.f);
            this->updateBounds(newPos, size_);
            this->quad.quad.setRect(newPos, size_);
            break;
        }
        default: {
            break;
        }
    }

    if (force_ratio) {
        float original_ratio = this->original_size.x / this->original_size.y;
        float new_ratio = size_.x / size_.y;
        //std::cout << original_ratio << "\n" << new_ratio << "\n";
        if (new_ratio < original_ratio) {
            this->UV_modifier.x = 1.f / (original_ratio / new_ratio);
            UV_offset.x = 0.5f - UV_modifier.x / 2.f;
            this->UV_modifier.y = 1.f;
            UV_offset.y = 0.f;
        }
        else {
            this->UV_modifier.x = 1.f;
            UV_offset.y = 0.5f - UV_modifier.y / 2.f;
            this->UV_modifier.y = original_ratio / new_ratio;
            UV_offset.x = 0.f;
        }
    }

    this->onBoundsResized.trigger();
}

LGF::Widgets::ImagePanel& LGF::Widgets::ImagePanel::forceRatio(bool force) {
    this->force_ratio = force;
    return *this;
}

LGF::Widgets::ImagePanel& LGF::Widgets::ImagePanel::setRect(const glm::vec2& pos, const glm::vec2& size) {
    this->position = pos;
    if (this->original_size.x == 0 && this->original_size.y == 0) {
        this->original_size = size;
        std::cout << "Set Original Size to: (" << original_size.x << "; " << original_size.y << ")\n";
    }
    this->size = size;
    this->updateBounds(pos, size);
    this->bounds.originalSize = size;
    updatePanel();
    return *this;
}

LGF::Widgets::ImagePanel& LGF::Widgets::ImagePanel::setColour(const int& r, const int& g, const int& b, const int& a) {
    glUniform4f(glGetUniformLocation(quad.quad.getShaderID(), "colour"), r / 255.f, g / 255.f, b / 255.f, a / 255.f);
    return *this;
}

LGF::Widgets::ImagePanel::~ImagePanel() {
    this->children.clear();
    this->quad.quad.~Quad();
}