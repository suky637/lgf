#pragma once
#include "lgf/quad.h"
#include "lgf/window.h"
#include "lgf/widgets/anchors.h"
#include "lgf/widgets/resizeDirection.h"
#include "lgf/widgets/bounds.h"
#include "lgf/widgets/widget.h"
#include "lgf/events.h"
#include "lgf/image.h"
#include <any>

namespace LGF::Widgets {
        class ImagePanel : public LGF::Widgets::Widget<ImagePanel> {
            public:
            ImagePanel(LGF::LGFWindow* window, LGF::Draw::Image* image);
            ImagePanel& setRect(const glm::vec2& pos, const glm::vec2& size);
            ImagePanel& setColour(const int& r, const int& g, const int& b, const int& a);
            ImagePanel& setMinimumSize(const glm::vec2& size);
            ImagePanel& setMaximumSize(const glm::vec2& size);
            float getCornerRadius();
            ImagePanel& setCornerRadius(float radius);
            ~ImagePanel();
            private:
            float radius;
            void updatePanel();
            glm::vec2 size;
            glm::vec2 maxSize;
            glm::vec2 minSize;
            LGF::Draw::ImageQuad quad;
        };
};
