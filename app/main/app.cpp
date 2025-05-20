#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream> // Hinzugefügt für Dateieingabe
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
  
    // std::vector<std::string> fileLines;
    // std::ifstream fileStream(filePath);
    // if (fileStream.is_open()) {
    //     std::string line;
    //     while (std::getline(fileStream, line)) {
    //         fileLines.push_back(line);
    //     }
    //     fileStream.close();
    // } else {
    //     fileLines.push_back("Fehler: Datei konnte nicht geöffnet werden: " + filePath);
    // }

    std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>> fileLines = parser.parseToPair(filePath);

    // Initialize the simulator
    PicSimulatorVM vm;

    try {
        
        int lineCount = 0;
        short* parsedLines = parser.parseToShort(filePath, lineCount);
        std::vector<short> lines(parsedLines, parsedLines + lineCount);
        for (int i = 0; i < lines.size(); i++){
            Logger::info(parsedLines);
        }
        

        vm.initialize(lines);
    } catch (const std::exception& e) {
        Logger::warning(std::string("Error during initialization: ") + e.what());
        std::cin.get();
        return 1;
    }
    
    // ui
    auto screen = ScreenInteractive::Fullscreen();
    auto document = Document(filePath, fileLines, vm);
    screen.Loop(document);


    std::cin.get(); // Wait for user input before closing
    return 0;
}
