#ifndef DECODE_H
#define DECODE_H

#include "instruction.h"
#include "logger.h"

class Decoder {
public:
    Instruction decode(int code);
    Instruction decodeByteOrControlOriented(int code);
    Instruction decodeBitOriented(int code);
    Instruction decodeJumpOriented(int code);
    Instruction decodeLiteralOriented(int code);
};

#endif // DECODE_H
