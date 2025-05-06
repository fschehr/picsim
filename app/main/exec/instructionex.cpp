#include "instructionex.h"
#include "../memory/program.h"
#include "../memory/ram.h"
#include "../memory/stack.h"
#include "../memory/eeprom.h"
#include "../instruction.h"
#include "../decode.h"
#include "literalex.h"
#include "jumpex.h"
#include "byte.h"
#include "bit.h"
#include <iostream>

RamMemory<uint8_t>::SFR INDF = RamMemory<uint8_t>::SFR::entries()[0];
RamMemory<uint8_t>::SFR TMR0 = RamMemory<uint8_t>::SFR::entries()[1];
RamMemory<uint8_t>::SFR PCL = RamMemory<uint8_t>::SFR::entries()[2];
RamMemory<uint8_t>::SFR STATUS = RamMemory<uint8_t>::SFR::entries()[3];
RamMemory<uint8_t>::SFR FSR = RamMemory<uint8_t>::SFR::entries()[4];
RamMemory<uint8_t>::SFR PORTA = RamMemory<uint8_t>::SFR::entries()[5];
RamMemory<uint8_t>::SFR PORTB = RamMemory<uint8_t>::SFR::entries()[6];
RamMemory<uint8_t>::SFR EEDATA = RamMemory<uint8_t>::SFR::entries()[7];
RamMemory<uint8_t>::SFR EEADR = RamMemory<uint8_t>::SFR::entries()[8];
RamMemory<uint8_t>::SFR PCLATH = RamMemory<uint8_t>::SFR::entries()[9];
RamMemory<uint8_t>::SFR INTCON = RamMemory<uint8_t>::SFR::entries()[10];
RamMemory<uint8_t>::SFR OPTION = RamMemory<uint8_t>::SFR::entries()[11];
RamMemory<uint8_t>::SFR TRISA = RamMemory<uint8_t>::SFR::entries()[12];
RamMemory<uint8_t>::SFR TRISB = RamMemory<uint8_t>::SFR::entries()[13];
RamMemory<uint8_t>::SFR EECON1 = RamMemory<uint8_t>::SFR::entries()[14];
RamMemory<uint8_t>::SFR EECON2 = RamMemory<uint8_t>::SFR::entries()[15];


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

    InstructionExecution::InstructionExecution(ProgramMemory<uint16_t>& programMemory, RamMemory<uint8_t>& ram,
                         StackMemory<int>& stack, EepromMemory<uint8_t>& eeprom)
        : programMemory(programMemory), ram(ram), stack(stack), eeprom(eeprom),
          literalExecutionUnit(*this), jumpExecutionUnit(*this),
          byteAndControlExecutionUnit(*this), bitExecutionUnit(*this),
          workingRegister(0), instructionRegister(0), programCounter(0),
          runtimeCounter(0.0), frequency(4000000.0) {}

    void InstructionExecution::init() {
        // Observe RAM memory for detecting reading/writing the EEPROM
        ram.addPropertyChangeListener("ram", [this](int address, uint8_t oldValue, uint8_t newValue) {
            if (address == EECON1.address) {
                if ((newValue & 0b10) && (newValue & 0b1)) { // Check RD and WR bits
                    if (newValue & 0b10) { // RD bit
                        ram.set(EEDATA, eeprom.get(ram.get(EEADR)));
                    }
                    if (newValue & 0b1) { // WR bit
                        eeprom.set(ram.get(EEADR), ram.get(EEDATA));
                    }
                    ram.set(EECON1, newValue & 0b11111100); // Clear RD and WR bits
                }
            }
        });
    }

    int InstructionExecution::execute() {
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

    void InstructionExecution::reset() {
        setWorkingRegister(0x00);
        setProgramCounter(0x00);
        setInstructionRegister(0x00);

        // Initialize runtime counter
        setRuntimeCounter(0.0);

        // Initialize special function registers
        std::vector<RamMemory<uint8_t>::SFR> sfrEntries = RamMemory<uint8_t>::SFR::entries();
        ram.set(INDF, 0x00);
        ram.set(TMR0,0x00);
        ram.set(PCL, 0x00);
        ram.set(STATUS, 28);
        ram.set(FSR, 0x00);
        ram.set(PORTA, 0x00);
        ram.set(PORTB, 0x00);
        ram.set(EEDATA, 0x00);
        ram.set(EEADR, 0x00);
        ram.set(PCLATH, 0x00);
        ram.set(INTCON, 0x00);
        ram.set(OPTION, 255);
        ram.set(TRISA, 31);
        ram.set(TRISB, 255);
        ram.set(EECON1, 0x00);
        ram.set(EECON2, 0x00);
    }
    int InstructionExecution::getWorkingRegister() {
        return workingRegister;
    }
    int InstructionExecution::getProgramCounter() {
        return programCounter;
    }
    bool InstructionExecution::checkZeroFlag(int value) {
        ram.set(STATUS, (value & 0xFF) == 0 ? ram.get(STATUS) | 0x04 : ram.get(STATUS) & ~0x04);
        return (value & 0xFF) == 0;
    }
    bool InstructionExecution::checkCarryFlag(bool condition) {
        ram.set(STATUS, condition ? ram.get(STATUS) | 0x01 : ram.get(STATUS) & ~0x01);
        return condition;
    }
    bool InstructionExecution::checkDigitCarryFlag(bool condition) {
        ram.set(STATUS, condition ? ram.get(STATUS) | 0x02 : ram.get(STATUS) & ~0x02);
        return condition;
    }
    bool InstructionExecution::isCarryFlag() const {
        return (ram.get(STATUS) & 0x01) != 0;
    }
    StackMemory<int>& InstructionExecution::getStack(){
        return stack;
    }
    int InstructionExecution::getProgramCounter() {
        return programCounter;
    }

    //get ram content
    uint8_t InstructionExecution::getRamContent(int address) {
        return ram.get(address);
    }
    uint8_t InstructionExecution::getRamContent(RamMemory<uint8_t>::SFR sfr) const {
        return ram.get(sfr);
    }
    uint8_t InstructionExecution::getRamContent(RamMemory<uint8_t>::Bank bank, int address) const {
            // Assuming bank switching is handled externally, calculate the actual address
            int actualAddress = (static_cast<int>(bank) << 7) | address; // Example: Combine bank and address
            return ram.get(actualAddress);
        }
    //set ram content
    void InstructionExecution::setRamContent(RamMemory<uint8_t>::SFR sfr, uint8_t value) {
        ram.set(sfr, value);
    }
    void InstructionExecution::setRamContent(RamMemory<uint8_t>::Bank bank, int address, uint8_t value) {
        // Assuming bank switching is handled externally, calculate the actual address
        int actualAddress = (static_cast<int>(bank) << 7) | address; // Example: Combine bank and address
        ram.set(bank, actualAddress, value);
    }

    int InstructionExecution::getFileAddress(const Instruction& instruction) const {
        int address = instruction.getArguments()[0];
        RamMemory<uint8_t>::Bank bank = instruction.getBank();
        return (static_cast<int>(bank) << 7) | address; // Example: Combine bank and address
    }
    RamMemory<uint8_t>::Bank InstructionExecution::getSelectedBank(const Instruction& instruction) {
        return instruction.getBank();
    }

    void InstructionExecution::pushStack(int value) {
        stack.push(value);
    }
    int InstructionExecution::popStack() {
        return stack.pop();
    }

    void InstructionExecution::updateRuntimeCounter(int cycles) {
        double timePerCycle = 4000000.0 / frequency;
        runtimeCounter += timePerCycle * cycles;
    }

    void InstructionExecution::updateTimer() {
        if (ram.get(TMR0) == 0xFF) {
            ram.set(INTCON, ram.get(INTCON) | 4);
        }
        ram.set(TMR0, ram.get(TMR0) + 1);
    }

    bool InstructionExecution::checkTMR0Interrupt() {
        return (ram.get(INTCON) & 0b10100100) == 0xA4;
    }

    bool InstructionExecution::checkRB0Interrupt() {
        return (ram.get(INTCON) & 0b10010010) == 0x92;
    }

    bool InstructionExecution::checkRBInterrupts() {
        return (ram.get(INTCON) & 0b10001001) == 0x89;
    }

    void InstructionExecution::callISR(int address) {
        ram.set(INTCON, ram.get(INTCON) & 0b01111111);
        stack.push(programCounter);

        int pclathBits = (ram.get(PCLATH) & 24) << 8;
        address = (address & 2047) | pclathBits;

        setProgramCounter(address);
    }

    void InstructionExecution::setInstructionRegister(uint16_t value) {
        instructionRegister = value;
    }

    void InstructionExecution::setProgramCounter(int value) {
        programCounter = value;
    }

    void InstructionExecution::setWorkingRegister(uint8_t value) {
        workingRegister = value;
    }

    void InstructionExecution::setRuntimeCounter(double value) {
        runtimeCounter = value;
    }
