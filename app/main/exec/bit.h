#ifndef BIT_H
#define BIT_H

#include "../instruction.h"
#include "../memory/ram.h"
#include "../logger.h"

class InstructionExecution;

class BitExecution {
private:
    InstructionExecution& executor; // Reference to InstructionExecution

public:
    explicit BitExecution(InstructionExecution& executor, const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines, const std::vector<std::pair<short,short>>& prog);
    void executeBCF(const Instruction& instruction);
    void executeBSF(const Instruction& instruction);    
    void executeBTFSC(const Instruction& instruction);
    void executeBTFSS(const Instruction& instruction);

    const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines;
    const std::vector<std::pair<short,short>>& prog;
};

#endif // BIT_H
