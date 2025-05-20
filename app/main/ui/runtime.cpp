#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <cctype>  // Für isdigit
#include <iostream> // Für std::cout
#include "../simvm.h"

ftxui::Component Runtime(PicSimulatorVM &vm) {
    using namespace ftxui;

    static std::string runtime = "0000";

    static std::string frequency; // in Hz
    // static std::string microseconds;
    static bool invalidInput = false;

    InputOption inputOption;
    inputOption.placeholder = std::to_string(vm.microseconds);
    inputOption.on_change = [&]() {
        invalidInput = false;
        if (!frequency.empty()) {
            for (char c : frequency) {
                if (!std::isdigit(c)) {
                    invalidInput = true;
                    break;
                } else {
                    vm.microseconds = std::stoi(frequency) / 4; // 4 / Cycles = Microseconds per Cycle
                }
            }
        }
    };

    Component frequencyInput = Input(&frequency, "Frequency (Hz)", inputOption);

    auto container = Container::Vertical({
        frequencyInput
    });

    auto registers_renderer = Renderer(container, [
        &vm,
        frequencyInput
    ] {
        return window(
            text(" Runtime "),
            vbox({
                hbox({
                    hbox({
                        frequencyInput->Render() | xflex,
                        center(
                            text("  →  ") | xflex
                        ),
                        text(std::to_string(vm.microseconds) + "µs") | align_right | xflex,
                    }) | flex,
                    text("  ⇒  ") | xflex,
                    text(" Runtime ") | bgcolor(Color::White) | color(Color::Black) | xflex,
                    text(" " + std::to_string(vm.executor.runtimeCounter) + "µs ") | bgcolor(Color::GrayLight) | color(Color::Black) | xflex,
                }) | center,
                invalidInput ? text("Invalid input") | bgcolor(Color::IndianRed) | color(Color::White) | xflex : vbox({}) | size(WIDTH, EQUAL, 0),
            }) | xflex
        );
    });

    return registers_renderer;
}