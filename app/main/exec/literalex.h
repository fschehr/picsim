#ifndef LITERALEX_H
#define LITERALEX_H

#include "../instruction.h"

class InstructionExecution;

class LiteralExecution {
private:
    InstructionExecution& executor;

public:
    explicit LiteralExecution(InstructionExecution& executor);

    void executeADDLW(const Instruction& instruction);
    void executeSUBLW(const Instruction& instruction);
    void executeANDLW(const Instruction& instruction);
    void executeMOVLW(const Instruction& instruction);
    void executeIORLW(const Instruction& instruction);
    void executeXORLW(const Instruction& instruction);
    void executeRETLW(const Instruction& instruction);
};

#endif // LITERALEX_H
