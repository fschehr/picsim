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

ConcreteInstruction::ConcreteInstruction(Instruction::OperationCode opc, const std::string& arguments)
    : Instruction(opc), arguments(arguments) {}

ConcreteInstruction::ConcreteInstruction(Instruction inst)
    : Instruction(inst.getOpc()), arguments(inst.getArgumentsAsString()) {}

Instruction::OperationCode ConcreteInstruction::getConcOpc() {
    return this->getOpc();
}

std::string ConcreteInstruction::getConcArguments() {
    return arguments;
}

PicSimulatorVM::PicSimulatorVM()
    : ram(ramBankSize), program(programMemorySize), stack(stackSize), eeprom(eepromSize), executor(program, ram, stack, eeprom) {
    programMemory.resize(programMemorySize, nullptr);
}

PicSimulatorVM::~PicSimulatorVM() {
    for (Instruction* instruction : programMemory) {
        delete instruction;
    }
}

void PicSimulatorVM::initialize(const std::vector<short>& prog) {
    programDecode(prog);
    load(prog);
    while (running) {
        execute();
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
