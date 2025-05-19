#ifndef PARS_H
#define PARS_H

#include <string>
#include <vector>
#include "logger.h"

class Parser {
public:
    short* parseToShort(const std::string& filename, int& size);
    std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>> parseToPair(const std::string& filename);
    // https://media.tenor.com/AT7MQToJVHoAAAAM/oh-vector.gif
};

#endif // PARS_H
