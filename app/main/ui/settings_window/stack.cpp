#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

/**
 * @brief Creates a stack component to display the stack values.
 * 
 * @return ftxui::Component The stack component.
 */
ftxui::Component Stack() {
    using namespace ftxui;

    std::vector<std::string> stackValues = {
        "0000",
        "0000",
        "0000",
        "0000",
        "0000",
        "0000",
        "0000",
        "0000"
    };

    auto container = Container::Vertical({

    });

    auto registers_renderer = Renderer(container, [=] {
        return window(
            text(" Stack "),
            center(
                vbox({
                    text(stackValues[0]),
                    text(stackValues[1]),
                    text(stackValues[2]),
                    text(stackValues[3]),
                    text(stackValues[4]),
                    text(stackValues[5]),
                    text(stackValues[6]),
                    text(stackValues[7])
                })
            )
        );
    });

    return registers_renderer;
}