#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/component/component.hpp>

ftxui::Component StatusRegisterColumn(std::string columnName) {
    using namespace ftxui;

    auto button = Button("0", [] {} );
    
    auto column = Renderer(button, [button, columnName] {
        return vbox({
            text(columnName) | center | flex,
            button->Render() | center | flex
        });
    });

    return column;
}

ftxui::Component StatusRegister() {
    using namespace ftxui;
    
    // Erstelle 8 Spalten für die Status-Register-Bits
    auto col_irp = StatusRegisterColumn("IRP");
    auto col_rp1 = StatusRegisterColumn("RP1");
    auto col_rp0 = StatusRegisterColumn("RP0");
    auto col_to = StatusRegisterColumn("TO");
    auto col_pd = StatusRegisterColumn("PD");
    auto col_z = StatusRegisterColumn("Z");
    auto col_dc = StatusRegisterColumn("DC");
    auto col_c = StatusRegisterColumn("C");
    
    // Kombiniere alle Spalten horizontal
    auto all_columns = Container::Horizontal({
        col_irp | xflex,
        col_rp1 | xflex,
        col_rp0 | xflex,
        col_to | xflex,
        col_pd | xflex,
        col_z | xflex,
        col_dc | xflex,
        col_c | xflex
    }) | xflex;
    
    // Erstelle den Renderer für das gesamte Status-Register mit flex, um volle Breite zu nutzen
    auto statusRegisters_renderer = Renderer(all_columns, [all_columns] {
        return vbox({
            text("Status Register") | center | bgcolor(Color::Aquamarine1) | color(Color::Black),
            text(" ") | center,
            all_columns->Render() | xflex
        }) | xflex;
    });

    return statusRegisters_renderer;
}

ftxui::Component WRegister() {
    using namespace ftxui;

    auto wRegister_renderer = Renderer([] {
        return vbox({
            text("W Register") | center | bgcolor(Color::SeaGreen1) | color(Color::Black),
            text(" ") | center,
            hbox({
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex,
                text("0") | center | xflex
            }) | xflex
        });
    });

    return wRegister_renderer;
}

ftxui::Component Registers() {
    using namespace ftxui;

    auto StatusRegisterComponent = StatusRegister();
    auto WRegisterComponent = WRegister();

    auto container = Container::Vertical({
        StatusRegisterComponent,
        WRegisterComponent,
    });

    auto registers_renderer = Renderer(container, [
        StatusRegisterComponent,
        WRegisterComponent
    ] {
        return window(
            text(" Registers "),
            vbox({
                StatusRegisterComponent->Render() | flex,  // Entferne center, um volle Breite zu nutzen
                separatorDashed(),
                WRegisterComponent->Render() | flex,
            }) | xflex
        );
    });

    return registers_renderer;
}