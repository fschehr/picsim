#include "../instruction.cpp"
#include "../memory/stack.cpp"
#include "../memory/ram.cpp"
#include "instructionex.cpp"
#include <cstdint>

class LiteralExecution {
private:
    InstructionExecution& executor;

public:
    explicit LiteralExecution(InstructionExecution& executor) : executor(executor) {}

    void executeADDLW(const Instruction& instruction) {
        uint8_t argument = instruction.getArguments()[0];
        uint8_t workingRegister = executor.getWorkingRegister();

        executor.checkDigitCarryFlag((0xF < ((argument & 0xF) + (workingRegister & 0xF))));

        int result = (0xFF & argument) + (0xFF & workingRegister);

        executor.checkCarryFlag(result);
        executor.checkZeroFlag(result);
        executor.setWorkingRegister(static_cast<uint8_t>(result));
    }

    void executeSUBLW(const Instruction& instruction) {
        uint8_t argument = instruction.getArguments()[0];
        uint8_t workingRegister = executor.getWorkingRegister();

        executor.checkDigitCarryFlag((0xF < ((argument & 0xF) + ((~workingRegister + 1) & 0xF))));

        int result = (0xFF & argument) + (0xFF & (~workingRegister + 1));

        executor.checkCarryFlag(result);
        executor.checkZeroFlag(result);
        executor.setWorkingRegister(static_cast<uint8_t>(result));
    }

    void executeANDLW(const Instruction& instruction) {
        uint8_t argument = instruction.getArguments()[0];
        uint8_t workingRegister = executor.getWorkingRegister();

        int result = argument & workingRegister;

        executor.checkZeroFlag(result);
        executor.setWorkingRegister(static_cast<uint8_t>(result));
    }

    void executeMOVLW(const Instruction& instruction) {
        uint8_t argument = instruction.getArguments()[0];
        executor.setWorkingRegister(argument);
    }

    void executeIORLW(const Instruction& instruction) {
        uint8_t argument = instruction.getArguments()[0];
        uint8_t workingRegister = executor.getWorkingRegister();

        int result = argument | workingRegister;

        executor.checkZeroFlag(result);
        executor.setWorkingRegister(static_cast<uint8_t>(result));
    }

    void executeXORLW(const Instruction& instruction) {
        uint8_t argument = instruction.getArguments()[0];
        uint8_t workingRegister = executor.getWorkingRegister();

        int result = argument ^ workingRegister;

        executor.checkZeroFlag(result);
        executor.setWorkingRegister(static_cast<uint8_t>(result));
    }

    void executeRETLW(const Instruction& instruction) {
        // Restores address of next instruction from stack memory
        executor.setProgramCounter(executor.stack.pop());
        executor.setWorkingRegister(instruction.getArguments()[0]);
    }
};