#include <iostream>
#include <vector>
#include <sstream>
#include "instruction.h"

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

    Instruction(OperationCode opc, int arguments = 0, int argument2 = 0) 
        : opc_(opc), arguments_{arguments, argument2} {}

    OperationCode getOpc() const { return opc_; }

    std::vector<int> getArguments() const { return arguments_; }

    std::string getArgumentsAsString() const {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < arguments_.size(); ++i) {
            oss << arguments_[i];
            if (i < arguments_.size() - 1) {
                oss << ", ";
            }
        }
        oss << "]";
        return oss.str();
    }

private:
    OperationCode opc_;
    std::vector<int> arguments_;
};

std::ostream& operator<<(std::ostream& os, Instruction::OperationCode opc) {
    switch (opc) {
        case Instruction::OperationCode::ADDWF: os << "ADDWF"; break;
        case Instruction::OperationCode::ANDWF: os << "ANDWF"; break;
        case Instruction::OperationCode::CLRF: os << "CLRF"; break;
        case Instruction::OperationCode::CLRW: os << "CLRW"; break;
        case Instruction::OperationCode::COMF: os << "COMF"; break;
        case Instruction::OperationCode::DECF: os << "DECF"; break;
        case Instruction::OperationCode::DECFSZ: os << "DECFSZ"; break;
        case Instruction::OperationCode::INCF: os << "INCF"; break;
        case Instruction::OperationCode::INCFSZ: os << "INCFSZ"; break;
        case Instruction::OperationCode::IORWF: os << "IORWF"; break;
        case Instruction::OperationCode::MOVF: os << "MOVF"; break;
        case Instruction::OperationCode::MOVWF: os << "MOVWF"; break;
        case Instruction::OperationCode::NOP: os << "NOP"; break;
        case Instruction::OperationCode::RLF: os << "RLF"; break;
        case Instruction::OperationCode::RRF: os << "RRF"; break;
        case Instruction::OperationCode::SUBWF: os << "SUBWF"; break;
        case Instruction::OperationCode::SWAPF: os << "SWAPF"; break;
        case Instruction::OperationCode::XORWF: os << "XORWF"; break;
        case Instruction::OperationCode::BCF: os << "BCF"; break;
        case Instruction::OperationCode::BSF: os << "BSF"; break;
        case Instruction::OperationCode::BTFSC: os << "BTFSC"; break;
        case Instruction::OperationCode::BTFSS: os << "BTFSS"; break;
        case Instruction::OperationCode::ADDLW: os << "ADDLW"; break;
        case Instruction::OperationCode::ANDLW: os << "ANDLW"; break;
        case Instruction::OperationCode::CALL: os << "CALL"; break;
        case Instruction::OperationCode::CLRWDT: os << "CLRWDT"; break;
        case Instruction::OperationCode::GOTO: os << "GOTO"; break;
        case Instruction::OperationCode::IORLW: os << "IORLW"; break;
        case Instruction::OperationCode::MOVLW: os << "MOVLW"; break;
        case Instruction::OperationCode::RETFIE: os << "RETFIE"; break;
        case Instruction::OperationCode::RETLW: os << "RETLW"; break;
        case Instruction::OperationCode::RETURN: os << "RETURN"; break;
        case Instruction::OperationCode::SLEEP: os << "SLEEP"; break;
        case Instruction::OperationCode::SUBLW: os << "SUBLW"; break;
        case Instruction::OperationCode::XORLW: os << "XORLW"; break;
        default: os << "UNKNOWN"; break;
    }
    return os;
}