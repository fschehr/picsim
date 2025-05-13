#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/table.hpp>
#include <string>
#include <vector>

/**
 * @brief Creates a component that represents the flags of the CPU.
 * 
 * @return A ftxui::Component presenting the values set in the flags.
 */
ftxui::Component Flags() {
    using namespace ftxui;

    static bool zFlagSet = false;
    static bool dcFlagSet = false;
    static bool cFlagSet = false;
    
    auto flags_renderer = Renderer([&] {
        auto table = Table({
            {
                text("Z"),
                text("DC"),
                text("C"),
            },
            {
                text(zFlagSet ? "1" : "0"),
                text(dcFlagSet ? "1" : "0"),
                text(cFlagSet ? "1" : "0"),}
        });
    
        table.SelectRow(0).Border(DOUBLE);
        table.SelectRow(0).SeparatorVertical(LIGHT);
        
        table.SelectRow(1).Border(LIGHT);

        return window(
            text(" Flags "),
            center(
                table.Render() | flex
            )
        );
    });

    return flags_renderer;
}