#include <stdbool.h>
#include "decode.h"
#include "constants.h"

void decode(ARM *arm, instruction *decodedInstruction, bool *endInstruction) {
    uint32_t currentInstruction = arm->fetchedInstruction.instruction;
    arm->currentlyDecoded.instruction = currentInstruction;
    // If necessary (i.e. for a branch instruction), clear the pipeline.
    if (arm->fetchedInstruction.instruction == 0) {
        return;
    }
    // Depending on which bits are set in the instruction, execute the relevant helper decode function.
    if (((currentInstruction & DECODE_BRANCH_MASK))) {
        *decodedInstruction = decodeBranch(currentInstruction);
    } else if (((currentInstruction & DECODE_SDT_MASK))) {
        *decodedInstruction = decodeSdt(currentInstruction);
    } else if (!((currentInstruction & DECODE_MULTIPLY_MASK1))
               && (((currentInstruction & DECODE_MULTIPLY_MASK2) >> 4)) == MULTIPLY_INSTR_CODE) {
        *decodedInstruction = decodeMultiply(currentInstruction);
    } else {
        if (currentInstruction == 0) {
            *endInstruction = true;
        }
        *decodedInstruction = decodeDataProcessing(currentInstruction);
    }
    arm->currentlyDecoded.isSet = true;
}

instruction decodeBranch(uint32_t currInstr) {
    instruction currentInstruction;
    currentInstruction.type = b;
    currentInstruction.cond = convertCond(((currInstr & DECODE_CONDITION_MASK) >> COND_SHIFT));
    currentInstruction.instructionType.branchInstruction.offset = currInstr & BRANCH_OFFSET_MASK;
    return currentInstruction;
}

instruction decodeSdt(uint32_t currInstr) {
    instruction currentInstruction;
    currentInstruction.type = sdt;
    currentInstruction.cond = convertCond((currInstr & DECODE_CONDITION_MASK) >> COND_SHIFT);
    currentInstruction.instructionType.sdtInstruction.I = (currInstr & SDT_IFLAG_MASK) >> I_SHIFT;
    currentInstruction.instructionType.sdtInstruction.P = (currInstr & SDT_PFLAG_MASK) >> P_SHIFT;
    currentInstruction.instructionType.sdtInstruction.U = (currInstr & SDT_UFLAG_MASK) >> U_SHIFT;
    currentInstruction.instructionType.sdtInstruction.L = (currInstr & SDT_LFLAG_MASK) >> L_SHIFT;
    currentInstruction.instructionType.sdtInstruction.rn = (currInstr & SDT_RN_MASK) >> RN_SHIFT;
    currentInstruction.instructionType.sdtInstruction.rd = (currInstr & SDT_RD_MASK) >> RD_SHIFT;
    currentInstruction.instructionType.sdtInstruction.offset = (currInstr & SDT_OFFSET_MASK);
    currentInstruction.instructionType.sdtInstruction.gpioAddr = currInstr;
    return currentInstruction;
}

instruction decodeDataProcessing(uint32_t currInstr) {
    instruction currentInstruction;
    currentInstruction.type = dp;
    currentInstruction.cond = convertCond((currInstr & DECODE_CONDITION_MASK) >> COND_SHIFT);
    currentInstruction.instructionType.dataProcessingInstruction.op = convertOpcodeDataProcessing((currInstr & DATAPROCESSING_OP_MASK) >> OP_SHIFT);
    currentInstruction.instructionType.dataProcessingInstruction.I = (currInstr & DATAPROCESSING_IFLAG_MASK) >> I_SHIFT;
    currentInstruction.instructionType.dataProcessingInstruction.S = (currInstr & DATAPROCESSING_SFLAG_MASK) >> L_SHIFT;
    currentInstruction.instructionType.dataProcessingInstruction.rn = (currInstr & DATAPROCESSING_RN_MASK) >> RN_SHIFT;
    currentInstruction.instructionType.dataProcessingInstruction.rd = (currInstr & DATAPROCESSING_RD_MASK) >> RD_SHIFT  ;
    currentInstruction.instructionType.dataProcessingInstruction.op2 = (currInstr & DATAPROCESSING_OP2_MASK);
    return currentInstruction;
}

instruction decodeMultiply(uint32_t currInstr) {
    instruction currentInstruction;
    currentInstruction.type = m;
    currentInstruction.cond = convertCond((currInstr & DECODE_CONDITION_MASK) >> COND_SHIFT);
    currentInstruction.instructionType.multiplyInstruction.A = (currInstr & MULTIPLY_AFLAG_MASK) >> OP_SHIFT;
    currentInstruction.instructionType.multiplyInstruction.S = (currInstr & MULTIPLY_SFLAG_MASK) >> L_SHIFT;
    currentInstruction.instructionType.multiplyInstruction.rd = (currInstr & MULTIPLY_RD_MASK) >> RN_SHIFT;
    currentInstruction.instructionType.multiplyInstruction.rn = (currInstr & MULTIPLY_RN_MASK) >> RD_SHIFT;
    currentInstruction.instructionType.multiplyInstruction.rs = (currInstr & MULTIPLY_RS_MASK) >> MUL_SHIFT;
    currentInstruction.instructionType.multiplyInstruction.rm = (currInstr & MULTIPLY_RM_MASK);
    return currentInstruction;
}

enum condition convertCond(uint32_t cond) {
    enum condition ret;
    switch (cond) {
        case 0:
            ret = eq;
            break;
        case 1:
            ret = ne;
            break;
        case 10:
            ret = ge;
            break;
        case 11:
            ret = lt;
            break;
        case 12:
            ret = gt;
            break;
        case 13:
            ret = le;
            break;
        case 14:
            ret = al;
            break;
        default:
            ret = -1;
    }
    return ret;
}


enum opcode convertOpcodeDataProcessing(uint32_t cond) {
    enum opcode ret;
    switch (cond) {
        case 0:
            ret = and;
            break;
        case 1:
            ret = eor;
            break;
        case 2:
            ret = sub;
            break;
        case 3:
            ret = rsb;
            break;
        case 4:
            ret = add;
            break;
        case 8:
            ret = tst;
            break;
        case 9:
            ret = teq;
            break;
        case 10:
            ret = cmp;
            break;
        case 12:
            ret = orr;
            break;
        case 13:
            ret = mov;
            break;
        default:
            ret = -1;
    }
    return ret;
}
