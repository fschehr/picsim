#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

// TODO: When tris for port b is set to i, then changing the values of port b should not change the led value for that pin
/**
 * @brief Creates the LED Array component.
 * 
 * @param portBBits Array of pointers to boolean values representing the state of Port B pins
 * @return ftxui::Component The LED Array component.
 */
ftxui::Component LedArray(std::string &portBHex, std::string &trisBHex) {
    using namespace ftxui;

    auto ledArray_renderer = Renderer([&portBHex, &trisBHex] {
        static bool trisBBits[8] = {false, false, false, false, false, false, false, false};
        unsigned int trisBValue = 0;
        for (int i = 0; i < 8; i++) {
            if (trisBHex.length() > 0) {
                std::stringstream ss;
                ss << std::hex << trisBHex;
                ss >> trisBValue;
            }
            trisBBits[i] = (trisBValue & (1 << 0)) != 0;
        }

        static bool raLedArray[8] = {false, false, false, false, false, false, false, false};
        unsigned int portBValue = 0;
        
        if (portBHex.length() > 0) {
            std::stringstream ss;
            ss << std::hex << portBHex;
            ss >> portBValue;
        }
    
        for (int i = 0; i < 8; i++) {
            raLedArray[i] = (portBValue & (1 << i)) != 0;
        }

        return window(
            text(" LED-Array "),
            hbox({
                text("  ") | (raLedArray[7] && !trisBBits[7] ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (raLedArray[6] && !trisBBits[6] ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (raLedArray[5] && !trisBBits[5] ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (raLedArray[4] && !trisBBits[4] ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (raLedArray[3] && !trisBBits[3] ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (raLedArray[2] && !trisBBits[2] ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (raLedArray[1] && !trisBBits[1] ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
                text(" "),
                text("  ") | (raLedArray[0] && !trisBBits[0] ? bgcolor(Color::RedLight) : bgcolor(Color::DarkRed)),
            }) | center
        );
    });

    return ledArray_renderer;
}