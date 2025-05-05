#ifndef INSTRUCTIONEX_H
#define INSTRUCTIONEX_H

#include "../memory/program.h"
#include "../memory/ram.h"
#include "../memory/stack.h"
#include "../memory/eeprom.h"
#include "../instruction.h"
#include "../decode.h"
#include <mutex>

class LiteralExecution;
class JumpExecution;
class ByteExecution;
class BitExecution;

class InstructionExecution {
private:
    uint8_t workingRegister;
    uint16_t instructionRegister;
    int programCounter;
    double runtimeCounter;
    double frequency;

    ProgramMemory<uint16_t>& programMemory;
    RamMemory<uint8_t>& ram;
    StackMemory<int>& stack;
    EepromMemory<uint8_t>& eeprom;

    LiteralExecution& literalExecutionUnit;
    JumpExecution& jumpExecutionUnit;
    ByteExecution& byteAndControlExecutionUnit;
    BitExecution& bitExecutionUnit;

    std::mutex lock;
    Decoder decoder;

    void updateRuntimeCounter(int cycles);
    void updateTimer();
    bool checkTMR0Interrupt();
    bool checkRB0Interrupt();
    bool checkRBInterrupts();
    void callISR(int address);

public:
    InstructionExecution(ProgramMemory<uint16_t>& programMemory, RamMemory<uint8_t>& ram,
                         StackMemory<int>& stack, EepromMemory<uint8_t>& eeprom);

    void init();
    int execute();
    void reset();

    void setInstructionRegister(uint16_t value);
    void setProgramCounter(int value);
    void setWorkingRegister(uint8_t value);
    void setRuntimeCounter(double value);

    void setZeroFlag();

    void setRamContent(int address, uint8_t value);
    void setRamContent(RamMemory<uint8_t>::SFR sfr, uint8_t value);
    void setRamContent(RamMemory<uint8_t>::Bank bank, int address, uint8_t value);

    void pushStack(int value);
    int popStack();

    int getWorkingRegister();
    bool checkZeroFlag(int value);
    bool checkCarryFlag(int value);
    bool checkDigitCarryFlag(int value);
    int getProgramCounter();
    StackMemory<int>& getStack();
    uint8_t getRamContent(int address);
    uint8_t getRamContent(RamMemory<uint8_t>::SFR sfr) const;
    uint8_t getRamContent(RamMemory<uint8_t>::Bank bank, int address) const;
    int getFileAddress(const Instruction& instruction) const;
    RamMemory<uint8_t>::Bank getSelectedBank(const Instruction& instruction) const;
    bool isCarryFlag() const;
};

#endif // INSTRUCTIONEX_H
