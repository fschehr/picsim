#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>


ftxui::Component Cycles() {
    using namespace ftxui;

    std::string microseconds;
    int frequency;

    auto container = Container::Vertical({

    });

    auto registers_renderer = Renderer(container, [=] {
        return window(
            text(" Cycles "),
            center(
                hbox({
                    text("Cycles content goes here..."),
                })
            )
        );
    });

    return registers_renderer;
}