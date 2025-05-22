#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/screen/color.hpp>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <iostream>
#include "../logger.h"

/**
 * @brief Creates the editor component for displaying lines of code.
 * 
 * @param filePath The path of the file being edited.
 * @param fileLines The lines of code to be displayed.
 * 
 * @return ftxui::Component The editor component.
 */
ftxui::Component Editor(const std::string &filePath, std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>> &fileLines, int &currentLine) {
    using namespace ftxui;
    currentLine = fileLines[0].first.first;

    int lineCount  = fileLines.size();
    int digitsCount = std::to_string(lineCount).length();

    // Holds the state of each line's breakpoint.
    for (int i = 0; i < lineCount; ++i) {
        fileLines[i].first.second = new bool(false);
    }
    
    // Vector to track the components for each line.
    std::vector<Component> lineComponents;

    // Create a editor line for each line of code from a checkbox and text
    for (int i = 0; i < lineCount; ++i) {
        // The checkbox for the current line of code.
        CheckboxOption checkboxOptions = CheckboxOption{};
        checkboxOptions.on_change = [&fileLines, i] {
            // Toggle the breakpoint state when the checkbox is clicked.
            Logger::info("Breakpoint at line " + std::to_string(i + 1) + ": " + (*fileLines[i].first.second ? "Enabled" : "Disabled"));
        };
        auto checkbox = Checkbox("", fileLines[i].first.second, checkboxOptions);

        // Renders the line with the checkbox and the code.
        auto line = Renderer(checkbox, [i, checkbox, &fileLines, currentLine, digitsCount] {
            return hbox({
                text(" ") | ( fileLines[i].first.first ? bgcolor(Color::Orange1) : bgcolor(Color::Black)),
                text(std::string(digitsCount - std::to_string(i + 1).length(), ' ') + std::to_string(i + 1))
                    | ( fileLines[i].first.first ? bgcolor(Color::Orange1)  | color(Color::Black) : bgcolor(Color::Black) | color(Color::GrayDark))
                    | align_right
                    | size(WIDTH, EQUAL, digitsCount),
                text(" ") | ( fileLines[i].first.first ? bgcolor(Color::Orange1) : bgcolor(Color::Black)),
                checkbox->Render() | ( fileLines[i].first.first ? bgcolor(Color::Orange1) | color(Color::Black) : bgcolor(Color::Black) | color(Color::White)),
                (text(fileLines[i].second.second) | ( fileLines[i].first.first ? bgcolor(Color::Orange1) | color(Color::Black) : *fileLines[i].first.second ? bgcolor(Color::IndianRed1) | color(Color::Black) : bgcolor(Color::Black)) | xflex_grow) 
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