# LGF
Landeria's GUI Framework (yes I know, very original name)

## /!\ THIS PROJECT IS IN WIP, IT IS NOT DONE /!\

Currently I support:
- Frames

Dependancies:
- glad
- glm

Please note there is only linux back-ends at the moment, windows back-ends will be developped in later stages of development. Also, it is quite easy to setup windows back-end.

Also please copy the shaders folder to your build folder to make it work.

## Example
```C++
#include <lgf/window.h>
#include <lgf/widgets/panel.h>
#include <lgf/widgets/layout.h>

int main() {
    LGF::LGFWindow window{800, 600, "LGF Window"};
    window.setFillColour(100, 100, 100);

    LGF::Widgets::Layout layout{&window};

    LGF::Widgets::Panel panel{&window};
    panel.setAnchor(LGF::Widgets::Anchors::CENTRE);
    panel.setResizeDirection(LGF::Widgets::ResizeDirection::HORIZONTAL | LGF::Widgets::ResizeDirection::VERTICAL);
    panel.setRect(glm::vec2(0.f), glm::vec2(400.f));
    panel.setColour(0, 0, 0, 100);
    panel.setMinimumSize(glm::vec2(100.f));
    panel.setMaximumSize(glm::vec2(600.f));
    panel.setCornerRadius(16.f);

    layout.addChild(&panel);

    while (!window.windowShouldClose()) {
        window.pollEvents();
        window.render();
    }
    return 0;
}
```

This should render this:
![Window output](https://github.com/suky637/lgf/blob/main/screenshots/2025-06-11.png)