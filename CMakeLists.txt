cmake_minimum_required(VERSION 3.11)

# Project name
project(picsim)

# Set C++ standard
set(CMAKE_CXX_STANDARD 22)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add ftxui library as a subproject
add_subdirectory(external/ftxui)

# Add source files
set(SOURCES
    app/main/app.cpp
    app/main/simvm.cpp
    app/main/decode.cpp
    app/main/pars.cpp
    app/main/fileReader.cpp
    app/main/instruction.cpp
    app/main/memory/eeprom.cpp
    app/main/memory/program.cpp
    app/main/memory/ram.cpp
    app/main/memory/stack.cpp
    app/main/memory/watcher.cpp
    app/main/exec/bit.cpp
    app/main/exec/byte.cpp
    app/main/exec/exwatcher.cpp
    app/main/exec/instructionex.cpp
    app/main/exec/jumpex.cpp
    app/main/exec/literalex.cpp
)

add_executable(picsim ${SOURCES})

# Add needed ftxui library stuff
target_link_libraries(picsim
    PRIVATE ftxui::screen # screen buffer, where elements (UI) are drawn on
    PRIVATE ftxui::dom # defines Elements that are drawn on the screen
    PRIVATE ftxui::component # components allowing user interaction
)
