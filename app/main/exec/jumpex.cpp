#include "jumpex.h"
#include "instructionex.h"
#include "../instruction.h"
#include "../memory/ram.h"
#include "../logger.h"
#include <cstdint>

    JumpExecution::JumpExecution(InstructionExecution& executor, const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines, const std::vector<std::pair<short,short>>& prog) : executor(executor), fileLines(fileLines), prog(prog) {}

    void JumpExecution::executeCALL(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        // Save address of next instruction to stack memory
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
        executor.pushStack(executor.getprevProgCounter()+1);

        // Consists of the opcode/address given as argument and the upper bits (bit 3 + 4) of PCLATH register
        int pclathBits = (executor.getRamContent(RamMemory<uint8_t>::SFR::entries()[9].address) & 0b00011000) << 8;

        int address = instruction.getArguments()[0]; // Load jump address
        address = address & 2047;                   // Clear upper two bits
        address = address | pclathBits;             // Adding PCLATH
        
        executor.setProgramCounter(address);
    }

    /**
     * Makes a jump to the given address inside of program memory.
     *
     * @param instruction Instruction consisting of OPC and arguments
     */
    void JumpExecution::executeGOTO(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
        int pclathBits = (executor.getRamContent(RamMemory<uint8_t>::SFR::entries()[9].address) & 0b00011000) << 8;

        int address = instruction.getArguments()[0]; // Load jump address
        address = address & 0b001111111111;          // Clear upper two bits
        address = address | pclathBits;             // Adding PCLATH
        
        executor.setProgramCounter(address);
        executor.setRamContent(RamMemory<uint8_t>::SFR::entries()[2], (address & 0xFF)); 
    }