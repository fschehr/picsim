#include "controls.cpp"
#include "editor.cpp"
#include "registerTable.cpp"
#include "settings.cpp"
#include "stats.cpp"
#include "logs.cpp"
#include "resets.cpp"
#include "runtime.cpp"
#include "ledArray.cpp"
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <array>
#include "../simvm.h"

ftxui::Component Document(const std::string &filePath, 
                            std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>> &fileLines, 
                            PicSimulatorVM &vm);

