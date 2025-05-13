#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

/**
 * @brief Creates the Port A section of the I/O Pins component.
 * 
 * @return ftxui::Component The Port A component.
 */
ftxui::Component PortA() {
    using namespace ftxui;

    ftxui::Component columns[8] = {};
    ftxui::Component buttons[5] = {
        Button("0", [] {}),
        Button("0", [] {}),
        Button("0", [] {}),
        Button("0", [] {}),
        Button("0", [] {})
    };

    auto allButtons = Container::Horizontal({
        buttons[4] | xflex,
        buttons[3] | xflex,
        buttons[2] | xflex,
        buttons[1] | xflex,
        buttons[0] | xflex
    });

    for (int i = 7; i >= 0; --i) {
        if (i == 7 || i == 6 || i == 5) {
            columns[i] = Renderer([i] {
                return vbox({
                    text(std::to_string(i)) | center | flex,
                    text("o") | center | flex,
                    text("0") | center | flex,
                });
            });
        } else if (4 >= i || i >= 0) {
            columns[i] = Renderer(allButtons, [buttons, i] {
                return vbox({
                    text(std::to_string(i)) | center | flex,
                    text("i") | center | flex,
                    buttons[i]->Render() | center | flex
                });
            });
        }
    }

    auto all_columns = Container::Horizontal({
        columns[7] | xflex,
        columns[6] | xflex,
        columns[5] | xflex,
        columns[4] | xflex,
        columns[3] | xflex,
        columns[2] | xflex,
        columns[1] | xflex,
        columns[0] | xflex
    });

    auto portA_renderer = Renderer(all_columns, [all_columns] {
        return hbox({
            vbox({
                text("RA"),
                text("Tris"),
                text("Pin"), 
            }),
            all_columns->Render() | xflex
        });
    });

    return portA_renderer;
}

/**
 * @brief Creates the Port B section of the I/O Pins component.
 * 
 * @return ftxui::Component The Port B component.
 */
ftxui::Component PortB() {
    using namespace ftxui;

    ftxui::Component columns[8] = {};
    ftxui::Component buttons[8] = {
        Button("0", [] {}),
        Button("0", [] {}),
        Button("0", [] {}),
        Button("0", [] {}),
        Button("0", [] {}),
        Button("0", [] {}),
        Button("0", [] {}),
        Button("0", [] {})
    };
    
    auto allButtons = Container::Horizontal({
        buttons[7] | xflex,
        buttons[6] | xflex,
        buttons[5] | xflex,
        buttons[4] | xflex,
        buttons[3] | xflex,
        buttons[2] | xflex,
        buttons[1] | xflex,
        buttons[0] | xflex
    });

    for (int i = 7; i >= 0; --i) {
        columns[i] = Renderer(allButtons, [buttons, i] {
            return vbox({
                text(std::to_string(i)) | center | flex,
                text("i") | center | flex,
                buttons[i]->Render() | center | flex,
            });
        });
    }


    auto all_columns = Container::Horizontal({
        columns[7] | xflex,
        columns[6] | xflex,
        columns[5] | xflex,
        columns[4] | xflex,
        columns[3] | xflex,
        columns[2] | xflex,
        columns[1] | xflex,
        columns[0] | xflex
    });

    auto portB_renderer = Renderer(all_columns, [all_columns] {
        return hbox({
            vbox({
                text("RB"),
                text("Tris"),
                text("Pin"), 
            }),
            all_columns->Render() | xflex
        });
    });

    return portB_renderer;
}

/**
 * @brief Creates the I/O Pins component.
 */
ftxui::Component IoPins() {
    using namespace ftxui;

    auto portAComponent = PortA();
    auto portBComponent = PortB();

    auto container = Container::Vertical({
        portAComponent,
        portBComponent,
    });

    auto registers_renderer = Renderer(container, [
        portAComponent,
        portBComponent
    ] {
        return window(
            text(" I/O Pins "),
            vbox({
                text("Port A") | center | bgcolor(Color::Aquamarine1) | color(Color::Black),
                text(" ") | center,
                portAComponent->Render() | xflex,
                separatorDashed(),
                text("Port B") | center | bgcolor(Color::SeaGreen1) | color(Color::Black),
                text(" ") | center,
                portBComponent->Render() | xflex
            })
        );
    });

    return registers_renderer;
}