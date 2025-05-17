#include <string>

/**
 * @brief Updates the hex value of the register array for the ui
 * 
 * @param registerBits Array of boolean values representing the state of the register bits
 * @param registerHex Reference to the string that will be updated with the hex value
 * 
 * @return void
 */
void updateHexValue(bool registerBits[8], std::string &registerHex);