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

    auto container = Container::Vertical({});
    
    auto registers_renderer = Renderer(container, [&vm] {
        auto stackCopy = vm.getStack().getStackContents();
        int stackPointer = stackCopy.size() - 1;

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
                    hbox({
                        (stackPointer == 0 ? text("* ") : text("  ")),
                        text(stackDisplay[0]),
                    }),
                    hbox({
                        (stackPointer == 1 ? text("* ") : text("  ")),
                        text(stackDisplay[1]),
                    }),
                    hbox({
                        (stackPointer == 2 ? text("* ") : text("  ")),
                        text(stackDisplay[2]),
                    }),
                    hbox({
                        (stackPointer == 3 ? text("* ") : text("  ")),
                        text(stackDisplay[3]),
                    }),
                    hbox({
                        (stackPointer == 4 ? text("* ") : text("  ")),
                        text(stackDisplay[4]),
                    }),
                    hbox({
                        (stackPointer == 5 ? text("* ") : text("  ")),
                        text(stackDisplay[5]),
                    }),
                    hbox({
                        (stackPointer == 6 ? text("* ") : text("  ")),
                        text(stackDisplay[6]),
                    }),
                    hbox({
                        (stackPointer == 7 ? text("* ") : text("  ")),
                        text(stackDisplay[7]),
                    })
                })
            )
        );
    });

    return registers_renderer;
}