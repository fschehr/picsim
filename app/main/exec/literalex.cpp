#include "instructionex.h"
#include "../instruction.h"
#include "../memory/stack.h"
#include "../memory/ram.h"
#include "../logger.h"
#include <cstdint>

    LiteralExecution::LiteralExecution(InstructionExecution& executor) : executor(executor) {}

    void LiteralExecution::executeADDLW(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        uint8_t argument = instruction.getArguments()[0];
        uint8_t workingRegister = executor.getWorkingRegister();

        executor.checkDigitCarryFlag((0xF < ((argument & 0xF) + (workingRegister & 0xF))));

        int result = (0xFF & argument) + (0xFF & workingRegister);

        executor.checkCarryFlag(result);
        executor.checkZeroFlag(result);
        executor.setWorkingRegister(static_cast<uint8_t>(result));
    }

    void LiteralExecution::executeSUBLW(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        uint8_t argument = instruction.getArguments()[0];
        uint8_t workingRegister = executor.getWorkingRegister();

        executor.checkDigitCarryFlag((0xF < ((argument & 0xF) + ((~workingRegister + 1) & 0xF))));

        int result = (0xFF & argument) + (0xFF & (~workingRegister + 1));

        executor.checkCarryFlag(result);
        executor.checkZeroFlag(result);
        executor.setWorkingRegister(static_cast<uint8_t>(result));
    }

    void LiteralExecution::executeANDLW(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        uint8_t argument = instruction.getArguments()[0];
        uint8_t workingRegister = executor.getWorkingRegister();

        int result = argument & workingRegister;

        executor.checkZeroFlag(result);
        executor.setWorkingRegister(static_cast<uint8_t>(result));
    }

    void LiteralExecution::executeMOVLW(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        uint8_t argument = instruction.getArguments()[0];
        executor.setWorkingRegister(argument);
    }

    void LiteralExecution::executeIORLW(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        uint8_t argument = instruction.getArguments()[0];
        uint8_t workingRegister = executor.getWorkingRegister();

        int result = argument | workingRegister;

        executor.checkZeroFlag(result);
        executor.setWorkingRegister(static_cast<uint8_t>(result));
    }

    void LiteralExecution::executeXORLW(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        uint8_t argument = instruction.getArguments()[0];
        uint8_t workingRegister = executor.getWorkingRegister();

        int result = argument ^ workingRegister;

        executor.checkZeroFlag(result);
        executor.setWorkingRegister(static_cast<uint8_t>(result));
    }

    void LiteralExecution::executeRETLW(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        // Restores address of next instruction from stack memory
        executor.setProgramCounter(executor.popStack());
        executor.setWorkingRegister(instruction.getArguments()[0]);
    }