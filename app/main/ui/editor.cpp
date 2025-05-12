#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

ftxui::Component Editor() {
    using namespace ftxui;

    auto container = Container::Vertical({
        
    });

    auto editor_renderer = Renderer(container, [=] {
        return window(
            text(" Editor "),
            center(
                hbox({
                    text("Editor content goes here..."),
                })
            )
        );
    });

    return editor_renderer;
}