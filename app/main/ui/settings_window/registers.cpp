#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/component/component.hpp>

// ftxui::Component StatusRegisterColumn(std::string columnName) {
//     using namespace ftxui;

//     auto button = Button("0", [] {} );
    
//     auto column = Renderer(button, [button, columnName] {
//         return vbox({
//             text(columnName) | center | flex,
//             button->Render() | center | flex
//         });
//     });

//     return column;
// }

/**
 * @brief Creates the Status Register component.
 * 
 * @return ftxui::Component The Status Register component.
 */
ftxui::Component StatusRegister() {
    using namespace ftxui;
    
    ftxui::Component columns[8] = {};
    static std::string column_labels[8] = {
        "IRP", "RP1", "RP0", "TO", "PD", "Z", "DC", "C"
    };
    static std::string status_labels[8] = {
        "0", "0", "0", "0", "0", "0", "0", "0"
    };

    ftxui::Component buttons[8] = {
        Button(&status_labels[0], [] {
            status_labels[0] = (status_labels[0]) == "0" ? "1" : "0";
        }),
        Button(&status_labels[1], [] {
            status_labels[1] = status_labels[1] == "0" ? "1" : "0";
        }),
        Button(&status_labels[2], [] {
            status_labels[2] = status_labels[2] == "0" ? "1" : "0";
        }),
        Button(&status_labels[3], [] {
            status_labels[3] = status_labels[3] == "0" ? "1" : "0";
        }),
        Button(&status_labels[4], [] {
            status_labels[4] = status_labels[4] == "0" ? "1" : "0";
        }),
        Button(&status_labels[5], [] {
            status_labels[5] = status_labels[5] == "0" ? "1" : "0";
        }),
        Button(&status_labels[6], [] {
            status_labels[6] = status_labels[6] == "0" ? "1" : "0";
        }),
        Button(&status_labels[7], [] {
            status_labels[7] = status_labels[7] == "0" ? "1" : "0";
        })
    };

    auto allButtons = Container::Horizontal({
        buttons[0] | xflex,
        buttons[1] | xflex,
        buttons[2] | xflex,
        buttons[3] | xflex,
        buttons[4] | xflex,
        buttons[5] | xflex,
        buttons[6] | xflex,
        buttons[7] | xflex
    });

    for (int i = 0; i <= 7; ++i) {
        columns[i] = Renderer(allButtons, [buttons, i] {
            return vbox({
                text(column_labels[i]) | center | flex,
                buttons[i]->Render() | center | flex,
            });
        });
    }
    
    // Kombiniere alle Spalten horizontal
    auto all_columns = Container::Horizontal({
        columns[0]| xflex,
        columns[1]| xflex,
        columns[2]| xflex,
        columns[3]| xflex,
        columns[4]| xflex,
        columns[5]| xflex,
        columns[6]| xflex,
        columns[7]| xflex
    });
    
    // Erstelle den Renderer für das gesamte Status-Register mit flex, um volle Breite zu nutzen
    auto statusRegisters_renderer = Renderer(all_columns, [all_columns] {
        return vbox({
            text("Status Register") | center | bgcolor(Color::Aquamarine1) | color(Color::Black),
            text(" ") | center,
            all_columns->Render() | xflex
        }) | xflex;
    });

    return statusRegisters_renderer;
}

ftxui::Component WRegister() {
    using namespace ftxui;

    auto wRegister_renderer = Renderer([] {
        return vbox({
            text("W Register") | center | bgcolor(Color::SeaGreen1) | color(Color::Black),
            text(" ") | center,
            hbox({
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex
            }) | xflex
        });
    });

    return wRegister_renderer;
}

ftxui::Component Registers() {
    using namespace ftxui;

    auto StatusRegisterComponent = StatusRegister();
    auto WRegisterComponent = WRegister();

    auto container = Container::Vertical({
        StatusRegisterComponent,
        WRegisterComponent,
    });

    auto registers_renderer = Renderer(container, [
        StatusRegisterComponent,
        WRegisterComponent
    ] {
        return window(
            text(" Registers "),
            vbox({
                StatusRegisterComponent->Render() | flex,  // Entferne center, um volle Breite zu nutzen
                separatorDashed(),
                WRegisterComponent->Render() | flex,
            }) | xflex
        );
    });

    return registers_renderer;
}