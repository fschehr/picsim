#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>
#include <string>
#include <ostream>
#include <initializer_list>
#include "memory/ram.h"
#include "logger.h"

class Instruction {
public:
    enum class OperationCode {
        ADDWF, ANDWF, CLRF, CLRW, COMF, DECF, DECFSZ, INCF, INCFSZ, IORWF, MOVF, MOVWF, NOP, RLF, RRF, SUBWF, SWAPF, XORWF,
        BCF, BSF, BTFSC, BTFSS,
        ADDLW, ANDLW, CALL, CLRWDT, GOTO, IORLW, MOVLW, RETFIE, RETLW, RETURN, SLEEP, SUBLW, XORLW
    };

    Instruction();
    ~Instruction();
    Instruction(OperationCode opc, std::initializer_list<int> args);
    Instruction(OperationCode opc, const std::vector<int>& args);
    OperationCode getOpc() const;
    std::vector<int> getArguments() const;
    std::string getArgumentsAsString() const;
    RamMemory<uint8_t>::Bank getBank() const;
private:
    OperationCode opc_;
    std::vector<int> arguments_;
};

std::ostream& operator<<(std::ostream& os, Instruction::OperationCode opc);

#endif // INSTRUCTION_H
