#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

ftxui::Component Registers() {
    using namespace ftxui;

    auto container = Container::Vertical({

    });

    auto registers_renderer = Renderer(container, [=] {
        return window(
            text(" Registers "),
            center(
                hbox({
                    text("Registers content goes here..."),
                })
            )
        );
    });

    return registers_renderer;
}