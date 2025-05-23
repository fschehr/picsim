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
ftxui::Component Controls(bool *statsVisible, bool *logsVisible, PicSimulatorVM &vm) {
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
    // auto IgnoreButton = Button("Ignore", [] { 1 + 1; }); // TODO:
    auto StepButton = Button("Step", [&vm] { vm.executeStep(); });
    // auto StepOutButton = Button("Step Out", [] { 1 + 1; }); // TODO:
    // auto StepOverButton = Button("Step Over", [] { 1 + 1; }); // TODO:
    
    static std::string stats_label = "Show Stats";
    auto StatsButton = Button(&stats_label, [statsVisible] { 
        *statsVisible = !(*statsVisible);
        stats_label = *statsVisible ? "Hide Stats" : "Show Stats";
    });
    static std::string logs_label = "Hide Logs";  // Angepasst, da Logs standardmäßig angezeigt werden
    auto LogsButton = Button(&logs_label, [logsVisible] {
        *logsVisible = !(*logsVisible);
        logs_label = *logsVisible ? "Hide Logs" : "Show Logs";
    });

    auto container = Container::Horizontal({
        OpenDocumentationButton,
        GoButton,
        ResetButton,
        HaltButton,
        // IgnoreButton,
        StepButton,
        // StepOutButton,
        // StepOverButton,
        LogsButton,
        StatsButton
    });

    auto controls_renderer = Renderer(container, [=] {
        return window(
            text(" Controls "),
            hbox({
                hbox({
                    OpenDocumentationButton->Render(),
                }) | xflex,
                hbox({
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
                    StatsButton->Render(),
                }) | align_right
            }) | xflex
        );
    });

    return controls_renderer;
}