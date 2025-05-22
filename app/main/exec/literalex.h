#ifndef LITERALEX_H
#define LITERALEX_H

#include "../instruction.h"
#include "../logger.h"

class InstructionExecution;

class LiteralExecution {
private:
    InstructionExecution& executor;

public:
    LiteralExecution(InstructionExecution& executor, const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines, const std::vector<std::pair<short,short>>& prog);

    void executeADDLW(const Instruction& instruction);
    void executeSUBLW(const Instruction& instruction);
    void executeANDLW(const Instruction& instruction);
    void executeMOVLW(const Instruction& instruction);
    void executeIORLW(const Instruction& instruction);
    void executeXORLW(const Instruction& instruction);
    void executeRETLW(const Instruction& instruction);

    const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines;
    const std::vector<std::pair<short,short>>& prog;
};

#endif // LITERALEX_H
