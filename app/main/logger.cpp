#include "logger.h"
#include <iostream>

void Logger::info(const std::string& message) {
    std::cout << "INFO: " << message << std::endl;
}

void Logger::warning(const std::string& message) {
    std::cout << "WARNING: " << message << std::endl;
}
