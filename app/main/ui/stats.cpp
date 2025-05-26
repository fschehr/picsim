#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <string>
#include <sstream>
#include <iomanip>
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
        // std::string pc = std::to_string(vm.executor.getProgramCounter() & 0xFF);
        std::stringstream ssPc, ssPcl, ssPclath, ssStatus, ssFsr;
        ssPc << std::uppercase << std::setfill('0') << std::setw(4) << std::hex
              << (vm.executor.getProgramCounter() & 0xFFFF);
        std::string pc = ssPc.str();

        ssPclath << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
                  << (vm.executor.getRamContent(0x0A) & 0xFF);  // PCLATH at 0x0A
        std::string pclath = ssPclath.str();

        ssPcl << std::uppercase << std::setfill('0') << std::setw(2) << std::hex
                  << (vm.executor.getRamContent(0x02) & 0xFF);     // PCL at 0x02
        std::string pcl = ssPcl.str();

        ssFsr << std::uppercase << std::setfill('0') << std::setw(2) << std::hex 
             << (vm.executor.getRamContent(0x04) & 0xFF);
        std::string fsr = ssFsr.str();        
        
        ssStatus << std::uppercase << std::setfill('0') << std::setw(2) << std::hex 
                << (vm.executor.getRamContent(0x03) & 0xFF);
        std::string status = ssStatus.str();
        

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
                    text(" " + pclath + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" PCL ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + pcl + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" FSR ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + fsr + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" Status ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + status + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
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