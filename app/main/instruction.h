#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <vector>

class Instruction {
public:
    enum class OperationCode {
        ADDWF, ANDWF, CLRF, CLRW, COMF, DECF, DECFSZ, INCF, INCFSZ, IORWF, MOVF, MOVWF, NOP, RLF, RRF, SUBWF, SWAPF, XORWF,
        BCF, BSF, BTFSC, BTFSS,
        ADDLW, ANDLW, CALL, CLRWDT, GOTO, IORLW, MOVLW, RETFIE, RETLW, RETURN, SLEEP, SUBLW, XORLW
    };

    Instruction(OperationCode opc, int arguments = 0, int argument2 = 0);
    OperationCode getOpc() const;
    std::vector<int> getArguments() const;

private:
    OperationCode opc_;
    std::vector<int> arguments_;
};

#endif // INSTRUCTION_H
