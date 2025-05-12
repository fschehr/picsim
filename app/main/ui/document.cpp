// #include "document.hpp"
#include "controls.cpp"
#include "editor.cpp"
#include "registerTable.cpp"
#include "settings.cpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

ftxui::Component Document() {
    using namespace ftxui;

    auto controlsComponent = Controls();
    auto editorComponent = Editor();
    auto registerTableComponent = RegisterTable();
    auto settingsComponent = Settings();

    auto appLayout = Container::Vertical({
        controlsComponent,
        Container::Horizontal({
            settingsComponent,
            editorComponent,
            registerTableComponent,
        })
    });

    auto document_renderer = Renderer(appLayout, [
        controlsComponent,
        editorComponent,
        registerTableComponent,
        settingsComponent,
        appLayout
    ] {
        return vbox({
            controlsComponent->Render(),
            hbox({
                settingsComponent->Render() | flex,
                editorComponent->Render() | flex,
                registerTableComponent->Render() | flex,
            }) | flex,
        });
    });

    return document_renderer;
}