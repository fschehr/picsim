#include "decode.h"
#include "instruction.h"
#include <iostream>
#include <stdexcept>
#include <cstdint>

    static const int LITERAL_8BIT_MASK = 0b00000011111111;
    static const int LITERAL_OPC_MASK = 0b11111100000000;
    static const int OPERATION_TYPE_MASK = 0b11000000000000;
    static const int OPERATION_TYPE_OFFSET = 12;
    static const int JUMP_OPC_MASK = 0b11100000000000;
    static const int BIT_OPC_MASK = 0b11110000000000;
    static const int BYTE_OPC_MASK = 0b11111100000000;
    static const int BIT_ADDRESS_MASK = 0b00001110000000;
    static const int FILE_ADDRESS_MASK = 0b00000001111111;
    static const int DESTINATION_BIT_MASK = 0b00000010000000;
    static const int LITERAL_11BIT_MASK = 0b00011111111111;
    static const int BIT_ADDRESS_OFFSET = 7;
    static const int DESTINATION_BIT_OFFSET = 7;

    Instruction Decoder::decode(int code) {
        int operationType = (code & OPERATION_TYPE_MASK) >> OPERATION_TYPE_OFFSET;

        switch (operationType) {
            case 0b00:
                return decodeByteOrControlOriented(code);
            case 0b01:
                return decodeBitOriented(code);
            case 0b10:
                return decodeJumpOriented(code);
            case 0b11:
                return decodeLiteralOriented(code);
            default:
                throw std::invalid_argument("Illegal operation type determined");
        }
    }

    Instruction Decoder::decodeByteOrControlOriented(int code) {
        // Distinguish between byte-oriented and control operations
        switch (code) {
            case 0x0064:
                return Instruction(Instruction::OperationCode::CLRWDT, {});
            case 0x0009:
                return Instruction(Instruction::OperationCode::RETFIE, {});
            case 0x0008:
                return Instruction(Instruction::OperationCode::RETURN, {});
            case 0x0063:
                return Instruction(Instruction::OperationCode::SLEEP, {});
            default:
                if ((code >> 7) == 2) { // CLRW
                    return Instruction(Instruction::OperationCode::CLRW, {});
                } else if ((code >> 7) == 0 && (code & 31) == 0) { // NOP
                    return Instruction(Instruction::OperationCode::NOP, {});
                } else if ((code >> 7) == 3) { // CLRF
                    int address = code & FILE_ADDRESS_MASK;
                    return Instruction(Instruction::OperationCode::CLRF, {address, 0});
                } else if ((code >> 7) == 1) { // MOVWF
                    int address = code & FILE_ADDRESS_MASK;
                    return Instruction(Instruction::OperationCode::MOVWF, {address, 0});
                } else {
                    int destination = (code & DESTINATION_BIT_MASK) >> DESTINATION_BIT_OFFSET;
                    int address = code & FILE_ADDRESS_MASK;

                    switch (code & BYTE_OPC_MASK) {
                        case 0x0700:
                            return Instruction(Instruction::OperationCode::ADDWF, {address, destination});
                        case 0x0500:
                            return Instruction(Instruction::OperationCode::ANDWF, {address, destination});
                        case 0x0900:
                            return Instruction(Instruction::OperationCode::COMF, {address, destination});
                        case 0x0300:
                            return Instruction(Instruction::OperationCode::DECF, {address, destination});
                        case 0x0B00:
                            return Instruction(Instruction::OperationCode::DECFSZ, {address, destination});
                        case 0x0A00:
                            return Instruction(Instruction::OperationCode::INCF, {address, destination});
                        case 0x0F00:
                            return Instruction(Instruction::OperationCode::INCFSZ, {address, destination});
                        case 0x0400:
                            return Instruction(Instruction::OperationCode::IORWF, {address, destination});
                        case 0x0800:
                            return Instruction(Instruction::OperationCode::MOVF, {address, destination});
                        case 0x0D00:
                            return Instruction(Instruction::OperationCode::RLF, {address, destination});
                        case 0x0C00:
                            return Instruction(Instruction::OperationCode::RRF, {address, destination});
                        case 0x0200:
                            return Instruction(Instruction::OperationCode::SUBWF, {address, destination});
                        case 0x0E00:
                            return Instruction(Instruction::OperationCode::SWAPF, {address, destination});
                        case 0x0600:
                            return Instruction(Instruction::OperationCode::XORWF, {address, destination});
                    }
                }
        }
        throw std::invalid_argument("Illegal operation type determined");
    }

    Instruction Decoder::decodeBitOriented(int code) {
        int bit = (code & BIT_ADDRESS_MASK) >> BIT_ADDRESS_OFFSET;
        int fileAddress = code & FILE_ADDRESS_MASK;

        switch (code & BIT_OPC_MASK) {
            case 0x1000:
                return Instruction(Instruction::OperationCode::BCF, {fileAddress, bit});
            case 0x1400:
                return Instruction(Instruction::OperationCode::BSF, {fileAddress,bit});
            case 0x1800:
                return Instruction(Instruction::OperationCode::BTFSC, {fileAddress, bit});
            case 0x1C00:
                return Instruction(Instruction::OperationCode::BTFSS, {fileAddress, bit});
        }
        throw std::invalid_argument("Illegal operation type determined");
    }

    Instruction Decoder::decodeJumpOriented(int code) {
        int address = code & LITERAL_11BIT_MASK;

        switch (code & JUMP_OPC_MASK) {
            case 0x2000:
                return Instruction(Instruction::OperationCode::CALL, {address});
            case 0x2800:
                return Instruction(Instruction::OperationCode::GOTO, {address});
        }
        throw std::invalid_argument("Illegal operation type determined");
    }

    Instruction Decoder::decodeLiteralOriented(int code) {
        int literal = code & LITERAL_8BIT_MASK;

        switch (code & LITERAL_OPC_MASK) {
            case 0x3F00:
            case 0x3E00:
                return Instruction(Instruction::OperationCode::ADDLW, {literal});
            case 0x3900:
                return Instruction(Instruction::OperationCode::ANDLW, {literal});
            case 0x3800:
                return Instruction(Instruction::OperationCode::IORLW, {literal});
            case 0x3300:
            case 0x3200:
            case 0x3100:
            case 0x3000:
                return Instruction(Instruction::OperationCode::MOVLW, {literal});
            case 0x3700:
            case 0x3600:
            case 0x3500:
            case 0x3400:
                return Instruction(Instruction::OperationCode::RETLW, {literal});
            case 0x3D00:
            case 0x3C00:
                return Instruction(Instruction::OperationCode::SUBLW, {literal});
            case 0x3A00:
                return Instruction(Instruction::OperationCode::XORLW, {literal});
        }
        throw std::invalid_argument("Illegal operation type determined");
    }


