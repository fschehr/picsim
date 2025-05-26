#include "instructionex.h"
#include "../instruction.h"
#include "../memory/ram.h"
#include "../logger.h"
#include <cstdint>

    BitExecution::BitExecution(InstructionExecution& executor, const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines, const std::vector<std::pair<short,short>>& prog) : executor(executor), fileLines(fileLines), prog(prog) {}

    void BitExecution::executeBCF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = instruction.getArguments()[0];  // File register address comes first
        int bit = instruction.getArguments()[1];      // Bit position comes second
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        value &= ~(1 << bit);  // Just clear the bit, nothing fancy
        
        executor.setRamContent(bank, address, value);
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
        uint8_t mask = static_cast<uint8_t>(0x01 << instruction.getArguments()[1]);        
        value = static_cast<uint8_t>(mask | value); // Set bit using the mask
        Logger::info("value: " + std::to_string(value));
        executor.setRamContent(bank, address, value);
    }

    /**
     * Test if given bit in file register is clear, if yes skip next instruction otherwise
     * execute it.
     *
     * @param instruction Instruction consisting of OPC and arguments.
     */    void BitExecution::executeBTFSC(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = instruction.getArguments()[0];  // File register address comes first
        int bit = instruction.getArguments()[1];      // Bit position comes second
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        Logger::info("BTFSC checking bit " + std::to_string(bit) + " of value " + std::to_string(value));
        
        if ((value & (1 << bit)) == 0) {  // If bit is clear
            executor.setProgramCounter(executor.getProgramCounter() + 1);  // Skip next instruction
            Logger::info("BTFSC: Bit is clear, skipping next instruction");
        } else {
            Logger::info("BTFSC: Bit is set, continuing");
        }
    }

    /**
     * Test if given bit in file register is set, if yes skip next instruction otherwise
     * execute it.
     *
     * @param instruction Instruction consisting of OPC and arguments.
     */    void BitExecution::executeBTFSS(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = instruction.getArguments()[0];  // file register address
        int bit = instruction.getArguments()[1];      // bit position
        auto bank = executor.getSelectedBank(instruction);
        
        uint8_t value = executor.getRamContent(bank, address);
        
        // Log the value we're checking
        Logger::info("BTFSS checking bit " + std::to_string(bit) + " of value " + std::to_string(value));
        
        if ((value & (1 << bit)) != 0) {  // if bit is set
            // Skip next instruction
            executor.setProgramCounter(executor.getProgramCounter() + 1);
            Logger::info("BTFSS: Bit is set, skipping next instruction");
        } else {
            Logger::info("BTFSS: Bit is not set, continuing");
        }
    }
