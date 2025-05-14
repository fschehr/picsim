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
    static std::string pin_labels[8] = {
        "0", "0", "0", "0", "0", "0", "0", "0"
    };
    static std::string tris_labels[8] = {
        "i", "i", "i", "i", "i", "o", "o", "o"
    };
    static bool disabled = false;
    
    ftxui::Component buttons[8] = {
        Button(&pin_labels[0], [] {
            pin_labels[0] = (pin_labels[0]) == "0" ? "1" : "0";
        }),
        Button(&pin_labels[1], [] {
            pin_labels[1] = pin_labels[1] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[2], [] {
            pin_labels[2] = pin_labels[2] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[3], [] {
            pin_labels[3] = pin_labels[3] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[4], [] {
            pin_labels[4] = pin_labels[4] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[5], [] {}),
        Button(&pin_labels[6], [] {}),
        Button(&pin_labels[7], [] {})
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
        if (i == 7 || i == 6 || i == 5) {
            columns[i] = Renderer(allButtons, [buttons, i] {
                return vbox({
                    text(std::to_string(i)) | center | flex,
                    text(tris_labels[i]) | center | flex,
                    buttons[i]->Render() | color(Color::GrayDark) | center | flex,
                });
            });
        } else {
            columns[i] = Renderer(allButtons, [buttons, i] {
                return vbox({
                    text(std::to_string(i)) | center | flex,
                    text(tris_labels[i]) | center | flex,
                    buttons[i]->Render() | center | flex,
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
    static std::string pin_labels[8] = {
        "0", "0", "0", "0", "0", "0", "0", "0"
    };
    static std::string tris_labels[8] = {
        "i", "i", "i", "i", "i", "o", "o", "o"
    };
    static bool disabled = false;

    ftxui::Component buttons[8] = {
        Button(&pin_labels[0], [] {
            pin_labels[0] = (pin_labels[0]) == "0" ? "1" : "0";
        }),
        Button(&pin_labels[1], [] {
            pin_labels[1] = pin_labels[1] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[2], [] {
            pin_labels[2] = pin_labels[2] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[3], [] {
            pin_labels[3] = pin_labels[3] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[4], [] {
            pin_labels[4] = pin_labels[4] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[5], [] {
            pin_labels[5] = pin_labels[5] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[6], [] {
            pin_labels[6] = pin_labels[6] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[7], [] {
            pin_labels[7] = pin_labels[7] == "0" ? "1" : "0";
        })
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
                text(tris_labels[i]) | center | flex,
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