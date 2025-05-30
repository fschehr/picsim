#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "settings_window/registers.cpp"
#include "settings_window/ioPins.cpp"
#include "settings_window/flags.cpp"
#include "settings_window/stack.cpp"
// #include "settings_window/reset.cpp"

/**
 * @brief Creates the Settings component.
 * 
 * @param portBBits Array of pointers to boolean values representing the state of Port B pins
 * 
 * @return ftxui::Component The Settings component.
 */
ftxui::Component Settings(
    std::string &statusHex,
    std::string &optionHex,
    std::string &portAHex,
    std::string &portBHex,
    std::string &trisAHex,
    std::string &trisBHex,
    PicSimulatorVM &vm
) {
    using namespace ftxui;

    auto registersComponent = Registers(
        vm,
        statusHex,
        optionHex
    );
    auto ioPinsComponent = IoPins(
        vm,
        portAHex,
        portBHex,
        trisAHex,
        trisBHex
    );
    auto flagsComponent = Flags(
        vm
    );
    auto stackComponent = Stack(vm);

    auto container = Container::Vertical({
        flagsComponent,
        stackComponent,
        registersComponent,
        ioPinsComponent
    });

    auto settings_renderer = Renderer(container, [
        flagsComponent,
        stackComponent,
        registersComponent,
        ioPinsComponent
    ] {
        return window(
            text(" Special File Registers "),
            vbox({
                filler(),
                vbox({
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