#pragma once

#include "lgf/widgets/widget.h"
#include "lgf/events.h"

namespace LGF::Widgets {
        class Layout : public LGF::Widgets::Widget<Layout> {
            public:
            Layout(LGF::LGFWindow* window);
        };
};