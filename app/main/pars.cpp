#include "pars.h"
#include "fileReader.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

short* Parser::parseToShort(const std::string& filename, int& size) {
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
        Logger::warning(std::string("Parsing error: ") + e.what());
        return nullptr;
    }
}
std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>> Parser::parseToPair(const std::string& filename) {
    // vector of pairs of pairs. first pair is (bool (says if its relevant), bool*(empty for now, will be true if a breakpoint is added)), second pair is (short(line number), string(actual line content))
    Logger::info("Parsing file: " + filename);
    try {
        FileReader fileReader;
        std::vector<std::string> lines = fileReader.read(filename);
        if (lines.empty()) {
            throw std::runtime_error("File is empty or could not be read");
        }

        std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>> output;
        for (int i = 0; i < lines.size(); i++) {
            //if (!lines[i].empty() && lines[i][0] != ' ') {
                output.push_back({{true, nullptr}, {static_cast<short>(i), lines[i]}});
            //}
        }
        Logger::info("Parsing complete, size: " + std::to_string(output.size()));
        return output;
    } catch (const std::exception& e) {
        Logger::warning(std::string("Parsing error: ") + e.what());
        return {};
    }
}

