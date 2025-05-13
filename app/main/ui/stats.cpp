#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <string>

ftxui::Component Stats() {
    using namespace ftxui;

    static std::string pc = "0000";
    static std::string fsr = "00";
    static std::string stackPointer = "00";
    static std::string pcl = "00";
    static std::string pclath = "00";
    static std::string status = "00";
    static std::string vt = "00";
    static std::string wdt = "00";
    static std::string wRegister = "00";

    auto stats_renderer = Renderer([] {
        return window(
            text(" Stats "),
            hbox({
                hbox({
                    text(" Program Counter:") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + pc + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" FSR:") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + fsr + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" Stackpointer:") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + stackPointer + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" PCL:") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + pcl + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" PCLATH:") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + pclath + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" Status:") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + status + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" VT:") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + vt + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" WDT:") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + wdt + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                }),
                text("   "),
                hbox({
                    text(" W-Register:") | bgcolor(Color::White) | color(Color::Black) | bold,
                    text(" " + wRegister + " ") | bgcolor(Color::GrayLight) | color(Color::Black)
                })
            }) | center
        );
    });

    return stats_renderer;
}