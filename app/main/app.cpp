#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "pars.h"
#include "simvm.h"

int main(int argc, char* argv[]) {
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
        return 1;
    }

    return 0;
}
