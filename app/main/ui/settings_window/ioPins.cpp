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
            if (tris_labels[0] == "o") return;
            pin_labels[0] = (pin_labels[0]) == "0" ? "1" : "0";
        }),
        Button(&pin_labels[1], [] {
            if (tris_labels[1] == "o") return;
            pin_labels[1] = pin_labels[1] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[2], [] {
            if (tris_labels[2] == "o") return;
            pin_labels[2] = pin_labels[2] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[3], [] {
            if (tris_labels[3] == "o") return;
            pin_labels[3] = pin_labels[3] == "0" ? "1" : "0";
        }),
        Button(&pin_labels[4], [] {
            if (tris_labels[4] == "o") return;
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
                    buttons[i]->Render() | (tris_labels[i] == "o" ? color(Color::GrayDark) : color(Color::White)) | center | flex,
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
ftxui::Component PortB(bool *portBBits[]) {
    using namespace ftxui;

    ftxui::Component columns[8] = {};
    
    static std::string tris_labels[8] = {
        "i", "i", "i", "i", "i", "i", "i", "i"
    };
    static bool disabled = false;

    static std::string button_labels[8] = {"0", "0", "0", "0", "0", "0", "0", "0"};
    
    for (int i = 0; i < 8; i++) {
        if (portBBits[i] != nullptr) {
            button_labels[i] = *(portBBits[i]) ? "1" : "0";
        }
    }

    ftxui::Component buttons[8] = {
        Button(&button_labels[0], [portBBits, index = 0] {
            if (tris_labels[0] == "o" || portBBits[0] == nullptr) return;
            *(portBBits[0]) = !*(portBBits[0]);
            button_labels[0] = *(portBBits[0]) ? "1" : "0";
        }),
        Button(&button_labels[1], [portBBits, index = 1] {
            if (tris_labels[1] == "o" || portBBits[1] == nullptr) return;
            *(portBBits[1]) = !*(portBBits[1]);
            button_labels[1] = *(portBBits[1]) ? "1" : "0";
        }),
        Button(&button_labels[2], [portBBits, index = 2] {
            if (tris_labels[2] == "o" || portBBits[2] == nullptr) return;
            *(portBBits[2]) = !*(portBBits[2]);
            button_labels[2] = *(portBBits[2]) ? "1" : "0";
        }),
        Button(&button_labels[3], [portBBits, index = 3] {
            if (tris_labels[3] == "o" || portBBits[3] == nullptr) return;
            *(portBBits[3]) = !*(portBBits[3]);
            button_labels[3] = *(portBBits[3]) ? "1" : "0";
        }),
        Button(&button_labels[4], [portBBits, index = 4] {
            if (tris_labels[4] == "o" || portBBits[4] == nullptr) return;
            *(portBBits[4]) = !*(portBBits[4]);
            button_labels[4] = *(portBBits[4]) ? "1" : "0";
        }),
        Button(&button_labels[5], [portBBits, index = 5] {
            if (tris_labels[5] == "o" || portBBits[5] == nullptr) return;
            *(portBBits[5]) = !*(portBBits[5]);
            button_labels[5] = *(portBBits[5]) ? "1" : "0";
        }),
        Button(&button_labels[6], [portBBits, index = 6] {
            if (tris_labels[6] == "o" || portBBits[6] == nullptr) return;
            *(portBBits[6]) = !*(portBBits[6]);
            button_labels[6] = *(portBBits[6]) ? "1" : "0";
        }),
        Button(&button_labels[7], [portBBits, index = 7] {
            if (tris_labels[7] == "o" || portBBits[7] == nullptr) return;
            *(portBBits[7]) = !*(portBBits[7]);
            button_labels[7] = *(portBBits[7]) ? "1" : "0";
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
        columns[i] = Renderer(allButtons, [buttons, i, portBBits] {
            // Aktualisiere das Button-Label basierend auf dem aktuellen Wert
            if (portBBits[i] != nullptr) {
                button_labels[i] = *(portBBits[i]) ? "1" : "0";
            }
            
            return vbox({
                text(std::to_string(i)) | center | flex,
                text(tris_labels[i]) | center | flex,
                buttons[i]->Render() | (tris_labels[i] == "o" ? color(Color::GrayDark) : color(Color::White)) | center | flex,
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
ftxui::Component IoPins(bool *portBBits[]) {
    using namespace ftxui;

    auto portAComponent = PortA();
    auto portBComponent = PortB(portBBits);

    auto container = Container::Vertical({
        portAComponent,
        portBComponent,
    });

    auto registers_renderer = Renderer(container, [
        portAComponent,
        portBComponent
    ] {
        return window(
            text(" I/O-Pins "),
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