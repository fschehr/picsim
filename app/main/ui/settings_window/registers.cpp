#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "../../utils.hpp"
#include "../../simvm.h"

/**
 * @brief Creates the Status Register component.
 * 
 * @return ftxui::Component The Status Register component.
 */
ftxui::Component StatusRegister(PicSimulatorVM &vm, std::string &statusHex) {
    using namespace ftxui;
    
    ftxui::Component columns[8] = {};
    static std::string column_labels[8] = {
        "IRP", "RP1", "RP0", "TO", "PD", "Z", "DC", "C"
    };
    static bool statusBits[8] = {false, false, false, false, false, false, false, false};
    static std::string status_labels[8] = {};

    ftxui::Component buttons[8] = {};

    for (int i = 0; i < 8; ++i) {
        buttons[i] = Button(&status_labels[i], [&vm, i] {
            statusBits[i] = !statusBits[i];
            // Convert statusBits to a hex value
            uint8_t statusValue = 0;
            for (int j = 0; j < 8; ++j) {
                if (statusBits[j]) {
                    statusValue |= (1 << j);
                }
            }
            vm.executor.setRamContent(RamMemory<uint8_t>::Bank::BANK_0, 0x03, statusValue);
            // updateHexValue(statusBits, statusHex);
            // status_labels[i] = (statusBits[i]) ? "1" : "0";
        });
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

    for (int i = 0; i <= 7; ++i) {
        columns[i] = Renderer(allButtons, [buttons, i, &statusHex] {
            unsigned int statusValue = 0;
            if (statusHex.length() > 0) {
                std::stringstream ss;
                ss << std::hex << statusHex;
                ss >> statusValue;
            }
            
            statusBits[i] = (statusValue & (1 << i)) != 0;
            status_labels[i] = (statusBits[i]) ? "1" : "0";
            
            return vbox({
                text(column_labels[7 - i]) | center | flex,
                buttons[i]->Render() | center | flex,
            });
        });
    }
    
    // Kombiniere alle Spalten horizontal
    auto all_columns = Container::Horizontal({
        columns[7]| xflex,
        columns[6]| xflex,
        columns[5]| xflex,
        columns[4]| xflex,
        columns[3]| xflex,
        columns[2]| xflex,
        columns[1]| xflex,
        columns[0]| xflex
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

ftxui::Component WRegister(PicSimulatorVM &vm) {
    using namespace ftxui;

    static bool wRegBits[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    // std::wstring wRegBits(L"00000000");
    static std::string wRegHex = "00";

    auto wRegister_renderer = Renderer([&vm] {
        uint8_t wRegCopy = vm.executor.getWorkingRegister();
        Logger::info("W-Register: " + std::to_string(wRegCopy));

        for (int i = 0; i < 8; i++) {
            wRegBits[7 - i] = (wRegCopy >> i) & 1;
        }

        // Update the hex value
        std::stringstream ss;
        ss << std::hex << static_cast<int>(wRegCopy);
        wRegHex = ss.str();
        if (wRegHex.length() < 2) {
            wRegHex = "0" + wRegHex;
        }

        return vbox({
            text("W-Register") | center | bgcolor(Color::SeaGreen1) | color(Color::Black),
            text(" ") | center,
            hbox({
                text(std::to_string(wRegBits[0])) | xflex,
                text(std::to_string(wRegBits[1])) | xflex,
                text(std::to_string(wRegBits[2])) | xflex,
                text(std::to_string(wRegBits[3])) | xflex,
                text(" "),
                text(std::to_string(wRegBits[4])) | xflex,
                text(std::to_string(wRegBits[5])) | xflex,
                text(std::to_string(wRegBits[6])) | xflex,
                text(std::to_string(wRegBits[7])) | xflex,
                text(" → ") | xflex,
                text("0x" + wRegHex) | xflex,
            }) | center
        });
    });

    return wRegister_renderer;
}

ftxui::Component Registers(PicSimulatorVM &vm, std::string &statusHex) {
    using namespace ftxui;

    auto StatusRegisterComponent = StatusRegister(
        vm,
        statusHex
    );
    auto WRegisterComponent = WRegister(vm);

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