#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <cctype>  // Für isdigit
#include <iostream> // Für std::cout

ftxui::Component Cycles() {
    using namespace ftxui;

    static std::string frequency;
    static std::string microseconds;
    static bool invalidInput = false;

    InputOption inputOption;
    inputOption.on_change = [&]() {
        invalidInput = false;
        if (!frequency.empty()) {
            microseconds = std::to_string(std::stoi(frequency) / 4);
            
            for (char c : frequency) {
                if (!std::isdigit(c)) {
                    invalidInput = true;
                    break;
                }
            }
        }
    };

    Component frequencyInput = Input(&frequency, "Frequency (Hz)", inputOption);

    auto container = Container::Vertical({
        frequencyInput
    });

    auto registers_renderer = Renderer(container, [
        frequencyInput
    ] {
        return window(
            text(" Cycles "),
            vbox({
                hbox({
                    frequencyInput->Render() | xflex,
                    text(" → " + microseconds + "us") | xflex
                }) | flex,
                invalidInput ? text("Invalid input") | bgcolor(Color::IndianRed) | color(Color::White) | xflex : vbox({}) | size(WIDTH, EQUAL, 0),
            }) | xflex
        );
    });

    return registers_renderer;
}