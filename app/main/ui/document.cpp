#include "document.hpp"

/**
 * @brief The main UI component
 * 
 * @param filePath The filepath from the program argument
 * 
 * @param fileLines The lines of code from the file
 * 
 * @return ftxui::Component The component representing the application
 */
ftxui::Component Document(const std::string &filePath, std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>> &fileLines, PicSimulatorVM &vm) {
    using namespace ftxui;

    // UI Variables
    static bool statsVisible = false;
    static bool logsVisible = false;
    static bool resetsVisible = false;
    static int editorRegistersWidth = 33;
    static int currentLine = 0;
    
    // Sim Variables
    static std::string registerValues[32][8] = {};

    auto controlsComponent = Controls(&statsVisible, &logsVisible, &resetsVisible, vm);
    auto runtimeComponent = Runtime(vm);
    auto ledArrayComponent = LedArray(registerValues[0][6], registerValues[16][6]);
    auto editorComponent = Editor(filePath, fileLines, currentLine);
    // Callback für Registeränderungen, der die VM aktualisiert
    auto registerChangeCallback = [&vm](int address, uint8_t value) {
        // Setze den RAM-Wert in der VM
        vm.executor.setRamContent(address, value);
    };
    auto registerTableComponent = RegisterTable(vm, registerValues, registerChangeCallback);
    auto settingsComponent = Settings(
        registerValues[0][3], // Status Register
        registerValues[16][1], // Option Register
        registerValues[0][5], // Port A Pins
        registerValues[0][6], // Port B Pins
        registerValues[16][5], // TRISA
        registerValues[16][6],  // TRISB
        vm
    );
    auto logsComponent = Logs();
    auto statsComponent = Stats(
        vm,
        registerValues[0][3], // status register
        registerValues[0][2], // program counter low
        registerValues[1][2], // program counter high latch;
        registerValues[0][4] // file select register
    );
    auto resetsComponent = Resets(vm);
    
    auto runtimeLedContainer = Container::Horizontal({
        runtimeComponent | xflex,
        ledArrayComponent | xflex
    });
    
    auto editorWithRuntimeContainer = Container::Vertical({
        runtimeLedContainer,
        editorComponent
    });

    auto editorRegistersSplitter = ResizableSplitLeft(
        registerTableComponent,
        editorWithRuntimeContainer,
        &editorRegistersWidth
    );

    auto appLayout = Container::Vertical({
        controlsComponent,
        logsComponent,
        resetsComponent,
        statsComponent,
        settingsComponent,
        editorRegistersSplitter
    });

    auto document_renderer = Renderer(appLayout, [
        &vm,
        controlsComponent,
        logsComponent,
        resetsComponent,
        statsComponent,
        settingsComponent,
        editorRegistersSplitter,
        runtimeComponent,
        ledArrayComponent
    ] {
        static auto lastUpdateTime = std::chrono::steady_clock::now();
        static std::array<std::array<uint8_t, 8>, 32> lastValues = {};
        static bool firstRun = true;
        
        auto now = std::chrono::steady_clock::now();
        // Update registers every frame to ensure smooth updates
        bool changed = false;
        for (int i = 0; i < 32; ++i) {
            for (int j = 0; j < 8; ++j) {
                uint8_t value = vm.executor.getRamContent(i * 8 + j);
                if (firstRun || value != lastValues[i][j]) {
                    lastValues[i][j] = value;
                    std::stringstream ss;
                    ss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << static_cast<int>(value);
                    registerValues[i][j] = ss.str();
                    changed = true;
                }
            }
        }
        firstRun = false;

        // Force UI refresh every frame
        lastUpdateTime = now;

        return vbox({
            controlsComponent->Render(),
            logsVisible ? logsComponent->Render() : vbox({}) | size(WIDTH, EQUAL, 0),
            resetsVisible ? resetsComponent->Render() : vbox({}) | size(WIDTH, EQUAL, 0),
            statsVisible ? statsComponent->Render() : vbox({}) | size(WIDTH, EQUAL, 0),
            hbox({
                settingsComponent->Render() | flex | size(WIDTH, EQUAL, 40),
                editorRegistersSplitter->Render() | flex,
            }) | flex,
        });
    });

    return document_renderer;
}