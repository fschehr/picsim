#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

#include "settings_window/registers.cpp"
#include "settings_window/ioPins.cpp"
#include "settings_window/cycles.cpp"
#include "settings_window/flags.cpp"
#include "settings_window/stack.cpp"
#include "settings_window/ledArray.cpp"

/**
 * @brief Creates the Settings component.
 * 
 * @return ftxui::Component The Settings component.
 */
ftxui::Component Settings() {
    using namespace ftxui;

    static bool portBBits[8] = {false, false, false, false, false, false, false, false};
    
    static bool* portBPointers[8] = {
        &portBBits[0], &portBBits[1], &portBBits[2], &portBBits[3], 
        &portBBits[4], &portBBits[5], &portBBits[6], &portBBits[7]
    };

    auto registersComponent = Registers();
    auto ioPinsComponent = IoPins(portBPointers);
    auto cyclesComponent = Cycles();
    auto flagsComponent = Flags();
    auto stackComponent = Stack();
    auto ledArrayComponent = LedArray(portBPointers);

    auto container = Container::Vertical({
        cyclesComponent,
        flagsComponent,
        stackComponent,
        registersComponent,
        ioPinsComponent,
        ledArrayComponent
    });

    auto settings_renderer = Renderer(container, [
        cyclesComponent,
        flagsComponent,
        stackComponent,
        registersComponent,
        ioPinsComponent,
        ledArrayComponent
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
                    ioPinsComponent->Render() | xflex,
                    ledArrayComponent->Render() | xflex,
                }) | xflex,
                filler()
            }) | flex
        ) | flex;
    });

    return settings_renderer;
}