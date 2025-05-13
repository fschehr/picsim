#ifndef JUMPEX_H
#define JUMPEX_H

#include "../instruction.h"
#include "../logger.h"

class InstructionExecution;

class JumpExecution {
private:
    InstructionExecution& executor;

public:
    JumpExecution(InstructionExecution& executor);

    void executeCALL(const Instruction& instruction);
    void executeGOTO(const Instruction& instruction);
};

#endif // JUMPEX_H
