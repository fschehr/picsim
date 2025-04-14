cmake_minimum_required(VERSION 3.10)

# Project name
project(picsim)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add source files
set(SOURCES
    app/src/main/app.cpp
    app/src/main/simvm.cpp
    app/src/main/decode.cpp
    app/src/main/pars.cpp
    app/src/main/fileReader.cpp
    app/src/main/instruction.cpp
    app/src/main/memory/eeprom.cpp
    app/src/main/memory/program.cpp
    app/src/main/memory/ram.cpp
    app/src/main/memory/stack.cpp
    app/src/main/memory/watcher.cpp
    app/src/main/exec/bit.cpp
    app/src/main/exec/byte.cpp
    app/src/main/exec/exwatcher.cpp
    app/src/main/exec/instructionex.cpp
    app/src/main/exec/jumpex.cpp
    app/src/main/exec/literalex.cpp
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
