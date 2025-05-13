#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

ftxui::Component LedArray() {
    using namespace ftxui;

    bool raLedArray[8] = {false, false, false, false, false, false, false, false};
    bool rbLedArray[8] = {false, false, false, false, false, false, false, false};

    auto all_columns = Container::Horizontal({
        Button("RA0", [&raLedArray] { raLedArray[0] = !raLedArray[0]; }) | xflex,
    });

    auto ledArray_renderer = Renderer(all_columns, [all_columns] {
        return window(
            text(" LED Array "),
            hbox({
                all_columns->Render() | xflex,
            })
        );
    });

    return ledArray_renderer;
}