#include "lgf/widgets/label.h"

LGF::Widgets::Label::Label(LGF::LGFWindow* window, LGF::Draw::Font* font) {
    this->window = window;
    this->onAddChild += [&]() {
        this->parent->onBoundsResized += [&]() {
            this->updateLabel();
        };
        this->parent->onRender += [&]() {
            if (!enabled) return;
            this->font->renderText(this->text, this->bounds.position.x, this->bounds.position.y, 0.5f, this->colour);
            this->onRender.trigger();
        };
    };
    this->anchor = LGF::Widgets::Anchors::CENTRE;
    this->font = font;
}

void LGF::Widgets::Label::updateLabel() {
    glm::vec2 size = this->font->getTextSize(this->text, 0.5f);

    float centre_x = this->parent->bounds.position.x + position.x - size.x / 2.f;
    float centre_y = this->parent->bounds.position.y + position.y - size.y / 2.f;

    switch(this->anchor) {
        case LGF::Widgets::Anchors::CENTRE: {
            modified_pos = glm::vec2(centre_x, centre_y);
            this->updateBounds(modified_pos, size);
            break;
        }
    }
}

LGF::Widgets::Label& LGF::Widgets::Label::setPosition(const glm::vec2& position) {
    this->position = position;
    return *this;
}

LGF::Widgets::Label& LGF::Widgets::Label::setColour(const int& r, const int& g, const int& b, const int& a) {
    this->colour = glm::vec4(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
    return *this;
}

LGF::Widgets::Label& LGF::Widgets::Label::setText(const std::string& text) {
    this->text = text;
    return *this;
}