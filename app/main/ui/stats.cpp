#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <string>

ftxui::Component Stats(
    std::string &pcl,
    std::string &pclath,
    std::string &status,
    std::string &fsr
) {
    using namespace ftxui;

    // static std::string pc = pclath + pcl; // program counter: PCL && PCLATH
    // static std::string pcl = "00"; // program counter low: PC<7:0> - 0x02 (Bank 0) && 0x82 (Bank 1)
    // static std::string pclath = "00"; // program counter high latch: PC<12:8> - 0x0A (Bank 0) && 0x8A (Bank 1)
    // static std::string fsr = "00"; // file select register: FSR<7:0> - 0x04 (Bank 0) && 0x84 (Bank 1)
    static std::string stackPointer = "00"; // not in the register array
    // static std::string status = "00"; // status register: STATUS<7:0> - 0x03 (Bank 0) && 0x83 (Bank 1)
    static std::string vt = "00"; // ?
    static std::string wdt = "00"; // watchdog timer: inside the status register

    auto stats_renderer = Renderer([
        &pcl,
        &pclath,
        &status,
        &fsr
    ] {
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
                }),
                text("   "),
                hbox({
                    text(" VT ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + vt + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" WDT ") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + wdt + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                })
            }) | center
        );
    });

    return stats_renderer;
}