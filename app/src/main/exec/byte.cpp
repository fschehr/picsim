#include "InstructionExecution.h"
#include "Instruction.h"
#include "RamMemory.h"
#include <cstdint>

class ByteExecution {
private:
    InstructionExecution& executor;

public:
    explicit ByteExecution(InstructionExecution& executor) : executor(executor) {}

    void executeADDWF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        int result = (0xFF & value) + (0xFF & executor.getWorkingRegister());

        executor.checkDigitCarryFlag(0xF < ((value & 0xF) + (executor.getWorkingRegister() & 0xF)));
        executor.checkCarryFlag(result);
        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.ram.set(bank, address, static_cast<uint8_t>(result));
        }
    }

    void executeANDWF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        int result = value & executor.getWorkingRegister();

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.ram.set(bank, address, static_cast<uint8_t>(result));
        }
    }

    void executeXORWF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        int result = value ^ executor.getWorkingRegister();

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.ram.set(bank, address, static_cast<uint8_t>(result));
        }
    }

    void executeSUBWF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        int result = (0xFF & value) + (0xFF & (~executor.getWorkingRegister() + 1));

        executor.checkDigitCarryFlag(0xF < ((value & 0xF) + ((~executor.getWorkingRegister() + 1) & 0xF)));
        executor.checkCarryFlag(result);
        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.ram.set(bank, address, static_cast<uint8_t>(result));
        }
    }

    void executeCLRW() {
        executor.setWorkingRegister(0x00);
        executor.setZeroFlag();
    }

    void executeRETURN() {
        executor.setProgramCounter(executor.stack.pop());
    }

    void executeMOVWF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        executor.ram.set(bank, address, executor.getWorkingRegister());
    }

    void executeCLRF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        executor.ram.set(bank, address, 0x00);
        executor.setZeroFlag();
    }

    void executeCOMF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        int result = ~value;

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.ram.set(bank, address, static_cast<uint8_t>(result));
        }
    }

    void executeDECF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        int result = value - 1;

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.ram.set(bank, address, static_cast<uint8_t>(result));
        }
    }

    void executeINCF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        int result = value + 1;

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.ram.set(bank, address, static_cast<uint8_t>(result));
        }
    }

    void executeMOVF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);

        executor.checkZeroFlag(value);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(value);
        } else {
            executor.ram.set(bank, address, value);
        }
    }

    void executeIORWF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        int result = executor.getWorkingRegister() | value;

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.ram.set(bank, address, static_cast<uint8_t>(result));
        }
    }

    void executeDECFSZ(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        int result = value - 1;

        if (result == 0) {
            executor.setProgramCounter(executor.getProgramCounter() + 1);
        }

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.ram.set(bank, address, static_cast<uint8_t>(result));
        }
    }

    void executeINCFSZ(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        int result = value + 1;

        if (result == 0) {
            executor.setProgramCounter(executor.getProgramCounter() + 1);
        }

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.ram.set(bank, address, static_cast<uint8_t>(result));
        }
    }

    void executeRLF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        uint8_t newCarryFlag = (value & 0x80) >> 7;

        value = (value << 1) | (executor.isCarryFlag() ? 0x01 : 0x00);

        if (newCarryFlag) {
            executor.setCarryFlag();
        } else {
            executor.clearCarryFlag();
        }

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(value);
        } else {
            executor.ram.set(bank, address, value);
        }
    }

    void executeRRF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        uint8_t newCarryFlag = value & 0x01;

        value = (value >> 1) | (executor.isCarryFlag() ? 0x80 : 0x00);

        if (newCarryFlag) {
            executor.setCarryFlag();
        } else {
            executor.clearCarryFlag();
        }

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(value);
        } else {
            executor.ram.set(bank, address, value);
        }
    }

    void executeNOP() {
        // No operation
    }

    void executeSWAPF(const Instruction& instruction) {
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.ram.get(bank, address);
        uint8_t result = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(result);
        } else {
            executor.ram.set(bank, address, result);
        }
    }

    void executeRETFIE(const Instruction& instruction) {
        executor.ram.set(RamMemory<uint8_t>::SFR::INTCON, executor.ram.get(RamMemory<uint8_t>::SFR::INTCON) | 0x80);
        executor.setProgramCounter(executor.stack.pop());
    }
};