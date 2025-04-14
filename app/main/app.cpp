#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include "pars.cpp"
#include "simvm.cpp"

class Logger {
public:
    static void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }
};


class App {
public:
    std::string getGreeting() {
        return "Hello World!";
    }
};

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

        Logger::info(App().getGreeting());

        PicSimulatorVM vm;
        vm.initialize(lines);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
