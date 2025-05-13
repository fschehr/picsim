#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>
#include "logger.h"

class FileReader {
public:
    std::vector<std::string> read(const std::string& filePath);
};

#endif // FILEREADER_H
