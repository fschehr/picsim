#include <iostream>
#include <stdexcept>
#include <cstdint>

class Logger {
public:
    static void info(const std::string& message) {
        std::cout << "INFO: " << message << std::endl;
    }
};

class Instruction {
public:
    enum class OperationCode {
        // operation codes 
        ADDWF,
        ANDWF,
        CLRF,
        CLRW,
        CLRWDT,
        COMF,
        DECF,
        DECFSZ,
        GOTO,
        IORWF,
        INCF,
        INCFSZ,
        MOVLW,
        MOVF,
        MOVWF,
        NOP,
        RLF,
        RRF,
        RETFIE,
        RETLW,
        RETURN,
        SLEEP,
        SUBWF,
        SWAPF,
        XORWF,
        BCF,
        BSF,
        BTFSC,
        BTFSS,
        CALL
    };

    Instruction(OperationCode opcode) : opcode_(opcode) {}

    Instruction(OperationCode opcode, int destination, int address) : opcode_(opcode), destination_(destination), address_(address) {}

    Instruction(OperationCode opcode, int literal) : opcode_(opcode), literal_(literal) {}

    Instruction(OperationCode opcode, int bitAddress, int fileAddress) : opcode_(opcode), bitAddress_(bitAddress), fileAddress_(fileAddress) {}

    OperationCode getOpcode() const { return opcode_; }

    int getDestination() const { return destination_; }

    int getAddress() const { return address_; }

    int getLiteral() const { return literal_; }

    int getBitAddress() const { return bitAddress_; }

    int getFileAddress() const { return fileAddress_; }

private:
    OperationCode opcode_;
    int destination_ = 0;
    int address_ = 0;
    int literal_ = 0;
    int bitAddress_ = 0;
    int fileAddress_ = 0;
};

class Decoder {
public:
    static const int LITERAL_8BIT_MASK =    0b00_0000_1111_1111;
    static const int LITERAL_OPC_MASK =     0b11_1111_0000_0000;
    static const int OPERATION_TYPE_MASK =  0b11_0000_0000_0000;
    static const int OPERATION_TYPE_OFFSET = 12;
    static const int JUMP_OPC_MASK = 0b11_1000_0000_0000;
    static const int BIT_OPC_MASK = 0b11_1100_0000_0000;
    static const int BYTE_OPC_MASK = 0b11_1111_0000_0000;
    static const int BIT_ADDRESS_MASK = 0b00_0011_1000_0000;
    static const int FILE_ADDRESS_MASK = 0b00_0000_0111_1111;
    static const int DESTINATION_BIT_MASK = 0b00_0000_1000_0000;
    static const int LITERAL_11BIT_MASK = 0b00_0111_1111_1111;
    static const int BIT_ADDRESS_OFFSET = 7;
    static const int DESTINATION_BIT_OFFSET = 7;

