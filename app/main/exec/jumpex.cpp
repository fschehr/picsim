#include "jumpex.h"
#include "instructionex.h"
#include "../instruction.h"
#include "../memory/ram.h"
#include <cstdint>

    InstructionExecution& executor;

    explicit JumpExecution::JumpExecution(InstructionExecution& executor) : executor(executor) {}

    void JumpExecution::executeCALL(const Instruction& instruction) {
        // Save address of next instruction to stack memory
        executor.pushStack(executor.getProgramCounter());

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
        int pclathBits = (executor.getRamContent(RamMemory<uint8_t>::SFR::entries()[9].address) & 0b00011000) << 8;

        int address = instruction.getArguments()[0]; // Load jump address
        address = address & 0b001111111111;          // Clear upper two bits
        address = address | pclathBits;             // Adding PCLATH

        executor.setProgramCounter(address);
    }