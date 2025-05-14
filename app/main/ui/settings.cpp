#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

#include "settings_window/registers.cpp"
#include "settings_window/ioPins.cpp"
#include "settings_window/cycles.cpp"
#include "settings_window/flags.cpp"
#include "settings_window/stack.cpp"

/**
 * @brief Creates the Settings component.
 * 
 * @return ftxui::Component The Settings component.
 */
ftxui::Component Settings() {
    using namespace ftxui;

    auto registersComponent = Registers();
    auto ioPinsComponent = IoPins();
    auto cyclesComponent = Cycles();
    auto flagsComponent = Flags();
    auto stackComponent = Stack();

    auto container = Container::Vertical({
        cyclesComponent,
        flagsComponent,
        stackComponent,
        registersComponent,
        ioPinsComponent,
    });

    auto settings_renderer = Renderer(container, [
        cyclesComponent,
        flagsComponent,
        stackComponent,
        registersComponent,
        ioPinsComponent
    ] {
        return window(
            text(" Settings "),
            vbox({
                filler(),
                vbox({
                    cyclesComponent->Render() | xflex,
                    hbox({
                        flagsComponent->Render() | xflex,
                        stackComponent->Render() | xflex,
                    }) | xflex,
                    registersComponent->Render() | xflex,
                    ioPinsComponent->Render() | xflex
                }) | xflex,
                filler()
            }) | flex
        ) | flex;
    });

    return settings_renderer;
}