#include "pars.h"
#include "fileReader.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

short* Parser::parse(const std::string& filename, int& size) {
    Logger::info("Parsing file: " + filename);
    try {
        FileReader fileReader;
        std::vector<std::string> lines = fileReader.read(filename);
        if (lines.empty()) {
            throw std::runtime_error("File is empty or could not be read");
        }

        std::vector<std::string> converter;
        for (const auto& line : lines) {
            if (!line.empty() && line[0] != ' ') {
                converter.push_back(line.substr(5, 4));
            }
        }

        size = converter.size();
        short* output = new short[size];
        for (int i = 0; i < size; i++) {
            std::istringstream iss("0x" + converter[i]);
            iss >> std::hex >> output[i];
        }
        Logger::info("Parsing complete, size: " + std::to_string(size));
        return output;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return nullptr;
    }
}

