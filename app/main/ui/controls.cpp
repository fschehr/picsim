#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <cstdlib>

/**
 * @brief Creates the main set of control buttons for the PIC-Simulator.
 * 
 * @return ftxui::Component The component containing the control buttons.
 */
ftxui::Component Controls(bool *statsVisible) {
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

    auto GoButton = Button("Go", [] { 1 + 1; });
    auto ResetButton = Button("Reset", [] { 1 + 1; });
    auto IgnoreButton = Button("Ignore", [] { 1 + 1; });
    auto StepInButton = Button("Step In", [] { 1 + 1; });
    auto StepOutButton = Button("Step Out", [] { 1 + 1; });
    auto StepOverButton = Button("Step Over", [] { 1 + 1; });
    
    static std::string stats_label = "Show Stats";
    auto StatsButton = Button(&stats_label, [statsVisible] { 
        *statsVisible = !(*statsVisible);
        stats_label = *statsVisible ? "Hide Stats" : "Show Stats";
    });

    auto container = Container::Horizontal({
        OpenDocumentationButton,
        GoButton,
        ResetButton,
        IgnoreButton,
        StepInButton,
        StepOutButton,
        StepOverButton,
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
                    IgnoreButton->Render(),
                    StepInButton->Render(),
                    StepOutButton->Render(),
                    StepOverButton->Render()
                }) | xflex,
                hbox({
                    StatsButton->Render(),
                }) | align_right
            }) | xflex
        );
    });

    return controls_renderer;
}