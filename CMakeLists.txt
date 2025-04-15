cmake_minimum_required(VERSION 3.10)

# Project name
project(picsim)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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

# Add executable
add_executable(simulator ${SOURCES})

# Include directories
target_include_directories(simulator PRIVATE app/src/main)

# Enable warnings
if (MSVC)
    target_compile_options(simulator PRIVATE /W4 /WX)
else()
    target_compile_options(simulator PRIVATE -Wall -Wextra -Werror)
endif()
