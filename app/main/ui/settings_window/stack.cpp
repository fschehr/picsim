#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include "../../simvm.h"
/**
 * @brief Creates a stack component to display the stack values.
 * 
 * @return ftxui::Component The stack component.
 */
ftxui::Component Stack(PicSimulatorVM &vm) {
    using namespace ftxui;

    std::vector<std::string> stackValues = {
        "0000",
        "0000",
        "0000",
        "0000",
        "0000",
        "0000",
        "0000",
        "0000"
    };

    auto container = Container::Vertical({

    });

    auto registers_renderer = Renderer(container, [&vm] {
        auto stackCopy = vm.stack.getStackContents();
        std::vector<std::wstring> stackDisplay(8, L"0000");
        for (size_t i = 0; i < 8; ++i) {
            if (i < stackCopy.size()) {
                unsigned int value = static_cast<unsigned int>(stackCopy[i]) & 0xF; // Only 4 bits
                std::wstring bin;
                for (int b = 3; b >= 0; --b) {
                    bin += ((value >> b) & 1) ? L'1' : L'0';
                }
                stackDisplay[i] = bin;
            }
        }
        return window(
            text(" Stack "),
            center(
                vbox({
                    text(stackDisplay[0]),
                    text(stackDisplay[1]),
                    text(stackDisplay[2]),
                    text(stackDisplay[3]),
                    text(stackDisplay[4]),
                    text(stackDisplay[5]),
                    text(stackDisplay[6]),
                    text(stackDisplay[7])
                })
            )
        );
    });

    return registers_renderer;
}