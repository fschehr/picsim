#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

class Logger {
public:
    static void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }

    static void warning(const std::string& message) {
        std::cout << "WARNING: " << message << std::endl;
    }
};

class FileReader {
public:
    std::vector<std::string> read(const std::string& filePath) {
        try {
            Logger::info("Reading file");
            std::string resourcePath = "../testprogs" + filePath;
            std::ifstream file(resourcePath);
            if (!file.is_open()) {
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
                std::cout << line << std::endl;
            }

            file.close();
            Logger::info("Complete");
            return lines;
        } catch (const std::exception& e) {
            Logger::warning("There has been an error reading the file: " + std::string(e.what()));
            return {};
        }
    }
};
