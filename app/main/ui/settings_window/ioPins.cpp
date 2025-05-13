#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

ftxui::Component IoPins() {
    using namespace ftxui;

    auto container = Container::Vertical({

    });

    auto registers_renderer = Renderer(container, [=] {
        return window(
            text(" I/O Pins "),
            center(
                hbox({
                    text("I/O Pins content goes here..."),
                })
            )
        );
    });

    return registers_renderer;
}