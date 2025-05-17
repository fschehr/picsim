#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <sstream>
#include <iomanip>
#include <string>

void updateHexValue(bool portBits[8], std::string &portHex) {
    unsigned int portValue = 0;
    for (int i = 0; i < 8; i++) {
        if (portBits[i]) {
            portValue |= (1 << i);
        }
    }
    std::stringstream ss;
    ss << std::hex << std::uppercase << portValue;
    portHex = ss.str();

    if (portHex.length() == 1) {
        portHex = "0" + portHex;
    }
}

/**
 * @brief Creates the Port A section of the I/O Pins component.
 * 
 * @param portAHex The hex value of Port A, stored in the register Array in document.cpp
 * 
 * @return ftxui::Component The Port A component.
 */
ftxui::Component PortA(std::string &portAHex, std::string &trisAHex) {
    using namespace ftxui;

    ftxui::Component columns[8] = {};

    static bool trisABits[8] = {false, false, false, false, false, false, false, false};
    static std::string tris_labels[8] = {};
    
    static bool portABits[8] = {false, false, false, false, false, false, false, false};
    static std::string button_labels[8] = {"0", "0", "0", "0", "0", "0", "0", "0"};

    ftxui::Component buttons[8] = {};

    for (int i = 0; i < 8; i++) {
        if (i < 5) {
            buttons[i] = Button(&button_labels[i], [&portAHex, i] {
                if (tris_labels[i] == "o") return;
                portABits[i] = !portABits[i];
                updateHexValue(portABits, portAHex);
                button_labels[i] = (portABits[i]) ? "1" : "0";
            });
        } else if (i < 8) {
            buttons[i] = Button(&button_labels[i], [] {});
        }
    }

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
        if (i >= 5) {
            columns[i] = Renderer(allButtons, [buttons, i, &portAHex, &trisAHex] {
                unsigned int trisAValue = 0;
                if (trisAHex.length() > 0) {
                    std::stringstream ss;
                    ss << std::hex << trisAHex;
                    ss >> trisAValue;
                }

                trisABits[i] = (trisAValue & (1 << i)) != 0;
                if (trisABits[i]) {
                    tris_labels[i] = "i";
                } else {
                    tris_labels[i] = "o";
                }

                unsigned int portAValue = 0;
                if (portAHex.length() > 0) {
                    std::stringstream ss;
                    ss << std::hex << portAHex;
                    ss >> portAValue;
                }

                portABits[i] = (portAValue & (1 << i)) != 0;
                if (tris_labels[i] == "i") {
                    button_labels[i] = portABits[i] ? "1" : "0";
                }

                return vbox({
                    text(std::to_string(i)) | center | flex,
                    text(tris_labels[i]) | center | flex,
                    buttons[i]->Render() | (tris_labels[i] == "o" ? color(Color::GrayDark) : color(Color::White)) | center | flex,
                });
            });
        } else if (i < 5) {
            columns[i] = Renderer(allButtons, [buttons, i, &portAHex, &trisAHex] {
                unsigned int trisAValue = 0;
                if (trisAHex.length() > 0) {
                    std::stringstream ss;
                    ss << std::hex << trisAHex;
                    ss >> trisAValue;
                }

                trisABits[i] = (trisAValue & (1 << i)) != 0;
                if (trisABits[i]) {
                    tris_labels[i] = "i";
                } else {
                    tris_labels[i] = "o";
                }

                unsigned int portAValue = 0;
                if (portAHex.length() > 0) {
                    std::stringstream ss;
                    ss << std::hex << portAHex;
                    ss >> portAValue;
                }

                portABits[i] = (portAValue & (1 << i)) != 0;
                if (tris_labels[i] == "i") {
                    button_labels[i] = portABits[i] ? "1" : "0";
                }

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
ftxui::Component PortB(std::string &portBHex, std::string &trisBHex) {
    using namespace ftxui;

    ftxui::Component columns[8] = {};
    
    static bool trisBBits[8] = {false, false, false, false, false, false, false, false};
    static std::string tris_labels[8] = {};

    static bool portBBits[8] = {false, false, false, false, false, false, false, false};
    static std::string button_labels[8] = {"0", "0", "0", "0", "0", "0", "0", "0"};

    ftxui::Component buttons[8] = {};
    for (int i = 0; i < 8; i++) {
        buttons[i] = Button(&button_labels[i], [&portBHex, i] {
            if (tris_labels[i] == "o") return;
            portBBits[i] = !portBBits[i];
            updateHexValue(portBBits, portBHex);
            button_labels[i] = (portBBits[i]) ? "1" : "0";
        });
    }
    
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
        columns[i] = Renderer(allButtons, [buttons, i, &portBHex, &trisBHex] {
            unsigned int trisBValue = 0;
            if (trisBHex.length() > 0) {
                std::stringstream ss;
                ss << std::hex << trisBHex;
                ss >> trisBValue;
            }

            trisBBits[i] = (trisBValue & (1 << i)) != 0;
            if (trisBBits[i]) {
                tris_labels[i] = "i";
            } else {
                tris_labels[i] = "o";
            }

            unsigned int portBValue = 0;
            if (portBHex.length() > 0) {
                std::stringstream ss;
                ss << std::hex << portBHex;
                ss >> portBValue;
            }
            
            portBBits[i] = (portBValue & (1 << i)) != 0;
            if (tris_labels[i] == "i") {
                button_labels[i] = portBBits[i] ? "1" : "0";
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
ftxui::Component IoPins(
    std::string &portAHex,
    std::string &portBHex,
    std::string &trisAHex,
    std::string &trisBHex
) {
    using namespace ftxui;

    auto portAComponent = PortA(portAHex, trisAHex);
    auto portBComponent = PortB(portBHex, trisBHex);

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