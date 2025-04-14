#include "../instruction.cpp"
#include "../memory/ram.cpp"
#include "instructionex.cpp"
#include <cstdint>

class BitExecution {
private:
    InstructionExecution& executor;

public:
    explicit BitExecution(InstructionExecution& executor) : executor(executor) {}

    void executeBCF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address); // Fetch value from given file register
        uint8_t mask = static_cast<uint8_t>(0x01 << instruction.getArguments()[0]);

        value = static_cast<uint8_t>(value & ~mask); // Clear bit using the mask

        executor.ram.set(bank, address, value);
    }

    /**
     * Sets the selected bit inside of a file register.
     *
     * @param instruction Instruction consisting of OPC and arguments.
     */
    void executeBSF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address); // Fetch value from given file register
        uint8_t mask = static_cast<uint8_t>(0x01 << instruction.getArguments()[0]);

        value = static_cast<uint8_t>(value | mask); // Set bit using the mask

        executor.ram.set(bank, address, value);
    }

    /**
     * Test if given bit in file register is clear, if yes skip next instruction otherwise
     * execute it.
     *
     * @param instruction Instruction consisting of OPC and arguments.
     */
    void executeBTFSC(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address); // Fetch value from given file register
        uint8_t bit = static_cast<uint8_t>(0x01 << instruction.getArguments()[0]);

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
    void executeBTFSS(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address); // Fetch value from given file register
        uint8_t bit = static_cast<uint8_t>(0x01 << instruction.getArguments()[0]);

        if ((value & bit) != 0) { // Check if bit is set
            // Skip the next operation by incrementing the program counter
            executor.setProgramCounter(executor.getProgramCounter() + 1);
        }
    }
};