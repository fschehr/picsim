#include <iostream>
#include <vector>

class Instruction {
public:
    enum class OperationCode {
        // Byte-Oriented file register operations
        ADDWF,
        ANDWF,
        CLRF,
        CLRW,
        COMF,
        DECF,
        DECFSZ,
        INCF,
        INCFSZ,
        IORWF,
        MOVF,
        MOVWF,
        NOP,
        RLF,
        RRF,
        SUBWF,
        SWAPF,
        XORWF,

        // Bit-Oriented file register operations
        BCF,
        BSF,
        BTFSC,
        BTFSS,

        // Literal and control operations
        ADDLW,
        ANDLW,
        CALL,
        CLRWDT,
        GOTO,
        IORLW,
        MOVLW,
        RETFIE,
        RETLW,
        RETURN,
        SLEEP,
        SUBLW,
        XORLW,
    };

    Instruction(OperationCode opc, int arguments = 0, int argument2 = 0) : opc_(opc), arguments_{arguments, argument2} {}

    OperationCode getOpc() const { return opc_; }

    std::vector<int> getArguments() const { return arguments_; }

private:
    OperationCode opc_;
    std::vector<int> arguments_;
};