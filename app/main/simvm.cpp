#include "simvm.h"
#include "memory/ram.h"
#include "memory/stack.h"
#include "memory/eeprom.h"
#include "memory/program.h"
#include "decode.h"
#include "exec/instructionex.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>
#include <sstream>
#include <functional>

template class RamMemory<uint8_t>;
template class ProgramMemory<uint16_t>;
template class StackMemory<int>;
template class EepromMemory<uint8_t>;

ConcreteInstruction::ConcreteInstruction(Instruction::OperationCode opc, const std::string& arguments)
    : Instruction(opc, {}), arguments(arguments) {}

ConcreteInstruction::ConcreteInstruction(Instruction inst)
    : Instruction(inst.getOpc(), inst.getArguments()), arguments(inst.getArgumentsAsString()) {}

Instruction::OperationCode ConcreteInstruction::getConcOpc() {
    return this->getOpc();
}

std::string ConcreteInstruction::getConcArguments() {
    return arguments;
}

PicSimulatorVM::PicSimulatorVM(const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines, const std::vector<std::pair<short,short>>& prog)
    : ram(ramBankSize),
    program(programMemorySize),
    stack(stackSize),
    eeprom(eepromSize),
    fileLines(fileLines),
    prog(prog),
    executor(program, ram, stack, eeprom, fileLines, prog),
    running(false),
    loaded(false),
    cycles(0),
    runtime(0),
    microseconds(100000),
    programMemory(programMemorySize, nullptr)
{
    try {
        Logger::info("Starting PicSimulatorVM constructor");
        
        if (fileLines.empty()) {
            Logger::error("Empty file lines provided");
            throw std::runtime_error("Empty file lines provided");
        }
        
        if (prog.empty()) {
            Logger::error("Empty program provided");
            throw std::runtime_error("Empty program provided");
        }
        
        Logger::info("VM components initialized");
        executor.setCycleUpdateCallback([this](int cycles) {
            this->updateCyclesCounter(cycles);
        });
        Logger::info("Simulator constructed successfully");
    } catch (const std::exception& e) {
        Logger::error("Failed to construct PicSimulatorVM: " + std::string(e.what()));
        throw;
    }
}

PicSimulatorVM::~PicSimulatorVM() {
    Logger::info("Simulator destructed");
    // Stellen Sie sicher, dass alle Thread-Aktivitäten beendet sind
    stop();
    
    for (Instruction* instruction : programMemory) {
        delete instruction;
    }
}

void PicSimulatorVM::initialize() {
    try {
        Logger::info("Starting program decode");
        programDecode();
        Logger::info("Program decode completed");
        
        Logger::info("Starting program load");
        load();
        Logger::info("Program load completed");
        
        Logger::info("Initialization complete");
    } catch (const std::exception& e) {
        Logger::error("Initialization failed: " + std::string(e.what()));
        throw;
    }
}

void PicSimulatorVM::programDecode() {
    Logger::info("Program decode: checking size");
    if (prog.size() > programMemory.size()) {
        throw std::runtime_error("Program size exceeds program memory capacity");
    }
    
    Logger::info("Program decode: decoding instructions");
    for (size_t i = 0; i < prog.size(); i++) {
        //Logger::info("Decoding instruction at index " + std::to_string(i));
        Instruction* instruction = new ConcreteInstruction(decoder.decode(prog[i].second));
        programMemory[i] = instruction;
    }
    //following is simply for logging
    for (size_t i = 0; i < programMemory.size(); i++) {
        if (programMemory[i] != nullptr) {
            std::stringstream ss;
            ss << programMemory[i]->getOpc();
            //Logger::info("Instruction " + ss.str() + " with arguments " + programMemory[i]->getArgumentsAsString());
        }
    }
}

