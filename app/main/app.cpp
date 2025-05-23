#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <thread>
#include <atomic>
#include <chrono>
#include "pars.h"
#include "simvm.h"
#include "logger.h"
#include "ui/document.cpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>

int main(int argc, char* argv[]) {
    using namespace ftxui;

    // Logger-Konfiguration
    Logger::setOutputToConsole();
    Logger::info("PicSim gestartet");
    
    Parser parser;
    std::string fileName;
    if (argc > 1) {
        fileName = argv[1];
    }

    std::string filePath = fileName; 
    
    std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>> fileLines = parser.parseToPair(filePath);
    std::vector<std::pair<short, short>> fileLinesShort = parser.parseToShortWithLines(filePath);

    // Initialize the simulator
    PicSimulatorVM vm(fileLines, fileLinesShort);

    try {
        vm.initialize();
    } catch (const std::exception& e) {
        Logger::warning(std::string("Error during initialization: ") + e.what());
        std::cin.get();
        return 1;
    }
    
    // ui
    auto screen = ScreenInteractive::Fullscreen();
    
    // Create a flag for controlling the refresh thread
    std::atomic<bool> refresh_ui_continue = true;
    
    // Create background refresh thread
    std::thread refresh_ui([&] {
        while (refresh_ui_continue) {
            std::this_thread::sleep_for(std::chrono::milliseconds(18)); // ~60 FPS
            screen.Post(Event::Custom);
        }
    });
    
    auto document = Document(filePath, fileLines, vm);
    screen.Loop(document);

    // Clean up the refresh thread
    refresh_ui_continue = false;
    if (refresh_ui.joinable()) {
        refresh_ui.join();
    }

    return 0;
}
