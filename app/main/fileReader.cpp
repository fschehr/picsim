#include "fileReader.h"
#include "logger.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

std::vector<std::string> FileReader::read(const std::string& filePath) {
    try {
        Logger::info("Reading file, removing comments ("+filePath+")");
        std::ifstream file(filePath);
        if (!file.is_open()) {
            Logger::warning("File cannot be opened: " + filePath);
            throw std::runtime_error("Datei kann nicht geffnet werden");
        }

        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            size_t semicolonPos = line.find(';');
            if (semicolonPos != std::string::npos) {
                line = line.substr(0, semicolonPos);
            }
            lines.push_back(line);
            Logger::info("Read line: " + line);
        }

        file.close();
        Logger::info("File reading Complete; removed Comments");
        return lines;
    } catch (const std::exception& e) {
        Logger::warning("There has been an error reading the file: " + std::string(e.what()));
        return {};
    }
}
