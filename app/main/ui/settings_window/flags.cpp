#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

ftxui::Component Flags() {
    using namespace ftxui;

    auto container = Container::Vertical({

    });

    auto registers_renderer = Renderer(container, [=] {
        return window(
            text(" Flags "),
            center(
                hbox({
                    text("Flags content goes here..."),
                })
            )
        );
    });

    return registers_renderer;
}