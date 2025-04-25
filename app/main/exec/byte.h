#ifndef BYTE_H
#define BYTE_H

#include "../instruction.h"
#include "../memory/ram.h"

class InstructionExecution;

class ByteExecution {
private:
    InstructionExecution& executor;

public:
    explicit ByteExecution(InstructionExecution& executor);

    void executeADDWF(const Instruction& instruction);
    void executeANDWF(const Instruction& instruction);
    void executeXORWF(const Instruction& instruction);
    void executeSUBWF(const Instruction& instruction);
    void executeCLRW();
    void executeRETURN();
    void executeMOVWF(const Instruction& instruction);
    void executeCLRF(const Instruction& instruction);
    void executeCOMF(const Instruction& instruction);
    void executeDECF(const Instruction& instruction);
    void executeINCF(const Instruction& instruction);
    void executeMOVF(const Instruction& instruction);
    void executeIORWF(const Instruction& instruction);
    void executeDECFSZ(const Instruction& instruction);
    void executeINCFSZ(const Instruction& instruction);
    void executeRLF(const Instruction& instruction);
    void executeRRF(const Instruction& instruction);
    void executeNOP();
    void executeSWAPF(const Instruction& instruction);
    void executeRETFIE(const Instruction& instruction);
};

#endif // BYTE_H
