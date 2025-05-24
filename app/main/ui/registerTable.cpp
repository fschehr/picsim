#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <sstream>
#include <functional> // Für std::function

// Callback-Typ für Registeränderungen
using RegisterChangeCallback = std::function<void(int, uint8_t)>;

ftxui::Component RegisterTable(std::string (&registerValues)[32][8], RegisterChangeCallback on_register_change = nullptr) {
    using namespace ftxui;

    auto container = Container::Vertical({});

    for (int i = 0; i < 32; ++i) {
        auto row = Container::Horizontal({});
        
        for (int j = 0; j < 8; ++j) {
            auto registerInputOption = InputOption();
            registerInputOption.content = &registerValues[i][j];
            std::stringstream hexPos;
            hexPos << std::hex << std::uppercase << ( 8 * i ) + j;
            registerInputOption.placeholder = hexPos.str(); // No° des Registers
            registerInputOption.multiline = false;
            
            registerInputOption.on_change = [i, j, &registerValues, on_register_change]() {
                if (registerValues[i][j].length() > 2) {
                    registerValues[i][j] = registerValues[i][j].substr(0, 2);
                }
                
                std::string validInput;
                for (char c : registerValues[i][j]) {
                    if (std::isxdigit(c)) {
                        validInput.push_back(std::toupper(c));
                    }
                }
                registerValues[i][j] = validInput;
                
                // Konvertiere den HEX-String in einen uint8_t Wert und aktualisiere den RAM
                if (!validInput.empty()) {
                    try {
                        uint8_t value = static_cast<uint8_t>(std::stoi(validInput, nullptr, 16));
                        // Berechne die RAM-Adresse basierend auf i und j
                        int address = i * 8 + j;
                        // Externe Funktion aufrufen, um den RAM zu aktualisieren
                        if (on_register_change) {
                            on_register_change(address, value);
                        }
                    } catch (const std::exception& e) {
                        // Fehlerbehandlung bei ungültiger Konvertierung
                    }
                }
            };
            
            row->Add(Input(&registerValues[i][j], registerInputOption));
        }
        
        container->Add(row);
    }

    auto register_renderer = Renderer(container, [=] {
        Elements rowElements;
        
        for (int i = 0; i < 32; ++i) {
            Elements cellElements;
            
            for (int j = 0; j < 8; ++j) {
                // Element cellElement = container->ChildAt(i)->ChildAt(j)->Render();
                // cellElements.push_back(cellElement | size(ftxui::WIDTH, ftxui::EQUAL, 3));
                cellElements.push_back(container->ChildAt(i)->ChildAt(j)->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 3));
            }

            std::stringstream rowStartPosHex;
            rowStartPosHex << std::hex << std::uppercase << (i * 8);
            std::string rowStartPos = rowStartPosHex.str();
            if (rowStartPos.length() == 1) {
                rowStartPos = "0" + rowStartPos;
            }

            std::stringstream rowEndPosHex;
            rowEndPosHex << std::hex << std::uppercase << (i * 8 + 7);
            std::string rowEndPos = rowEndPosHex.str();
            if (rowEndPos.length() == 1) {
                rowEndPos = "0" + rowEndPos;
            }
            
            rowElements.push_back(hbox(
                filler(),
                text(rowStartPos) | color(Color::GrayDark) | align_right,
                text(" "),
                cellElements,
                text(rowEndPos) | color(Color::GrayDark),
                filler()
            ));
        }
        
        return vbox(rowElements);
    });

    // Verpacke alles in ein Fenster
    auto registerTable_renderer = Renderer(container, [=] {
        return window(
            text(" Register Table "),
            vbox({
                filler(),
                register_renderer->Render(),
                filler(),
            }) | flex
        );
    });

    return registerTable_renderer;
}