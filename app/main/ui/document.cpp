// #include "document.hpp"
#include "controls.cpp"
#include "editor.cpp"
#include "registerTable.cpp"
#include "settings.cpp"
#include "stats.cpp"
#include "runtime.cpp"
#include "ledArray.cpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <vector>
#include <string>

/**
 * @brief The main UI component
 * 
 * @param filePath The filepath from the program argument
 * @param fileLines The lines of code from the file
 * 
 * @return ftxui::Component The component representing the application
 */
ftxui::Component Document(const std::string &filePath, const std::vector<std::string>& fileLines) {
    using namespace ftxui;

    static bool statsVisible = false;
    static int editorRegistersWidth = 33;

    static bool portBBits[8] = {false, false, false, false, false, false, false, false};
    
    static bool* portBPointers[8] = {
        &portBBits[0], &portBBits[1], &portBBits[2], &portBBits[3], 
        &portBBits[4], &portBBits[5], &portBBits[6], &portBBits[7]
    };

    auto controlsComponent = Controls(&statsVisible);
    auto runtimeComponent = Runtime();
    auto ledArrayComponent = LedArray(portBPointers);
    auto editorComponent = Editor(filePath, fileLines);
    auto registerTableComponent = RegisterTable();
    auto settingsComponent = Settings(portBPointers);
    auto statsComponent = Stats();

    auto editorRegistersSplitter = ResizableSplitLeft(
        registerTableComponent,
        Container::Vertical({
            Container::Horizontal({
                runtimeComponent | xflex,
                ledArrayComponent | xflex
            }),
            editorComponent,
        }),
        &editorRegistersWidth
    );

    auto appLayout = Container::Vertical({
        controlsComponent,
        statsComponent,
        settingsComponent,
        editorRegistersSplitter
    });

    auto document_renderer = Renderer(appLayout, [
        controlsComponent,
        statsComponent,
        settingsComponent,
        editorRegistersSplitter
    ] {
        return vbox({
            controlsComponent->Render(),
            statsVisible ? statsComponent->Render() : vbox({}) | size(WIDTH, EQUAL, 0),
            hbox({
                settingsComponent->Render() | flex | size(WIDTH, EQUAL, 40),
                editorRegistersSplitter->Render() | flex,
            }) | flex,
        });
    });

    return document_renderer;
}