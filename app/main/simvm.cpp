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

PicSimulatorVM::PicSimulatorVM()
    : ram(ramBankSize), program(programMemorySize), stack(stackSize), eeprom(eepromSize), executor(program, ram, stack, eeprom) {
    Logger::info("Simulator constructed");
    programMemory.resize(programMemorySize, nullptr);
}

PicSimulatorVM::~PicSimulatorVM() {
    Logger::info("Simulator destructed");
    for (Instruction* instruction : programMemory) {
        delete instruction;
    }
}

void PicSimulatorVM::initialize(const std::vector<short>& prog) {
    programDecode(prog);
    load(prog);
    while (running) {
        execute();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // sleep for 500ms
    }
}

void PicSimulatorVM::programDecode(const std::vector<short>& prog) {
    if (prog.size() > programMemory.size()) {
        throw std::runtime_error("Program size exceeds program memory capacity");
    }
    for (size_t i = 0; i < prog.size(); i++) {
        Instruction* instruction = new ConcreteInstruction(decoder.decode(prog[i]));
        programMemory[i] = instruction;
    }
    for (size_t i = 0; i < programMemory.size(); i++) {
        if (programMemory[i] != nullptr) {
            std::cout << "Instruction " << programMemory[i]->getOpc() << " with arguments " << programMemory[i]->getArgumentsAsString() << std::endl;
        }
    }
}

void PicSimulatorVM::stop() {
    running = false;
}

void PicSimulatorVM::start() {
    running = true;
}

void PicSimulatorVM::load(const std::vector<short>& file) {
    stop();

    for (size_t address = 0; address < file.size(); address++) {
        program.set(address, file[address]);
    }
    loaded = true;
    start();
    executor.reset();
}

int PicSimulatorVM::execute() {
    if (!loaded) {
        throw std::runtime_error("No executable program loaded");
    } else {
        if (!running) {
            running = true;
            executor.reset();
        }
        return executor.execute();
    }
}
