#include "instructionex.h"
#include "../instruction.h"
#include "../memory/ram.h"
#include "../logger.h"
#include <cstdint>

    BitExecution::BitExecution(InstructionExecution& executor, const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines, const std::vector<std::pair<short,short>>& prog) : executor(executor), fileLines(fileLines), prog(prog) {}

    void BitExecution::executeBCF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        RamMemory<uint8_t>::Bank bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address); // Fetch value from given file register
        uint8_t mask = static_cast<uint8_t>(0x01 << instruction.getArguments()[0]);        
        value = static_cast<uint8_t>(value & ~mask); // Clear bit using the mask

        executor.setRamContent(bank, address, value);
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    /**
     * Sets the selected bit inside of a file register.
     *
     * @param instruction Instruction consisting of OPC and arguments.
     */
    void BitExecution::executeBSF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address); // Fetch value from given file register
        uint8_t mask = static_cast<uint8_t>(0x01 << instruction.getArguments()[0]);        
        value = static_cast<uint8_t>(value | mask); // Set bit using the mask

        executor.setRamContent(bank, address, value);
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    /**
     * Test if given bit in file register is clear, if yes skip next instruction otherwise
     * execute it.
     *
     * @param instruction Instruction consisting of OPC and arguments.
     */
    void BitExecution::executeBTFSC(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address); // Fetch value from given file register        
        uint8_t bit = static_cast<uint8_t>(0x01 << instruction.getArguments()[0]);
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
        if ((value & bit) == 0) { // Check if bit is clear
            // Skip the next operation by incrementing the program counter
            executor.setProgramCounter(executor.getProgramCounter() + 1);
        }
    }

    /**
     * Test if given bit in file register is set, if yes skip next instruction otherwise
     * execute it.
     *
     * @param instruction Instruction consisting of OPC and arguments.
     */
    void BitExecution::executeBTFSS(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);        
        uint8_t value = executor.getRamContent(bank, address); // Fetch value from given file register
        uint8_t bit = static_cast<uint8_t>(0x01 << instruction.getArguments()[0]);

        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
        if ((value & bit) != 0) { // Check if bit is set
            // Skip the next operation by incrementing the program counter
            executor.setProgramCounter(executor.getProgramCounter() + 1);
        }
    }
