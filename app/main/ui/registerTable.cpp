#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <sstream>
#include <functional> // Für std::function
#include "../simvm.h"

// Callback-Typ für Registeränderungen
using RegisterChangeCallback = std::function<void(int, uint8_t)>;

ftxui::Component RegisterTable(PicSimulatorVM &vm, std::string (&registerValues)[32][8], RegisterChangeCallback on_register_change = nullptr) {
    using namespace ftxui;

    auto container = Container::Vertical({});    for (int i = 0; i < 32; ++i) {
        auto row = Container::Horizontal({});
        
        for (int j = 0; j < 8; ++j) {
            std::stringstream hexPos;
            hexPos << std::hex << std::uppercase << ( 8 * i ) + j;
            
            auto input = Input(&registerValues[i][j], hexPos.str());
            row->Add(input);
        }
        
        container->Add(row);
    }

    auto register_renderer = Renderer(container, [=] {
        Elements rowElements;
        
        for (int i = 0; i < 32; ++i) {
            Elements cellElements;
            
            for (int j = 0; j < 8; ++j) {
                // Element cellElement = container->ChildAt(i)->ChildAt(j)->Render();
                // cellElements.push_back(cellElement | size(ftxui::WIDTH, ftxui::EQUAL, 3));
                cellElements.push_back(container->ChildAt(i)->ChildAt(j)->Render() | size(ftxui::WIDTH, ftxui::EQUAL, 3));
            }

            std::stringstream rowStartPosHex;
            rowStartPosHex << std::hex << std::uppercase << (i * 8);
            std::string rowStartPos = rowStartPosHex.str();
            if (rowStartPos.length() == 1) {
                rowStartPos = "0" + rowStartPos;
            }

            std::stringstream rowEndPosHex;
            rowEndPosHex << std::hex << std::uppercase << (i * 8 + 7);
            std::string rowEndPos = rowEndPosHex.str();
            if (rowEndPos.length() == 1) {
                rowEndPos = "0" + rowEndPos;
            }
            
            rowElements.push_back(hbox(
                filler(),
                text(rowStartPos) | color(Color::GrayDark) | align_right,
                text(" "),
                cellElements,
                text(rowEndPos) | color(Color::GrayDark),
                filler()
            ));
        }
        
        return vbox(rowElements);
    });

    // Verpacke alles in ein Fenster
    auto registerTable_renderer = Renderer(container, [=] {
        return window(
            text(" Register Table "),
            vbox({
                filler(),
                register_renderer->Render(),
                filler(),
            }) | flex
        );
    });

    return registerTable_renderer;
}