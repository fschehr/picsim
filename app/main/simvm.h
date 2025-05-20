#ifndef SIMVM_H
#define SIMVM_H

#include <vector>
#include <string>
#include <thread>
#include <functional>
#include "instruction.h"
#include "memory/program.h"
#include "memory/stack.h"
#include "memory/eeprom.h"
#include "decode.h"
#include "exec/instructionex.h"
#include "logger.h"

class ConcreteInstruction : public Instruction {
public:
    ConcreteInstruction(Instruction::OperationCode opc, const std::string& arguments);
    ConcreteInstruction(Instruction inst);
    Instruction::OperationCode getConcOpc();
    std::string getConcArguments();

private:
    std::string arguments;
};

class PicSimulatorVM {
public:
    PicSimulatorVM();
    ~PicSimulatorVM();
    void initialize(const std::vector<short>& prog);
    void stop();
    void load(const std::vector<short>& file);
    void execute();
    void executeStep(); // Führt einen einzelnen Schritt aus
    void programDecode(const std::vector<short>& prog);
    void start();
    void reset(); // Neue Methode zum Zurücksetzen der VM
    void updateCyclesCounter(int cycles);
    void updateRuntimeCounter();
    short microseconds = 100;
    short cycles;
    int runtime;
    InstructionExecution executor;
    int currentLine = 0;

    // Add getter for stack
    StackMemory<int>& getStack() { return stack; }

private:
    int ramBankSize = 128;
    int programMemorySize = 1024;
    int stackSize = 8;
    int eepromSize = 64;

    RamMemory<uint8_t> ram;
    ProgramMemory<uint16_t> program;
    std::vector<Instruction*> programMemory;
    StackMemory<int> stack;
    EepromMemory<uint8_t> eeprom;
    Decoder decoder;

    bool running = false;
    bool loaded = false;
    std::thread executionThread;
};

#endif // SIMVM_H
