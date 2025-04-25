#ifndef BIT_H
#define BIT_H

#include "../instruction.h"
#include "../memory/ram.h"

class InstructionExecution;

class BitExecution {
private:
    InstructionExecution& executor;

public:
    explicit BitExecution(InstructionExecution& executor);

    void executeBCF(const Instruction& instruction);
    void executeBSF(const Instruction& instruction);
    void executeBTFSC(const Instruction& instruction);
    void executeBTFSS(const Instruction& instruction);
};

#endif // BIT_H
