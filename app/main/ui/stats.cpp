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

        std::string pc = std::to_string(vm.executor.getProgramCounter() & 0xFF);
        std::string stackPointer = std::to_string(vm.getStack().peek());

        return window(
            text(" Stats "),
            hbox({
                hbox({
                    text(" PC ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" 00"
                        + (pcl.length() == 1 ? "0" + pcl : pcl.length() == 0 ? "00" : pcl)
                        + " "
                    ) | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" PCLATH ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + (pclath.length() == 1 ? "0" + pclath : pclath.length() == 0 ? "00" : pclath) + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" PCL ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + (pcl.length() == 1 ? "0" + pcl : pcl.length() == 0 ? "00" : pcl) + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" FSR ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + (fsr.length() == 1 ? "0" + fsr : fsr.length() == 0 ? "00" : fsr) + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" Status ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + (status.length() == 1 ? "0" + status : status.length() == 0 ? "00" : status) + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
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