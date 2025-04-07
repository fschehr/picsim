#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

//Logger class implementation
class Logger {
public:
    static void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }
};

class Parser {
public:
    std::vector<short> parse(const std::string& filename) {
        std::vector<short> lines;
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file");
        }

        short line;
        while (file >> line) {
            lines.push_back(line);
        }

        file.close();
        return lines;
    }
};

class PicSimulatorVM {
public:
    void initialize(const std::vector<short>& lines) {
        //....
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
        fileName = "TPicSim1.LST";
    }

    // Assuming the file is in the same directory as the executable
    std::string filePath = "../testprog" + fileName;

    try {
        Parser parser;
        std::vector<short> lines = parser.parse(filePath);
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
