#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

/**
 * @brief Creates the LED Array component.
 * 
 * @param portBBits Array of pointers to boolean values representing the state of Port B pins
 * @return ftxui::Component The LED Array component.
 */
ftxui::Component LedArray(bool *portBBits[]) {
    using namespace ftxui;

    bool raLedArray[8] = {false, false, false, false, false, false, false, false};
    
    auto all_columns = Container::Horizontal({
        Button("RA0", [&raLedArray] { raLedArray[0] = !raLedArray[0]; }) | xflex,
    });

    auto ledArray_renderer = Renderer(all_columns, [all_columns, portBBits] {
        return window(
            text(" LED-Array "),
            hbox({
                text("  ") | (portBBits[7] != nullptr && *(portBBits[7]) ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (portBBits[6] != nullptr && *(portBBits[6]) ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (portBBits[5] != nullptr && *(portBBits[5]) ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (portBBits[4] != nullptr && *(portBBits[4]) ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (portBBits[3] != nullptr && *(portBBits[3]) ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (portBBits[2] != nullptr && *(portBBits[2]) ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (portBBits[1] != nullptr && *(portBBits[1]) ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (portBBits[0] != nullptr && *(portBBits[0]) ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
            }) | center
        );
    });

    return ledArray_renderer;
}