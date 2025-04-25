#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>

class Logger {
public:
    static void info(const std::string& message);
    static void warning(const std::string& message);
};

class FileReader {
public:
    std::vector<std::string> read(const std::string& filePath);
};

#endif // FILEREADER_H
