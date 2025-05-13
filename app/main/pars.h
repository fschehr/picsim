#ifndef PARS_H
#define PARS_H

#include <string>
#include <vector>
#include "logger.h"

class Parser {
public:
    short* parse(const std::string& filename, int& size);
};

#endif // PARS_H
