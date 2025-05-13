// #include "document.hpp"
#include "controls.cpp"
#include "editor.cpp"
#include "registerTable.cpp"
#include "settings.cpp"

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

    auto controlsComponent = Controls();
    auto editorComponent = Editor(filePath, fileLines);
    auto registerTableComponent = RegisterTable();
    auto settingsComponent = Settings();

    static int editorRegistersWidth = 80;

    auto editorRegistersSplitter = ResizableSplitLeft(
        editorComponent,
        registerTableComponent,
        &editorRegistersWidth
    );

    auto appLayout = Container::Vertical({
        controlsComponent,
        settingsComponent,
        editorRegistersSplitter
    });

    auto document_renderer = Renderer(appLayout, [
        controlsComponent,
        settingsComponent,
        editorRegistersSplitter
    ] {
        return vbox({
            controlsComponent->Render(),
            hbox({
                settingsComponent->Render() | flex | size(WIDTH, EQUAL, 40),
                editorRegistersSplitter->Render() | flex,
            }) | flex,
        });
    });

    return document_renderer;
}