    Instruction decode(int code) {
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

    Instruction decodeByteOrControlOriented(int code) {
        // Distinguish between byte-oriented and control operations
        switch (code) {
            case 0x0064:
                return Instruction(Instruction::OperationCode::CLRWDT);
            case 0x0009:
                return Instruction(Instruction::OperationCode::RETFIE);
            case 0x0008:
                return Instruction(Instruction::OperationCode::RETURN);
            case 0x0063:
                return Instruction(Instruction::OperationCode::SLEEP);
            default:
                if ((code >> 7) == 2) { // CLRW
                    return Instruction(Instruction::OperationCode::CLRW);
                } else if ((code >> 7) == 0 && (code & 31) == 0) { // NOP
                    return Instruction(Instruction::Operation
                    } else if ((code >> 7) == 3) { // CLRF
                        int address = code & FILE_ADDRESS_MASK;
                        return Instruction(Instruction::OperationCode::CLRF, 0, address);
                    } else if ((code >> 7) == 1) { // MOVWF
                        int address = code & FILE_ADDRESS_MASK;
                        return Instruction(Instruction::OperationCode::MOVWF, 0, address);
                    } else {
                        int destination = (code & DESTINATION_BIT_MASK) >> DESTINATION_BIT_OFFSET;
                        int address = code & FILE_ADDRESS_MASK;
    
                        switch (code & BYTE_OPC_MASK) {
                            case 0x0700:
                                return Instruction(Instruction::OperationCode::ADDWF, destination, address);
                            case 0x0500:
                                return Instruction(Instruction::OperationCode::ANDWF, destination, address);
                            case 0x0900:
                                return Instruction(Instruction::OperationCode::COMF, destination, address);
                            case 0x0300:
                                return Instruction(Instruction::OperationCode::DECF, destination, address);
                            case 0x0B00:
                                return Instruction(Instruction::OperationCode::DECFSZ, destination, address);
                            case 0x0A00:
                                return Instruction(Instruction::OperationCode::INCF, destination, address);
                            case 0x0F00:
                                return Instruction(Instruction::OperationCode::INCFSZ, destination, address);
                            case 0x0400:
                                return Instruction(Instruction::OperationCode::IORWF, destination, address);
                            case 0x0800:
                                return Instruction(Instruction::OperationCode::MOVF, destination, address);
                            case 0x0D00:
                                return Instruction(Instruction::OperationCode::RLF, destination, address);
                            case 0x0C00:
                                return Instruction(Instruction::OperationCode::RRF, destination, address);
                            case 0x0200:
                                return Instruction(Instruction::OperationCode::SUBWF, destination, address);
                            case 0x0E00:
                                return Instruction(Instruction::OperationCode::SWAPF, destination, address);
                            case 0x0600:
                                return Instruction(Instruction::OperationCode::XORWF, destination, address);
                        }
                    }
            }
            throw std::invalid_argument("Illegal operation type determined");
        }
    
        Instruction decodeBitOriented(int code) {
            int bitAddress = (code & BIT_ADDRESS_MASK) >> BIT_ADDRESS_OFFSET;
            int fileAddress = code & FILE_ADDRESS_MASK;
    
            switch (code & BIT_OPC_MASK) {
                case 0x1000:
                    return Instruction(Instruction::OperationCode::BCF, bitAddress, fileAddress);
                case 0x1400:
                    return Instruction(Instruction::OperationCode::BSF, bitAddress, fileAddress);
                case 0x1800:
                    return Instruction(Instruction::OperationCode::BTFSC, bitAddress, fileAddress);
                case 0x1C00:
                    return Instruction(Instruction::OperationCode::BTFSS, bitAddress, fileAddress);
            }
            throw std::invalid_argument("Illegal operation type determined");
        }
    
        Instruction decodeJumpOriented(int code) {
            int address = code & LITERAL_11BIT_MASK;
    
            switch (code & JUMP_OPC_MASK) {
                case 0x2000:
                    return Instruction(Instruction::OperationCode::CALL, address);
                case 0x2800:
                    return Instruction(Instruction::OperationCode::GOTO, address);
            }
            throw std::invalid_argument("Illegal operation type determined");
        }
    
        Instruction decodeLiteralOriented(int code) {
            int literal = code & LITERAL_8BIT_MASK;
    
            switch (code & LITERAL_OPC_MASK) {
                case 0x3F00:
                case 0x3E00:
                    return Instruction(Instruction::OperationCode::ADDLW, literal);
                case 0x3900:
                    return Instruction(Instruction::OperationCode::ANDLW, literal);
                case 0x3800:
                    return Instruction(Instruction::OperationCode::IORLW, literal);
                case 0x3300:
                case 0x3200:
                case 0x3100:
                case 0x3000:
                    return Instruction(Instruction::OperationCode::MOVLW, literal);
                case 0x3700:
                case 0x3600:
                case 0x3500:
                case 0x3400:
                    return Instruction(Instruction::OperationCode::RETLW, literal);
                case 0x3D00:
                case 0x3C00:
                return Instruction(Instruction::OperationCode::SUBLW, literal);
                case 0x3A00:
                return Instruction(Instruction::OperationCode::XORLW, literal);
            }
            throw std::invalid_argument("Illegal operation type determined");
        }
    };
    
    int main() {
        Decoder decoder;
        int code = 0x0064; // Example code
        Instruction instruction = decoder.decode(code);
        std::cout << "Decoded instruction: " << static_cast<int>(instruction.getOpcode()) << std::endl;
        return 0;
    }
    
               
    