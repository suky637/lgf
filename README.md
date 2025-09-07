# LGF
Landeria's GUI Framework (yes I know, very original name)

## /!\ THIS PROJECT IS IN WIP, IT IS NOT DONE /!\

Currently I support:
- Frames
- Buttons
- Images
- Labels

Dependancies:
- glad
- glm
- freetype

Please note there is only linux back-ends at the moment, windows back-ends will be developped in later stages of development. Also, it is quite easy to setup windows back-end.

Also please copy the shaders folder to your build folder to make it work.

## Example
```C++
#include <lgf/window.h>
#include <lgf/text.h>
#include <lgf/image.h>
#include <lgf/widgets/panel.h>
#include <lgf/widgets/layout.h>
#include <lgf/widgets/label.h>
#include <lgf/widgets/image_panel.h>
#include <lgf/widgets/button.h>

int main() {
    LGF::LGFWindow window{800, 600, "LGF Window"};
    window.setFillColour(0, 0, 0);

    // let's try rendering text :)
    LGF::Draw::Image img("images/imageTrust.png");

    LGF::Draw::Font font("fonts/LiberationSans-Regular.ttf", 36, &window);

    LGF::Widgets::Layout layout{&window};

    LGF::Widgets::ImagePanel bg{&window, &img};
    bg.setRect(glm::vec2(0.f), window.getBounds()->size)
        .setResizeDirection(LGF::Widgets::ResizeDirection::HORIZONTAL | LGF::Widgets::ResizeDirection::VERTICAL)
        .setColour(255, 255, 255, 255)
        .setAnchor(LGF::Widgets::Anchors::CENTRE)
        .setCornerRadius(0);
    
    window.onResize += [&] {
        bg.setMaximumSize(window.getBounds()->size);
    };

    
    auto panel = LGF::Widgets::Panel(&window);
    panel.setRect(glm::vec2(0.f), glm::vec2(300.f, 400.f))
        .setAnchor(LGF::Widgets::Anchors::CENTRE)
        .setColour(0, 0, 0, 200)
        .setCornerRadius(15.f);
    
    LGF::Widgets::Button button1{&window};
    button1.setRect(glm::vec2(0.f), glm::vec2(300.f, 30.f))
        .setAnchor(LGF::Widgets::Anchors::CENTRE)
        .setColour(32, 32, 32, 255)
        .setCornerRadius(15.f);


    LGF::Widgets::Label button1Label{&window, &font};
    button1Label.setAnchor(LGF::Widgets::Anchors::CENTRE)
        .setPosition(glm::vec2(0.f))
        .setText("Hello, world!")
        .setColour(255, 255, 255, 255);
    
    layout.addChild(&bg);
    bg.addChild(&panel);
    panel.addChild(&button1);
    button1.addChild(&button1Label);
    
    layout.setActive(true);

    while (!window.windowShouldClose()) {
        window.pollEvents();
        if (button1.clicked) {
            std::cout << "Clicked!\n";
        }
        window.render();
    }
    return 0;
}
```

This should render this:
![Window output](https://github.com/suky637/lgf/blob/main/screenshots/2025-06-23.png)