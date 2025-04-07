#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>

class Parser {
public:
    short* parse(const std::string& filename, int& size) {
        try {
            std::ifstream file(filename);
            if (!file.is_open()) {
                throw std::runtime_error("Failed to open file");
            }

            std::vector<std::string> converter;
            std::string line;
            while (std::getline(file, line)) {
                if (line[0] != ' ') {
                    converter.push_back(line.substr(5, 4));
                }
            }

            file.close();
            size = converter.size();
            short* output = new short[size];
            for (int i = 0; i < size; i++) {
                std::istringstream iss("0x" + converter[i]);
                iss >> std::hex >> output[i];
            }

            return output;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return nullptr;
        }
    }
};
