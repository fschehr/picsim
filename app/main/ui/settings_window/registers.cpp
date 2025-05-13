#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/component/component.hpp>

ftxui::Component StatusRegisterColumn(std::string columnName) {
    using namespace ftxui;

    auto button = Button("0", [] {} );
    
    auto column = Renderer(button, [button, columnName] {
        return vbox({
            text(columnName) | center,
            button->Render() | center
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
        col_irp, col_rp1, col_rp0, col_to, col_pd, col_z, col_dc, col_c
    });
    
    // Erstelle den Renderer für das gesamte Status-Register
    auto statusRegisters_renderer = Renderer(all_columns, [all_columns] {
        return vbox({
            text("Status Register") | center,
            all_columns->Render() | flex
        });
    });

    return statusRegisters_renderer;
}

ftxui::Component Registers() {
    using namespace ftxui;

    auto StatusRegisterComponent = StatusRegister();
    // auto WRegisterComponent = WRegister();

    auto container = Container::Vertical({
        StatusRegisterComponent,
        // WRegisterComponent,
    });

    auto registers_renderer = Renderer(container, [
        StatusRegisterComponent
        // WRegisterComponent
    ] {
        return window(
            text(" Registers "),
            vbox({
                StatusRegisterComponent->Render() | flex | center,
                separatorDashed(),
                // WRegisterComponent->Render() | flex,
            })
        );
    });

    return registers_renderer;
}