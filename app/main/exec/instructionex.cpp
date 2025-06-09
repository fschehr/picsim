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
#include <algorithm>

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
    InstructionExecution::InstructionExecution(ProgramMemory<uint16_t>& programMemory, RamMemory<uint8_t>& ram,
                                               StackMemory<int>& stack, EepromMemory<uint8_t>& eeprom, const std::vector<std::pair<std::pair<bool,bool*>,std::pair<short, std::string>>>& fileLines, const std::vector<std::pair<short,short>>& prog)
        : programMemory(programMemory), ram(ram), stack(stack), eeprom(eeprom),
          literalExecutionUnit(*this, fileLines, prog), 
          jumpExecutionUnit(*this, fileLines, prog),
          byteAndControlExecutionUnit(*this, fileLines, prog), 
          bitExecutionUnit(*this, fileLines, prog),
          workingRegister(0), 
          instructionRegister(0), 
          programCounter(0),
          runtimeCounter(0),
          fileLines(fileLines),
          prog(prog),
          wdtEnabled(true),  // Can be controlled by configuration bits
          wdtCounter(0),
          wdtTimeout(18000)  // 18ms default timeout
    { }

    void InstructionExecution::init() {
        static bool eeWriteSequenceValid = false;
        static bool writeTimerActive = false;
        static uint8_t lastEECON2 = 0;

        // Observe RAM memory for detecting reading/writing the EEPROM
        ram.addPropertyChangeListener("ram", [this](int address, uint8_t oldValue, uint8_t newValue) {
            if (address == EECON2.address) {
                // Check write sequence: 55h followed by AAh
                if (lastEECON2 == 0x55 && newValue == 0xAA) {
                    eeWriteSequenceValid = true;
                } else {
                    eeWriteSequenceValid = false;
                }
                lastEECON2 = newValue;
            }
            else if (address == EECON1.address) {
                // Handle read operation
                if (newValue & 0b10) { // RD bit set
                    // Perform EEPROM read
                    try {
                        ram.set(EEDATA, eeprom.get(ram.get(EEADR)));
                    } catch (const std::out_of_range&) {
                        // Invalid address - do nothing
                    }
                    ram.set(EECON1, newValue & ~0b10); // Clear RD bit after read
                }
                
                // Handle write operation
                if (newValue & 0b1) { // WR bit set
                    if (!(ram.get(EECON1) & 0b100)) { // WREN bit not set
                        // Write not enabled - set WRERR
                        ram.set(EECON1, ram.get(EECON1) | 0b1000);
                        ram.set(EECON1, newValue & ~0b1); // Clear WR bit
                        return;
                    }
                    
                    if (!eeWriteSequenceValid) {
                        // Invalid write sequence - set WRERR
                        ram.set(EECON1, ram.get(EECON1) | 0b1000);
                        ram.set(EECON1, newValue & ~0b1); // Clear WR bit
                        return;
                    }

                    if (!writeTimerActive) {
                        writeTimerActive = true;
                        
                        // Start EEPROM write cycle
                        try {
                            eeprom.set(ram.get(EEADR), ram.get(EEDATA));
                            
                            // Write complete - set EEIF and clear WR
                            uint8_t eecon1 = ram.get(EECON1);
                            ram.set(EECON1, (eecon1 | 0b10000) & ~0b1); // Set EEIF, clear WR
                            
                            // Set INTCON.EEIE if enabled
                            if (ram.get(INTCON) & 0b01000000) { // EEIE bit
                                ram.set(INTCON, ram.get(INTCON) | 0b10000000); // Set GIE
                            }
                            
                        } catch (const std::out_of_range&) {
                            // Invalid address - set WRERR
                            ram.set(EECON1, ram.get(EECON1) | 0b1000);
                        }
                        
                        writeTimerActive = false;
                        eeWriteSequenceValid = false;
                    }
                }
            }
        });
    }

    int InstructionExecution::execute() {
        std::lock_guard<std::mutex> guard(lock);

        try {
            // Update and check WDT before instruction execution
            updateWDT();
            if (checkWDTTimeout()) {
                Logger::info("WDT timeout occurred");
                reset();  // WDT timeout causes reset
                return programCounter;
            }

            // Check and handle interrupts
            if (checkTMR0Interrupt() || checkRB0Interrupt() || checkRBInterrupts()) {
                callISR(0x0004); // Calls ISR at address 0x0004
                updateTimer();
                return programCounter;
            }
            Logger::info("Program counter is: " + std::to_string(programCounter));
            Logger::info("is set by vm"+std::to_string(setByVM));
            // Fetch and decode instruction
            setInstructionRegister(programMemory.get(programCounter));
            if(*fileLines[prog[programCounter].first].first.second){
                Logger::info("reached Breakpoint");
            }
            setProgramCounter(programCounter+1);
            Instruction instruction = decoder.decode(instructionRegister);
            Logger::info("Executing instruction: " + instruction.toString());
            

            //literalex.cpp
            switch (instruction.getOpc()) {
                case Instruction::OperationCode::ADDLW:
                    literalExecutionUnit.executeADDLW(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::SUBLW:
                    literalExecutionUnit.executeSUBLW(instruction);
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

                //jumpex.cpp
                case Instruction::OperationCode::CALL:
                    jumpExecutionUnit.executeCALL(instruction);
                    updateRuntimeCounter(2);
                    break;
                case Instruction::OperationCode::GOTO:
                    jumpExecutionUnit.executeGOTO(instruction);
                    updateRuntimeCounter(2);
                    break;

                //byte.cpp
                case Instruction::OperationCode::ADDWF:
                    byteAndControlExecutionUnit.executeADDWF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::ANDWF:
                    byteAndControlExecutionUnit.executeANDWF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::XORWF:
                    byteAndControlExecutionUnit.executeXORWF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::SUBWF:
                    byteAndControlExecutionUnit.executeSUBWF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::CLRW:
                    byteAndControlExecutionUnit.executeCLRW();
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::RETURN:
                    byteAndControlExecutionUnit.executeRETURN();
                    updateRuntimeCounter(2);
                    break;
                case Instruction::OperationCode::MOVWF:
                    byteAndControlExecutionUnit.executeMOVWF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::CLRF:
                    byteAndControlExecutionUnit.executeCLRF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::COMF:
                    byteAndControlExecutionUnit.executeCOMF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::DECF:
                    byteAndControlExecutionUnit.executeDECF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::INCF:
                    byteAndControlExecutionUnit.executeINCF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::MOVF:
                    byteAndControlExecutionUnit.executeMOVF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::IORWF:
                    byteAndControlExecutionUnit.executeIORWF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::DECFSZ:
                    byteAndControlExecutionUnit.executeDECFSZ(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::INCFSZ:
                    byteAndControlExecutionUnit.executeINCFSZ(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::RLF:
                    byteAndControlExecutionUnit.executeRLF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::RRF:
                    byteAndControlExecutionUnit.executeRRF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::NOP:
                    byteAndControlExecutionUnit.executeNOP();
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::SWAPF:
                    byteAndControlExecutionUnit.executeSWAPF(instruction);
                    updateRuntimeCounter(1);
                    break;                
                case Instruction::OperationCode::RETFIE:
                    byteAndControlExecutionUnit.executeRETFIE(instruction);
                    updateRuntimeCounter(2);
                    break;
                case Instruction::OperationCode::SLEEP:
                    byteAndControlExecutionUnit.executeSLEEP();
                    updateRuntimeCounter(1);
                    break;

                //bit.cpp
                case Instruction::OperationCode::BCF:
                    bitExecutionUnit.executeBCF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::BSF:
                    bitExecutionUnit.executeBSF(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::BTFSC:
                    bitExecutionUnit.executeBTFSC(instruction);
                    updateRuntimeCounter(1);
                    break;
                case Instruction::OperationCode::BTFSS:
                    bitExecutionUnit.executeBTFSS(instruction);
                    updateRuntimeCounter(1);
                    break;
                
                default:
                    throw std::runtime_error("Unsupported instruction code");
            }
        } catch (const std::out_of_range& e) {
            Logger::warning(std::string("Out of range error during execution: ") + e.what());
        } catch (const std::runtime_error& e) {
            Logger::warning(std::string("Runtime error during execution: ") + e.what());
        }
        const_cast<std::pair<bool,bool*>&>(fileLines[prog[prevProgCounter].first].first).first = false;
        const_cast<std::pair<bool,bool*>&>(fileLines[prog[programCounter].first].first).first = true;
        prevProgCounter = programCounter; 
        updateTimer();
        return programCounter;
    }

    void InstructionExecution::reset() {
        setWorkingRegister(0x00);
        setProgramCounter(0x00);
        setInstructionRegister(0x00);

        // Initialize runtime counter
        setRuntimeCounter(0);

        // Initialize special function registers
        std::vector<RamMemory<uint8_t>::SFR> sfrEntries = RamMemory<uint8_t>::SFR::entries();
        ram.set(INDF, 0x00);
        ram.set(TMR0,0x00);
        ram.set(PCL, 0x00);
        ram.set(STATUS, 0x18);
        ram.set(FSR, 0x00);
        ram.set(PORTA, 0x00);
        ram.set(PORTB, 0x00);
        ram.set(EEDATA, 0x00);
        ram.set(EEADR, 0x00);
        ram.set(PCLATH, 0x00);
        ram.set(INTCON, 0x00);
        ram.set(OPTION, 255);
        ram.set(TRISA, 0x1F);
        ram.set(TRISB, 0xFF);
        ram.set(EECON1, 0x00);
        ram.set(EECON2, 0x00);
        Logger::info("Simulator reset");
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
    bool InstructionExecution::isZeroFlag() const {
        return (ram.get(STATUS) & 0x04) != 0;
    }
    bool InstructionExecution::isDigitCarryFlag() const {
        return (ram.get(STATUS) & 0x02) != 0;
    }
    StackMemory<int>& InstructionExecution::getStack(){
        return stack;
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
    void InstructionExecution::setRamContent(int address, uint8_t value) {
        // Speichere den Wert direkt im RAM an der angegebenen Adresse
        // Wähle die Bank basierend auf dem höchsten Bit der Adresse (0x80)
        RamMemory<uint8_t>::Bank bank = (address < 0x80) ? RamMemory<uint8_t>::Bank::BANK_0 : RamMemory<uint8_t>::Bank::BANK_1;
        
        // Maskiere die niedrigeren 7 Bits (0x7F = 0b01111111), um die tatsächliche Adresse innerhalb der Bank zu erhalten
        int bankAddress = address & 0x7F;
        
        // Für Spezialregister in Bank 1 prüfen, ob sie in der SFR-Liste vorhanden sind
        if (bank == RamMemory<uint8_t>::Bank::BANK_1) {
            // Option Register, TRISA, TRISB, etc. sind Spezialfälle
            try {
                // Versuche, ein SFR für diese Adresse zu finden
                RamMemory<uint8_t>::SFR sfr = RamMemory<uint8_t>::SFR::valueOf(bank, bankAddress);
                ram.set(sfr, value);
                return;
            } catch (const std::invalid_argument&) {
                // Kein SFR gefunden, setze normal fort
            }
        }
        
        ram.set(bank, bankAddress, value);
    }
    
    void InstructionExecution::setRamContent(RamMemory<uint8_t>::SFR sfr, uint8_t value) {
        ram.set(sfr, value);
    }
    
    void InstructionExecution::setRamContent(RamMemory<uint8_t>::Bank bank, int address, uint8_t value) {
        // When setting a value to a mirrored register, we need to update both banks
        if (address < 0x0C && shouldMirrorAddress(address)) {  // First 12 addresses might be mirrored
            ram.set(RamMemory<uint8_t>::Bank::BANK_0, address, value);
            ram.set(RamMemory<uint8_t>::Bank::BANK_1, address, value);
            return;
        }
        ram.set(bank, address, value);
    }

    int InstructionExecution::getFileAddress(const Instruction& instruction) const {
        int address = instruction.getArguments()[0];
        RamMemory<uint8_t>::Bank bank = instruction.getBank();
        return (static_cast<int>(bank) << 7) | address; // Example: Combine bank and address
    }
    RamMemory<uint8_t>::Bank InstructionExecution::getSelectedBank(const Instruction& instruction) {
        uint8_t status = ram.get(STATUS);  // Use STATUS SFR directly
        bool rp0Set = ((status & (1 << 5)) != 0); // Check RP0 bit (bit 5 of STATUS)
        Logger::info("Bank selection: RP0=" + std::to_string(rp0Set) + " STATUS=" + std::to_string(status));
        return rp0Set ? RamMemory<uint8_t>::Bank::BANK_1 : RamMemory<uint8_t>::Bank::BANK_0;
    }

    void InstructionExecution::pushStack(int value) {
        stack.push(value);
    }
    int InstructionExecution::popStack() {
        return stack.pop();
    }

    void InstructionExecution::updateRuntimeCounter(int cycles) {
        // Interne Zählung für die Laufzeit
        runtimeCounter += cycles;
        
        // Callback aufrufen, wenn verfügbar
        if (cycleUpdateCallback) {
            cycleUpdateCallback(cycles);
        }
    }

    void InstructionExecution::updateTimer() {
        if (ram.get(TMR0) == 0xFF) {
            ram.set(INTCON, ram.get(INTCON) | 4);
        }
        ram.set(TMR0, ram.get(TMR0) + 1);
    }

    void InstructionExecution::updateWDT() {
        if (!wdtEnabled) return;
        
        // Get prescaler value if assigned to WDT
        uint8_t option = ram.get(OPTION);
        bool psa = (option & 0b00001000) != 0; // PSA bit
        if (psa) { // Prescaler is assigned to WDT
            uint8_t ps = option & 0b00000111; // PS2:PS0 bits
            uint32_t prescalerValue = 1 << ps; // 1:1 to 1:128
            wdtCounter += prescalerValue;
        } else {
            wdtCounter++; // No prescaler for WDT
        }
    }    void InstructionExecution::clearWDT() {
        // 1. Clear WDT counter
        wdtCounter = 0;

        // 2. Clear WDT prescaler if assigned to WDT
        uint8_t option = ram.get(OPTION);
        bool psa = (option & 0b00001000) != 0; // PSA bit
        if (psa) { // If prescaler is assigned to WDT
            // The prescaler is automatically cleared when WDT is cleared
            // Nothing to do here - prescaler state is implicitly reset when wdtCounter is cleared
        }

        // 3. Set both TO (bit 4) and PD (bit 3) bits in STATUS register
        uint8_t status = ram.get(STATUS);
        ram.set(STATUS, status | 0b00011000); // Set both TO and PD bits
    }

    bool InstructionExecution::checkWDTTimeout() {
        if (!wdtEnabled) return false;
        
        if (wdtCounter >= wdtTimeout) {
            // WDT timeout occurred
            clearWDT();
            // Clear TO bit in STATUS register on timeout
            uint8_t status = ram.get(STATUS);
            ram.set(STATUS, status & ~0b00010000);
            return true;
        }
        return false;
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
        if(ram.get(PCLATH) != 0x00){
            //if pclath bit is set, add the bits from PCLATH to PCL. 1111 1111 1111 1111
            //                                                          |----| |-------|
            //                                                          pclath     pc
            value = (value & 0x07FF) | ((ram.get(PCLATH) & 0x1C) << 8);
        }
        //PCL are the lower 8 bits of the program counter
        ram.set(PCL, value & 0xFF);
        programCounter = value;
    }

    void InstructionExecution::setWorkingRegister(uint8_t value) {
        workingRegister = value;
    }

    void InstructionExecution::setRuntimeCounter(int value) {
        runtimeCounter = value;
    }

    void InstructionExecution::setZeroFlag() {
        uint8_t status = ram.get(STATUS);
        ram.set(STATUS, (status | 0x04));
    }

    bool InstructionExecution::shouldMirrorAddress(int address) {
        // These addresses are mirrored between banks
        static const std::vector<int> mirroredAddresses = {
            0x00,  // INDF
            0x02,  // PCL
            0x03,  // STATUS
            0x04,  // FSR
            0x0A,  // PCLATH
            0x0B   // INTCON
        };
        return std::find(mirroredAddresses.begin(), mirroredAddresses.end(), address) != mirroredAddresses.end();
    }
