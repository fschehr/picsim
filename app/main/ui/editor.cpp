#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/color.hpp>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

struct BoolWrapper {
    bool value = false;
};

/**
 * @brief Combines a line of code and a Checkbox allowing to set breakpoints in a loaded program.
 * 
 * @param line The line of code to be displayed.
 * @param lineNumber The line number of the code.
 * @param breakpointState Pointer to a boolean indicating the state of the breakpoint.
 * @return ftxui::Component The component representing the line of code with a breakpoint checkbox.
 */
ftxui::Component EditorLine(const std::string &line, int lineNumber, bool *breakpointState) {
    using namespace ftxui;

    if (*breakpointState) {
        std::cerr << "DEBUG: breakpointState für Zeile '" << line << "' ist TRUE vor Checkbox-Erstellung." << std::endl;
    } else {
        std::cerr << "DEBUG: breakpointState für Zeile '" << line << "' ist FALSE vor Checkbox-Erstellung." << std::endl;
    }

    auto lineNumberElement = Renderer([lineNumber] {
        return text(std::to_string(lineNumber + 1)) | color(Color::GrayDark) | align_right | size(WIDTH, EQUAL, 3);
    });

    auto breakpointMark = Checkbox("", breakpointState);

    auto lineText = Renderer([lineCopy = line, breakpointState] {
        Element lineElement = text(lineCopy);

        if (*breakpointState) {
            lineElement = lineElement | bgcolor(Color::Red);
        }

        return lineElement | flex;
    });

    return Container::Horizontal({
        lineNumberElement,
        Renderer([] { return text(" ") | size(WIDTH, EQUAL, 1); }),
        breakpointMark,
        lineText,
    });
}

/**
 * @brief Creates the editor component for displaying lines of code.
 * 
 * @param fileLines The lines of code to be displayed.
 * @return ftxui::Component The editor component.
 */
ftxui::Component Editor(const std::string &filePath, const std::vector<std::string>& fileLines) {
    using namespace ftxui;

    auto breakpointStates = std::make_shared<std::vector<BoolWrapper>>(fileLines.size());
    std::fill(breakpointStates->begin(), breakpointStates->end(), BoolWrapper{false});
    
    for(size_t i = 0; i < breakpointStates->size(); ++i) {
       if ((*breakpointStates)[i].value) {
           std::cerr << "DEBUG: breakpointStates->at(" << i << ").value ist TRUE nach Initialisierung." << std::endl;
       }
    }

    auto container = Container::Vertical({
        
    });

    if (!fileLines.empty()) {
        for (size_t i = 0; i < fileLines.size(); ++i) {
            (*breakpointStates)[i].value = false; // explicitly set to false (-> seems to not work)
            container->Add(EditorLine(fileLines[i], i, &((*breakpointStates)[i].value)));
        }
    } else {
        return Renderer([=] {
            return window(
                text(" Editor "),
                text("No lines to display.") | center
            );
        });
    }

    auto editor_renderer = Renderer(container, [=] {
        if (container->ChildCount() == 0) {
            return window(
                text(" " + filePath + " "),
                text("No lines to display.") | center
            );
        } else {
            return window(
                text(" " + filePath + " "),
                container->Render() | vscroll_indicator | hscroll_indicator | frame | flex
            );
        }

    });

    return editor_renderer;
}