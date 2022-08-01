#ifndef ARM11_23_DECODE_H
#define ARM11_23_DECODE_H

#include <stdbool.h>
#include <stdlib.h>
#include "utils.h"

extern void decode(ARM* arm, instruction* decodedInstruction, bool *endInstruction);
extern instruction decodeBranch( uint32_t currInstr );
extern instruction decodeSdt( uint32_t currInstr );
extern instruction decodeDataProcessing( uint32_t currInstr );
extern instruction decodeMultiply( uint32_t currInstr );
enum opcode convertOpcodeDataProcessing(uint32_t cond);
enum condition convertCond(uint32_t cond);
#endif //ARM11_23_DECODE_H
