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
#include "condition_variable"
#include <atomic>

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
    PicSimulatorVM(const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLine, const std::vector<std::pair<short,short>>& prog);
    ~PicSimulatorVM();
    void initialize();
    void stop();
    void load();
    void execute();
    void executeStep(); // Führt einen einzelnen Schritt aus
    void programDecode();
    void start();
    void reset();
    void updateCyclesCounter(int cycles);
    void updateRuntimeCounter();
    void halt();
    void resume();
    bool getRunning();
    std::condition_variable cv;
    std::mutex mtx;
    bool halted = false;
    double microseconds = 100000.0;
    short cycles;
    double runtime;
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
    const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines;
    const std::vector<std::pair<short,short>>& prog;
    bool loaded = false;
    bool running = false;
    std::thread executionThread;
    std::atomic<bool> threadRunning{false};  // Add this line
};

#endif // SIMVM_H
