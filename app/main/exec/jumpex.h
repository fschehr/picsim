#ifndef JUMPEX_H
#define JUMPEX_H

#include "../instruction.h"
#include "../logger.h"

class InstructionExecution;

class JumpExecution {
private:
    InstructionExecution& executor;

public:
    JumpExecution(InstructionExecution& executor, const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines, const std::vector<std::pair<short,short>>& prog);

    void executeCALL(const Instruction& instruction);
    void executeGOTO(const Instruction& instruction);

    const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines;
    const std::vector<std::pair<short,short>>& prog;
};

#endif // JUMPEX_H
