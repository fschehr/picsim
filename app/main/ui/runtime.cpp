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

    auto input = Input(&frequency, std::to_string(vm.microseconds));
    
    auto input_with_validation = CatchEvent(input, [&](Event event) {
        if (event == Event::Character(' ') || event == Event::Return || event == Event::Tab) {
            invalidInput = false;
            if (!frequency.empty()) {
                for (char c : frequency) {
                    if (!std::isdigit(c)) {
                        invalidInput = true;
                        break;                    } else {
                        vm.microseconds = (4 / std::stod(frequency)*1000000); // 4 / Cycles = Microseconds per Cycle
                    }
                }
            }
        }
        return false;
    });    auto container = Container::Vertical({
        input_with_validation
    });

    auto registers_renderer = Renderer(container, [&vm, container, &invalidInput] {
        Elements inner_elements = {
            container->ChildAt(0)->Render() | xflex,
            center(text("  →  ") | xflex),
            text(std::to_string(vm.microseconds) + "µs") | align_right | xflex,
        };
        
        Elements runtime_elements = {
            hbox(inner_elements) | flex,
            text("  ⇒  ") | xflex,
            text(" Runtime ") | bgcolor(Color::White) | color(Color::Black) | xflex,
            text(" " + std::to_string(vm.executor.runtimeCounter) + "cy. ") | bgcolor(Color::GrayLight) | color(Color::Black) | xflex,
        };
        
        Elements main_elements = {
            hbox(runtime_elements) | center,
        };
        
        if (invalidInput) {
            main_elements.push_back(text("Invalid input") | bgcolor(Color::IndianRed) | color(Color::White) | xflex);
        }
        
        return window(
            text(" Runtime "),
            vbox(main_elements) | xflex
        );
    });

    return registers_renderer;
}