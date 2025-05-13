// #include "document.hpp"
#include "controls.cpp"
#include "editor.cpp"
#include "registerTable.cpp"
#include "settings.cpp"
#include "stats.cpp"

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
    // add further global variables from backend here (pc, fsr, etc.) -> parameters for windows
    static int editorRegistersWidth = 40;

    auto controlsComponent = Controls(&statsVisible);
    auto editorComponent = Editor(filePath, fileLines);
    auto registerTableComponent = RegisterTable();
    auto settingsComponent = Settings();
    auto statsComponent = Stats();


    auto editorRegistersSplitter = ResizableSplitLeft(
        registerTableComponent,
        editorComponent,
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