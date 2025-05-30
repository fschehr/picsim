#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/table.hpp>
#include <string>
#include <vector>
#include "../../simvm.h"

/**
 * @brief Creates a component that represents the flags of the CPU.
 * 
 * @return A ftxui::Component presenting the values set in the flags.
 */
ftxui::Component Flags(PicSimulatorVM &vm) {
    using namespace ftxui;

//    static bool zFlagSet = false;
//    static bool dcFlagSet = false;
//    static bool cFlagSet = false;
    
    auto flags_renderer = Renderer([&] {
        // auto zFlagCopy = vm.executor.checkZeroFlag(vm.executor.getWorkingRegister());
        // auto dcFlagCopy = vm.executor.checkDigitCarryFlag(vm.executor.isCarryFlag());
        auto zFlagCopy = vm.executor.isZeroFlag();
        auto dcFlagCopy = vm.executor.isDigitCarryFlag();
        auto cFlagCopy = vm.executor.isCarryFlag();

        auto table = Table({
            {
                text("Z") | center | flex,
                text("DC") | center | flex,
                text("C") | center | flex,
            },
            {
                text(zFlagCopy ? "1" : "0") | flex,
                text(dcFlagCopy ? "1" : "0") | flex,
                text(cFlagCopy ? "1" : "0") | flex,
            }
        });
        
        table.SelectRow(0).Border(LIGHT);
        table.SelectRow(0).SeparatorVertical(LIGHT);
        
        table.SelectRow(1).Border(LIGHT);

        return window(
            text(" Flags "),
            table.Render() | flex
        );
    });

    return flags_renderer;
}