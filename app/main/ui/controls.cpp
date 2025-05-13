// #include "controls.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

/**
 * @brief Creates the main set of control buttons for the PIC-Simulator.
 * 
 * @return ftxui::Component The component containing the control buttons.
 */
ftxui::Component Controls() {
    using namespace ftxui;

    auto GoButton = Button("Go", [] { 1 + 1; });
    auto ResetButton = Button("Reset", [] { 1 + 1; });
    auto IgnoreButton = Button("Ignore", [] { 1 + 1; });
    auto StepInButton = Button("Step In", [] { 1 + 1; });
    auto StepOutButton = Button("Step Out", [] { 1 + 1; });
    auto StepOverButton = Button("Step Over", [] { 1 + 1; });

    auto container = Container::Horizontal({
        GoButton,
        ResetButton,
        IgnoreButton,
        StepInButton,
        StepOutButton,
        StepOverButton
    });

    auto controls_renderer = Renderer(container, [=] {
        return window(
            text(" Controls "),
            center(
                hbox({
                    GoButton->Render(),
                    ResetButton->Render(),
                    IgnoreButton->Render(),
                    StepInButton->Render(),
                    StepOutButton->Render(),
                    StepOverButton->Render(),
                })
            )
        );
    });

    return controls_renderer;
}