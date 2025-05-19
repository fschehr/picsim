#include "utils.hpp"
#include <sstream>
#include <iomanip>
#include <string>

void updateHexValue(bool registerBits[8], std::string &registerHex) {
    unsigned int portValue = 0;
    for (int i = 0; i < 8; i++) {
        if (registerBits[i]) {
            portValue |= (1 << i);
        }
    }
    std::stringstream ss;
    ss << std::hex << std::uppercase << portValue;
    registerHex = ss.str();

    if (registerHex.length() == 1) {
        registerHex = "0" + registerHex;
    }
}