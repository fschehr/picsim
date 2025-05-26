#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <string>
#include "../simvm.h"

ftxui::Component Stats(
    PicSimulatorVM &vm,
    std::string &status,
    std::string &pcl,
    std::string &pclath,
    std::string &fsr
) {
    using namespace ftxui;

    auto stats_renderer = Renderer([
        &vm,
        &status,
        &pcl,
        &pclath,
        &fsr
    ] {
        // read the pc value from the vm and turn it into a string that shows the 4 bits
        // read the pclath value from the vm/register and show the bits as string

        // Convert values to hex strings with proper formatting
        std::string pc = std::to_string(vm.executor.getProgramCounter() & 0xFF);
        std::string actualPclath = std::to_string(vm.executor.getRamContent(0x0A) & 0xFF);  // PCLATH at 0x0A
        std::string actualPcl = std::to_string(vm.executor.getRamContent(0x02) & 0xFF);     // PCL at 0x02
        std::string actualFsr = std::to_string(vm.executor.getRamContent(0x04) & 0xFF);     // FSR at 0x04
        std::string actualStatus = std::to_string(vm.executor.getRamContent(0x03) & 0xFF);  // Status at 0x03

        static bool stackErrorLogged = false;
        std::string stackPointer = "00";
        try {
            int stackValue = vm.getStack().peek();
            stackPointer = std::to_string(stackValue).length() == 1 ? "0" + std::to_string(stackValue) : std::to_string(stackValue);
            stackErrorLogged = false;
        } catch (const std::out_of_range& e) {
            if (!stackErrorLogged) {
                Logger::error("Stack ist leer: " + std::string(e.what()));
                stackErrorLogged = true;
            }
        }

        return window(
            text(" Stats "),
            hbox({
                hbox({
                    text(" PC ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + pc + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" PCLATH ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + actualPclath + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" PCL ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + actualPcl + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" FSR ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + actualFsr + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" Status ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + actualStatus + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" SP ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + stackPointer + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                })
            }) | center
        );
    });

    return stats_renderer;
}