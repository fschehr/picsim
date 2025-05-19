#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/color.hpp>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <iostream>

/**
 * @brief Creates the editor component for displaying lines of code.
 * 
 * @param filePath The path of the file being edited.
 * @param fileLines The lines of code to be displayed.
 * 
 * @return ftxui::Component The editor component.
 */
ftxui::Component Editor(const std::string &filePath, const std::vector<std::string>& fileLines, int &currentLine) {
    using namespace ftxui;

    int lineCount  = fileLines.size();
    int digitsCount = std::to_string(lineCount).length();

    // Holds the state of each line's breakpoint.
    std::vector<bool*> breakpointsStatesList;
    for (int i = 0; i < lineCount; ++i) {
        breakpointsStatesList.push_back(new bool(false));
    }
    
    // Vector to track the components for each line.
    std::vector<Component> lineComponents;

    // Create a editor line for each line of code from a checkbox and text
    for (int i = 0; i < lineCount; ++i) {
        // The checkbox for the current line of code.
        auto checkbox = Checkbox("", breakpointsStatesList[i], CheckboxOption{});

        // Renders the line with the checkbox and the code.
        auto line = Renderer(checkbox, [i, checkbox, &fileLines, breakpointsStatesList, currentLine, digitsCount] {
            return hbox({
                text(" ") | ( currentLine == i ? bgcolor(Color::Orange1) : bgcolor(Color::Black)),
                text(std::string(digitsCount - std::to_string(i + 1).length(), ' ') + std::to_string(i + 1))
                    | ( currentLine == i ? bgcolor(Color::Orange1)  | color(Color::Black) : bgcolor(Color::Black) | color(Color::GrayDark))
                    | align_right
                    | size(WIDTH, EQUAL, digitsCount),
                text(" ") | ( currentLine == i ? bgcolor(Color::Orange1) : bgcolor(Color::Black)),
                checkbox->Render() | ( currentLine == i ? bgcolor(Color::Orange1) | color(Color::Black) : bgcolor(Color::Black) | color(Color::White)),
                (text(fileLines[i]) | ( currentLine == i ? bgcolor(Color::Orange1) | color(Color::Black) : *breakpointsStatesList[i] ? bgcolor(Color::IndianRed1) | color(Color::Black) : bgcolor(Color::Black)) | xflex_grow) 
            }) | xflex;
        });

        lineComponents.push_back(line);
    }
    
    // Container holding all the lines of code (needed for interactivity)
    auto container = Container::Vertical({lineComponents});

    // Renderer for the editor window component
    auto editor = Renderer(container, [container, filePath] {
        return window(
            text(" " + filePath + " "),
            container->Render() | yframe | yflex
        ) | flex;
    });

    return editor;
}