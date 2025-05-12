#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

ftxui::Component Stack() {
    using namespace ftxui;

    auto container = Container::Vertical({

    });

    auto registers_renderer = Renderer(container, [=] {
        return window(
            text(" Stack "),
            center(
                vbox({
                    text("0000"),
                    text("0000"),
                    text("0000"),
                    text("0000"),
                    text("0000"),
                    text("0000"),
                    text("0000"),
                    text("0000"),
                })
            )
        );
    });

    return registers_renderer;
}