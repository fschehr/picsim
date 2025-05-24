#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <cstdlib>
#include <iostream>
#include "../simvm.h"
#include "../logger.h"

/**
 * @brief Creates the main set of control buttons for the PIC-Simulator.
 * 
 * @return ftxui::Component The component containing the control buttons.
 */
ftxui::Component Controls(bool *statsVisible, bool *logsVisible, bool *resetsVisible, PicSimulatorVM &vm) {
    using namespace ftxui;

    auto OpenDocumentationButton = Button("Docs", [] {
        #ifdef _WIN32
            system("start ..\\documentation\\Doku_Simulator_20200604.pdf");
        #elif defined(__APPLE__)
            system("open documentation/Doku_Simulator_20200604.pdf");
        #else
            system("xdg-open documentation/Doku_Simulator_20200604.pdf");
        #endif
    });

    auto GoButton = Button("Go", [&vm] { vm.execute(); });
    auto ResetButton = Button("Reset", [&vm] { vm.reset(); });
    auto HaltButton = Button("Halt", [&vm] { vm.halt(); });
    auto StepButton = Button("Step", [&vm] { vm.executeStep(); });
    
    static std::string stats_label = "Show Stats";
    auto StatsButton = Button(&stats_label, [statsVisible] { 
        *statsVisible = !(*statsVisible);
        stats_label = *statsVisible ? "Hide Stats" : "Show Stats";
    });
    static std::string logs_label = "Show Logs";
    auto LogsButton = Button(&logs_label, [logsVisible] {
        *logsVisible = !(*logsVisible);
        logs_label = *logsVisible ? "Hide Logs" : "Show Logs";
    });
    static std::string resets_label = "Show Resets";
    auto ResetsButton = Button(&resets_label, [resetsVisible] {
        *resetsVisible = !(*resetsVisible);
        resets_label = *resetsVisible ? "Hide Resets" : "Show Resets";
    });

    auto container = Container::Horizontal({
        OpenDocumentationButton,
        GoButton,
        ResetButton,
        HaltButton,
        StepButton,
        LogsButton,
        ResetsButton,
        StatsButton
    });

    auto controls_renderer = Renderer(container, [
        &vm,
        OpenDocumentationButton,
        GoButton,
        ResetButton,
        HaltButton,
        StepButton,
        ResetsButton,
        LogsButton,
        StatsButton
    ] {
        bool running = vm.getRunning();
        bool halted = vm.halted;

        return window(
            text(" Controls "),
            hbox({
                hbox({
                    OpenDocumentationButton->Render(),
                }) | xflex,
                hbox({
                    // Statusindikator mit begrenzter Höhe
                    vbox({
                        filler(),
                        text("  ") | (running && !halted ? bgcolor(Color::SeaGreen2) : bgcolor(Color::DarkGreen)),
                        filler()
                    }) | size(HEIGHT, LESS_THAN, 1),
                    text("  "),
                    GoButton->Render(),
                    ResetButton->Render(),
                    HaltButton->Render(),
                    // IgnoreButton->Render(),
                    StepButton->Render(),
                    // StepOutButton->Render(),
                    // StepOverButton->Render()
                }) | xflex,
                hbox({
                    LogsButton->Render(),
                    ResetsButton->Render(),
                    StatsButton->Render(),
                }) | align_right
            }) | xflex
        );
    });

    return controls_renderer;
}