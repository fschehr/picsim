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
    microseconds(500000),
    programMemory(programMemorySize, nullptr)
{
    // Logger auf Dateiausgabe umstellen
    Logger::setOutputToFile();
    Logger::info("Simulator constructed");
    // programMemory.resize(programMemorySize, nullptr); // Already initialized in initializer list
    
    // Setze den Callback für Zyklusaktualisierungen
    executor.setCycleUpdateCallback([this](int cycles) {
        this->updateCyclesCounter(cycles);
    });
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
    programDecode();
    load();
    // execute(); //is called in ui
}

void PicSimulatorVM::programDecode() {
    if (prog.size() > programMemory.size()) {
        throw std::runtime_error("Program size exceeds program memory capacity");
    }
    for (size_t i = 0; i < prog.size(); i++) {
        Instruction* instruction = new ConcreteInstruction(decoder.decode(prog[i].second));
        programMemory[i] = instruction;
    }
    //following is simply for logging
    for (size_t i = 0; i < programMemory.size(); i++) {
        if (programMemory[i] != nullptr) {
            std::stringstream ss;
            ss << programMemory[i]->getOpc();
            Logger::info("Instruction " + ss.str() + 
                        " with arguments " + programMemory[i]->getArgumentsAsString());
        }
    }
}

void PicSimulatorVM::stop() {
    running = false;
    
    // Warten bis ein laufender Thread beendet ist, falls vorhanden und joinable
    if (executionThread.joinable()) {
        executionThread.join();
    }
}

void PicSimulatorVM::start() {
    running = true;
    
}
bool PicSimulatorVM::getRunning(){
    return running;
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
    stop();

    for (size_t address = 0; address < prog.size(); address++) {
        program.set(address, prog[address].second);
    }
    loaded = true;
    start();
    executor.reset();
    
    // Markiere die erste auszuführende Zeile direkt nach dem Laden des Programms
    if (prog.size() > 0) {
        const_cast<std::pair<bool,bool*>&>(fileLines[prog[0].first].first).first = true;
        executor.prevProgCounter = 0;
    }
}

void PicSimulatorVM::execute() {
    if (!loaded) {
        throw std::runtime_error("No executable program loaded");
    }
    
    // Wenn bereits ein Thread läuft, nichts tun
    if (executionThread.joinable()) {
        return;
    }
    if(halted){
        halted = false;
    }
    if (!running) {
        running = true;
        executor.reset();
        const_cast<std::pair<bool,bool*>&>(fileLines[prog[0].first].first).first = false;
    }
    
    // Thread starten, der kontinuierlich Instruktionen ausführt
    executionThread = std::thread([this]() {
        try {
            while (running && loaded) {
                // Führe eine Instruktion aus
                if((*fileLines[prog[executor.programCounter].first+1].first.second)==true){
                    halt();
                }
                executor.execute();
                
                // Kleine Pause für CPU-Entlastung und um UI-Thread Zeit zu geben
                std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
                // Überprüfe, ob der Thread angehalten werden soll
                std::unique_lock<std::mutex> lock(mtx);
                while (halted) {
                    cv.wait(lock);
                }
            }
        } catch (const std::exception& e) {
            Logger::warning(std::string("Fehler bei der Ausführung: ") + e.what());
            running = false; // Bei Fehler die Ausführung stoppen
        }
    });
    
    // Thread im Hintergrund laufen lassen
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