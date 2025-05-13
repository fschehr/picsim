#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream> // Hinzugefügt für Dateieingabe
#include "pars.h"
#include "simvm.h"
#include "ui/document.cpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <ftxui/component/screen_interactive.hpp>

int main(int argc, char* argv[]) {
    using namespace ftxui;

    std::string fileName;
    if (argc > 1) {
        fileName = argv[1];
    } else {
        fileName = "TPicSim1.LST";
    }

    std::string filePath = "../testprog/" + fileName; 

    // try {
    //     Parser parser;
    //     int lineCount = 0;
    //     short* parsedLines = parser.parse(filePath, lineCount);
    //     std::vector<short> lines(parsedLines, parsedLines + lineCount);
    //     for (const auto& line : lines) {
    //         std::cout << line << std::endl;
    //     }

    //     PicSimulatorVM vm;
    //     vm.initialize(lines);
    // } catch (const std::exception& e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    //     std::cin.get();
    //     return 1;
    // }

    std::vector<std::string> fileLines;
    std::ifstream fileStream(filePath);
    if (fileStream.is_open()) {
        std::string line;
        while (std::getline(fileStream, line)) {
            fileLines.push_back(line);
        }
        fileStream.close();
    } else {
        fileLines.push_back("Fehler: Datei konnte nicht geöffnet werden: " + filePath);
    }

    // ui
    auto screen = ScreenInteractive::Fullscreen();
    auto document = Document(filePath, fileLines);
    screen.Loop(document);
    
    std::cin.get(); // Wait for user input before closing
    return 0;
}
