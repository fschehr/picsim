#include "byte.h"
#include "instructionex.h"
#include "../instruction.h"
#include "../memory/ram.h"
#include "../logger.h"
#include <cstdint>

    ByteExecution::ByteExecution(InstructionExecution& executor, const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines, const std::vector<std::pair<short,short>>& prog) : executor(executor), fileLines(fileLines), prog(prog) {}

    void ByteExecution::executeADDWF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        int result = (0xFF & value) + (0xFF & executor.getWorkingRegister());

        executor.checkDigitCarryFlag(((value & 0xF) + (executor.getWorkingRegister() & 0xF)) > 0xF);
        executor.checkCarryFlag(result > 0xFF);
        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.setRamContent(bank, address, static_cast<uint8_t>(result));
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeANDWF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        int result = value & executor.getWorkingRegister();

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.setRamContent(bank, address, static_cast<uint8_t>(result));
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeXORWF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        RamMemory<uint8_t>::Bank bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        int result = value ^ executor.getWorkingRegister();

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.setRamContent(bank, address, static_cast<uint8_t>(result));
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeSUBWF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        int result = (0xFF & value) - (0xFF & executor.getWorkingRegister());

        executor.checkDigitCarryFlag(((value & 0xF) - (executor.getWorkingRegister() & 0xF)) >= 0);
        executor.checkCarryFlag(value >= executor.getWorkingRegister());
        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.setRamContent(bank, address, static_cast<uint8_t>(result));
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeCLRW() {
        Logger::info("executing CLRW");
        executor.setWorkingRegister(0x00);
        executor.setZeroFlag();
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeRETURN() {
        Logger::info("executing RETURN");
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
        executor.setProgramCounter(executor.popStack());
    }

    void ByteExecution::executeMOVWF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        executor.setRamContent(bank, address, executor.getWorkingRegister());
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeCLRF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);
        Logger::info("Bank: " + std::to_string(static_cast<int>(bank)) + " Address: " + std::to_string(address));
        executor.setRamContent(bank, address, 0x00);
        executor.setZeroFlag();
        executor.setByVM = 1;
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeCOMF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        int result = ~value;

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.setRamContent(bank, address, static_cast<uint8_t>(result));
            executor.setByVM = 1;
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeDECF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        int result = value - 1;

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.setRamContent(bank, address, static_cast<uint8_t>(result));
            executor.setByVM = 1;
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeINCF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        int result = value + 1;

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.setRamContent(bank, address, static_cast<uint8_t>(result));
            executor.setByVM = 1;
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeMOVF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);

        executor.checkZeroFlag(value);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(value);
        } else {
            executor.setRamContent(bank, address, value);
            executor.setByVM = 1;
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeIORWF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        int result = executor.getWorkingRegister() | value;

        executor.checkZeroFlag(result);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.setRamContent(bank, address, static_cast<uint8_t>(result));
            executor.setByVM = 1;
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeDECFSZ(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        int result = value - 1;
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;

        if (result == 0) {
            executor.setProgramCounter(executor.getProgramCounter() + 1);
        }

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.setRamContent(bank, address, static_cast<uint8_t>(result));
            executor.setByVM = 1;
        }
    }

    void ByteExecution::executeINCFSZ(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        int result = value + 1;
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;

        if (result == 0) {
            executor.setProgramCounter(executor.getProgramCounter() + 1);
        }

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(static_cast<uint8_t>(result));
        } else {
            executor.setRamContent(bank, address, static_cast<uint8_t>(result));
            executor.setByVM = 1;
        }
    }

    void ByteExecution::executeRLF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        uint8_t newCarryFlag = (value & 0x80) >> 7;

        value = (value << 1) | (executor.isCarryFlag() ? 0x01 : 0x00);

        executor.checkCarryFlag(newCarryFlag);
        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(value);
        } else {
            executor.setRamContent(bank, address, value);
            executor.setByVM = 1;
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeRRF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        uint8_t newCarryFlag = value & 0x01;

        value = (value >> 1) | (executor.isCarryFlag() ? 0x80 : 0x00);

        executor.checkCarryFlag(newCarryFlag);
        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(value);
        } else {
            executor.setRamContent(bank, address, value);
            executor.setByVM = 1;
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeNOP() {
        Logger::info("executing NOP");
        // No operation
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeSWAPF(const Instruction& instruction) {
        Logger::info("executing " + instruction.toString());
        int address = executor.getFileAddress(instruction);
        auto bank = executor.getSelectedBank(instruction);

        uint8_t value = executor.getRamContent(bank, address);
        uint8_t result = ((value & 0x0F) << 4) | ((value & 0xF0) >> 4);

        if (instruction.getArguments()[0] == 0) {
            executor.setWorkingRegister(result);
        } else {
            executor.setRamContent(bank, address, result);
            executor.setByVM = 1;
        }
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
    }

    void ByteExecution::executeRETFIE(const Instruction& instruction) {
        Logger::info("executing RETFIE");
        executor.setRamContent(RamMemory<uint8_t>::SFR::entries()[10], executor.getRamContent(RamMemory<uint8_t>::SFR::entries()[10]) | 0x80);
        executor.setByVM = 1;
        //const_cast<std::pair<bool,bool*>&>(fileLines[prog[executor.getProgramCounter()].first].first).first = false;
        executor.setProgramCounter(executor.popStack());
    }
