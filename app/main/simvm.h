#ifndef SIMVM_H
#define SIMVM_H

#include <vector>
#include <string>
#include "instruction.h"
#include "memory/program.h"
#include "memory/stack.h"
#include "memory/eeprom.h"
#include "decode.h"
#include "exec/instructionex.h"

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
    int execute();
    void programDecode(const std::vector<short>& prog);
    void start();

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
    InstructionExecution executor;

    bool running = false;
    bool loaded = false;
};

#endif // SIMVM_H
