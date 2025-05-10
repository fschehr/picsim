#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "pars.h"
#include "simvm.h"
//#include <ftxui/dom/elements.hpp>
//#include <ftxui/screen/screen.hpp>

int main(int argc, char* argv[]) {
    //using namespace ftxui;

    std::string fileName;
    if (argc > 1) {
        fileName = argv[1];
    } else {
        fileName = "TPicSim1.LST"; // Default file
    }

    std::string filePath = "../testprog/" + fileName; 

    try {
        Parser parser;
        int lineCount = 0;
        short* parsedLines = parser.parse(filePath, lineCount);
        std::vector<short> lines(parsedLines, parsedLines + lineCount);
        for (const auto& line : lines) {
            std::cout << line << std::endl;
        }

        PicSimulatorVM vm;
        vm.initialize(lines);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cin.get(); // Wait for user input before closing
        return 1;
    }

    // ui

    /*
    Element document = hbox({
        text("left")   | border,
        text("middle") | border | flex,
        text("right")  | border,
    });

    auto screen = Screen::Create(
        Dimension::Full(),       // Width
        Dimension::Fit(document) // Height
    );
    Render(screen, document);
    screen.Print();
    */

    
    std::cin.get(); // Wait for user input before closing
    return 0;
   
}
