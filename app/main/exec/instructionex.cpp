#include <vector>
#include <stdexcept>
#include <functional>
#include <map>
#include <mutex>
#include <cmath>
#include <iostream>
#include <string>
#include <memory>
#include "ProgramMemory.h"
#include "RamMemory.h"
#include "StackMemory.h"
#include "EepromMemory.h"
#include "Instruction.h"
#include "Decoder.h"
#include "LiteralExecution.h"
#include "JumpExecution.h"
#include "ByteExecution.h"
#include "BitExecution.h"

class InstructionExecution {
private:
    // Logger (placeholder for simplicity)
    std::string LOG = "InstructionExecution";

    // Registers and counters
    uint8_t workingRegister = 0;
    uint16_t instructionRegister = 0;
    int programCounter = 0;
    double runtimeCounter = 0.0;
    double frequency = 4000000.0;

    // Memory components
    ProgramMemory<uint16_t>& programMemory;
    RamMemory<uint8_t>& ram;
    StackMemory<int>& stack;
    EepromMemory<uint8_t>& eeprom;

    // ALU components
    LiteralExecution literalExecutionUnit;
    JumpExecution jumpExecutionUnit;
    ByteExecution byteAndControlExecutionUnit;
    BitExecution bitExecutionUnit;

    // Synchronization
    std::mutex lock;

    // Decoder
    Decoder decoder;

public:
    InstructionExecution(ProgramMemory<uint16_t>& programMemory, RamMemory<uint8_t>& ram,
                         StackMemory<int>& stack, EepromMemory<uint8_t>& eeprom)
        : programMemory(programMemory), ram(ram), stack(stack), eeprom(eeprom),
          literalExecutionUnit(*this), jumpExecutionUnit(*this),
          byteAndControlExecutionUnit(*this), bitExecutionUnit(*this) {}

    void init() {
        // Observe RAM memory for detecting reading/writing the EEPROM
        // Placeholder for adding property change listener
    }

    int execute() {
        std::lock_guard<std::mutex> guard(lock);

        try {
            // Check and handle interrupts
            if (checkTMR0Interrupt() || checkRB0Interrupt() || checkRBInterrupts()) {
                callISR(0x0004); // Calls ISR at address 0x0004
                updateTimer();
                return programCounter;
            }

            std::cout << "Program counter is: " << programCounter << std::endl;

            // Fetch and decode instruction
            setInstructionRegister(programMemory.get(programCounter));
            setProgramCounter(programCounter + 1);

            Instruction instruction = decoder.decode(instructionRegister);

            // Execute instruction
            switch (instruction.getOpc()) {
                case Instruction::OperationCode::ADDLW:
                    literalExecutionUnit.executeADDLW(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::ANDLW:
                    literalExecutionUnit.executeANDLW(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::MOVLW:
                    literalExecutionUnit.executeMOVLW(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::SUBLW:
                    literalExecutionUnit.executeSUBLW(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::IORLW:
                    literalExecutionUnit.executeIORLW(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::XORLW:
                    literalExecutionUnit.executeXORLW(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::RETLW:
                    literalExecutionUnit.executeRETLW(instruction);
                    updateRuntimeCounter(2);
                    break;
                case Instruction::OperationCode::CALL:
                    jumpExecutionUnit.executeCALL(instruction);
                    updateRuntimeCounter(2);
                    break;
                case Instruction::OperationCode::GOTO:
                    jumpExecutionUnit.executeGOTO(instruction);
                    updateRuntimeCounter(2);
                    break;
                // Add other cases for byte and bit operations
                default:
                    throw std::runtime_error("Unsupported instruction code");
            }
        } catch (const std::out_of_range& e) {
            std::cerr << e.what() << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }

        updateTimer();
        return programCounter;
    }

    void reset() {
        setWorkingRegister(0x00);
        setProgramCounter(0x00);
        setInstructionRegister(0x00);

        // Initialize runtime counter
        setRuntimeCounter(0.0);

        // Initialize special function registers
        ram.set(RamMemory<uint8_t>::SFR::INDF, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::TMR0, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::PCL, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::STATUS, 28);
        ram.set(RamMemory<uint8_t>::SFR::FSR, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::PORTA, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::PORTB, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::EEDATA, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::EEADR, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::PCLATH, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::INTCON, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::OPTION, 255);
        ram.set(RamMemory<uint8_t>::SFR::TRISA, 31);
        ram.set(RamMemory<uint8_t>::SFR::TRISB, 255);
        ram.set(RamMemory<uint8_t>::SFR::EECON1, 0x00);
        ram.set(RamMemory<uint8_t>::SFR::EECON2, 0x00);
    }

private:
    void updateRuntimeCounter(int cycles) {
        double timePerCycle = 4000000.0 / frequency;
        runtimeCounter += timePerCycle * cycles;
    }

    void updateTimer() {
        if (ram.get(RamMemory<uint8_t>::SFR::TMR0) == 0xFF) {
            ram.set(RamMemory<uint8_t>::SFR::INTCON, ram.get(RamMemory<uint8_t>::SFR::INTCON) | 4);
        }
        ram.set(RamMemory<uint8_t>::SFR::TMR0, ram.get(RamMemory<uint8_t>::SFR::TMR0) + 1);
    }

    bool checkTMR0Interrupt() {
        return (ram.get(RamMemory<uint8_t>::SFR::INTCON) & 0b10100100) == 0xA4;
    }

    bool checkRB0Interrupt() {
        return (ram.get(RamMemory<uint8_t>::SFR::INTCON) & 0b10010010) == 0x92;
    }

    bool checkRBInterrupts() {
        return (ram.get(RamMemory<uint8_t>::SFR::INTCON) & 0b10001001) == 0x89;
    }

    void callISR(int address) {
        ram.set(RamMemory<uint8_t>::SFR::INTCON, ram.get(RamMemory<uint8_t>::SFR::INTCON) & 0b01111111);
        stack.push(programCounter);

        int pclathBits = (ram.get(RamMemory<uint8_t>::SFR::PCLATH) & 24) << 8;
        address = (address & 2047) | pclathBits;

        setProgramCounter(address);
    }

    void setInstructionRegister(uint16_t value) {
        instructionRegister = value;
    }

    void setProgramCounter(int value) {
        programCounter = value;
    }

    void setWorkingRegister(uint8_t value) {
        workingRegister = value;
    }

    void setRuntimeCounter(double value) {
        runtimeCounter = value;
    }
};