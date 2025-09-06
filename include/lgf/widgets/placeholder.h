#pragma once
#include "lgf/quad.h"
#include "lgf/window.h"
#include "lgf/widgets/anchors.h"
#include "lgf/widgets/resizeDirection.h"
#include "lgf/widgets/bounds.h"
#include "lgf/widgets/widget.h"
#include "lgf/events.h"
#include <any>

namespace LGF::Widgets {
        class Placeholder : public LGF::Widgets::Widget<Placeholder> {
            public:
            explicit Placeholder(LGF::LGFWindow* window);
            Placeholder& setRect(const glm::vec2& pos, const glm::vec2& size);
            Placeholder& setMinimumSize(const glm::vec2& size);
            Placeholder& setMaximumSize(const glm::vec2& size);
            ~Placeholder();
            private:
            void updatePanel();
            glm::vec2 size;
            glm::vec2 maxSize;
            glm::vec2 minSize;
        };
};