void PicSimulatorVM::stop() {
    Logger::info("Stopping VM");
    running = false;
    halted = false;
    cv.notify_all();  // Wake up any waiting threads
    
    if (executionThread.joinable()) {
        Logger::info("Waiting for execution thread to join");
        executionThread.join();
    }
    threadRunning = false;
    Logger::info("VM stopped successfully");
}

void PicSimulatorVM::start() {
    Logger::info("Starting VM");
    running = true;
}

void PicSimulatorVM::reset() {
    const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.prevProgCounter].first].first).first = false;
    const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.programCounter].first].first).first = false;
    Logger::info("Resetting simulator");
    stop(); // Das beendet den Thread und setzt running auf false
    executor.reset();
    stack.clear(); // Leert den Stack
    //ram.clear();  TODO
    const_cast<std::pair<bool,bool*>&>(fileLines[prog[0].first].first).first = true;
    // start(); // Setzt running wieder auf true
}

// Neue Methode für die Ausführung eines einzelnen Schritts
void PicSimulatorVM::executeStep() {
    Logger::info("Executing single step");
    if (!loaded) {
        throw std::runtime_error("No executable program loaded");
    }
    
    if (!running) {
        running = true;
        executor.reset();
        const_cast<std::pair<bool,bool*>&>(fileLines[prog[0].first].first).first = false;
    }
    
    // Nur einen einzigen Befehl ausführen
    try {
        executor.execute();
    } catch (const std::exception& e) {
        Logger::warning(std::string("Fehler bei der Ausführung eines Schritts: ") + e.what());
    }
}

void PicSimulatorVM::load() {
    Logger::info("Load: stopping previous execution");
    stop();

    Logger::info("Load: loading program into memory");
    for (size_t address = 0; address < prog.size(); address++) {
        program.set(address, prog[address].second);
    }
    Logger::info("Load: program loaded into memory");
    loaded = true;
    Logger::info("Load: starting program");
    start();
    Logger::info("Load: resetting executor");
    executor.reset();
    
    if (prog.size() > 0) {
        Logger::info("Load: marking first line");
        const_cast<std::pair<bool,bool*>&>(fileLines[prog[0].first].first).first = true;
        executor.prevProgCounter = 0;
    }
    Logger::info("Load: completed successfully");
}

void PicSimulatorVM::execute() {
    if (!loaded) {
        throw std::runtime_error("No executable program loaded");
    }
    
    // If already running, just resume if halted
    if (threadRunning) {
        if (halted) {
            resume();
        }
        return;
    }

    if (!running) {
        running = true;
        executor.reset();
        const_cast<std::pair<bool,bool*>&>(fileLines[prog[0].first].first).first = false;
    }
    
    threadRunning = true;
    executionThread = std::thread([this]() {
        try {
            while (running && loaded) {
                if ((*fileLines[prog[executor.programCounter].first].first.second)) {
                    halt();
                }
                executor.execute();
                std::this_thread::sleep_for(std::chrono::microseconds(static_cast<int>(microseconds)));
                
                std::unique_lock<std::mutex> lock(mtx);
                while (halted) {
                    cv.wait(lock);
                }
            }
        } catch (const std::exception& e) {
            Logger::warning(std::string("Fehler bei der Ausführung: ") + e.what());
            running = false;
        }
        threadRunning = false;
    });
    
    executionThread.detach();
}

void PicSimulatorVM::halt() {
    std::lock_guard<std::mutex> lock(mtx);
    halted = true;
}

void PicSimulatorVM::resume() {
    std::lock_guard<std::mutex> lock(mtx);
    halted = false;
    cv.notify_one();
}
void PicSimulatorVM::updateCyclesCounter(int i) {
    cycles += i;
    //Logger::info("Runtime counter incremented by " + std::to_string(increment) + ", new value: " + std::to_string(runtimeCounter));
    updateRuntimeCounter();
}
void PicSimulatorVM::updateRuntimeCounter() {
    runtime = cycles * microseconds; 
}

bool PicSimulatorVM::getRunning() {
    return running;
}