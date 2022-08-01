#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../symbolTable/symbolTable.h"
#include "../assemble_constants.h"
#include "assembleUtils.h"

uint32_t assembleDp(symbolTable symbolTable, instruction *instruction, ldrExpr_t *ldrExpr) {
    uint32_t res = 0;
    char *op2 = NULL;
    res |= DP_COND_MASK;
    uint32_t opcode = enumToOpcode(instruction->mnemonic) << DP_OPCODE;
    res |= opcode;
    switch (instruction->mnemonic) {
        case AND:
        case EOR:
        case SUB:
        case RSB:
        case ADD:
        case ORR: {
            uint32_t rd = firstRegister(instruction->line);
            res |= (rd << DP_RD);
            uint32_t rn = secondRegister(instruction->line);
            res |= (rn << DP_RN);
            op2 = operand2(instruction->line);
            res |= numExprToInt(op2);
            break;
        }
        case MOV: {
            uint32_t rd = firstRegister(instruction->line);
            res |= (rd << DP_RD);
            op2 = operand2(instruction->line);
            // Remove any leading spaces in the second operand.
            if (op2[0] == ' ') {
                op2++;
            }
            uint32_t temp = numExprToInt(op2);
            if (op2[0] == 'r') {
                uint32_t op2val = regToNum(op2);
                res |= op2val;
            } else if (temp > DP_IMMEDIATE_MASK) {
                // This case covers when the immediate input cannot be represented in 12 bits - in this case, we use shifts.
                uint32_t imm = 0;
                uint32_t rotate = 0;
                if (roundDownToNearest4(firstSetBit(temp)) != firstSetBit(temp)) {
                    uint32_t shiftAmount = roundDownToNearest4(firstSetBit(temp)) + lastSetBit(temp) % ADDRESS_CONVERSION + 1;
                    imm = temp << (NUM_BITS_FOUR_BYTES - shiftAmount) | temp >> (shiftAmount);
                    rotate = (NUM_BITS_FOUR_BYTES - (shiftAmount)) / 2;
                } else {
                    imm = temp << (NUM_BITS_FOUR_BYTES - (roundDownToNearest4(firstSetBit(temp)))) | temp >> (roundDownToNearest4(firstSetBit(temp)));
                    rotate = (NUM_BITS_FOUR_BYTES - (roundDownToNearest4(firstSetBit(temp)))) / 2;
                }
                res |= (imm & DP_IMMEDIATE_MASK);
                res |= (rotate << DP_ROTATE_SHIFT);
            } else {
                res |= temp;
            }
            break;
        }
        case TST:
        case TEQ:
        case CMP: {
            uint32_t rn = firstRegister(instruction->line);
            res |= (rn << DP_RN);
            op2 = operand2(instruction->line);
            res |= numExprToInt(op2);
            res |= DP_S_FLAG;
            break;
        }
        default: {
            res = -1;
            break;
        }
    }
    if (op2[0] != 'r') { //If operand2 doesn't begin with an 'r'
        res |= DP_I_FLAG;
    }
    return res;
}

uint32_t assembleMul(symbolTable symbolTable, instruction *instruction ,ldrExpr_t *ldrExpr) {
    uint32_t binary = 0;
    uint32_t rd = firstRegister(instruction->line);
    uint32_t rm = secondRegister(instruction->line);
    bool accumulate = instruction->line[2] == 'a';
    if (accumulate) {
        char* rn = operand2(instruction->line);
        binary |= (regToNum(rn) << MULTIPLY_RN_SHIFT);
        uint32_t rs = thirdRegister(instruction->line);
        binary |= rs << MULTIPLY_RS_SHIFT;
        binary |= MULTIPLY_A_MASK;
    } else {
        char* rs = operand2(instruction->line);
        binary |= (regToNum(rs) << MULTIPLY_RS_SHIFT);
    }
    binary |= DP_COND_MASK;
    binary |= MULTIPLY_BIT_MASK;
    binary |= (rd << MULTIPLY_RD_SHIFT);
    binary |= (rm);
    return binary;
}

uint32_t assembleSdt(symbolTable symbolTable, instruction *instruction, ldrExpr_t *ldrExpr) {
    uint32_t res = 0;
    res |= SDT_COND_MASK;
    if (!(strchr(instruction->line, '-'))) {
        res |= SDT_U_FLAG;
    }
    res |= SDT_SET_BIT;
    uint32_t rd = firstRegister(instruction->line);
    res |= (rd << SDT_RD_SHIFT);

    if (instruction->mnemonic == LDR) {
        //Set L bit
        res |= SDT_L_FLAG;
        // If the expression can be interpreted as a mov instruction...
        if (exprFitsIntoMovArg(operand2(instruction->line)) && !(strchr(instruction->line, '['))) {
            res = 0;
            instruction->mnemonic = MOV;
            res |= DP_COND_MASK;
            res |= enumToOpcode(instruction->mnemonic) << SDT_OPCODE_SHIFT;
            char *op2Equals = operand2(instruction->line);
            res |= numExprToInt(op2Equals);
            res |= DP_I_FLAG;
            return res;
        } else {
            char *exp = operand2(instruction->line);
            if (exp[0] == '=') {
                uint32_t op2 = numExprToInt(exp);
                uint32_t rn = SDT_PC; //Set to PC
                res |= (rn << SDT_RN_SHIFT);
                // Add to array of values to be written at bottom of output file
                ldrExpr->valuesToWrite[ldrExpr->curr] = op2;
                uint32_t offset = (ldrExpr->nextPos + ldrExpr->curr) * 4 - (instruction->address + 8);
                ldrExpr->curr++;
                res |= offset;
                res |= SDT_P_FLAG;
                return res;
            }
        }
    }
    //store
    sdtAddress *addr = decodeAddress(instruction->line);
    if (addr->type == PREINDEXED) {
        //Set P flag to 1
        res |= SDT_P_FLAG;
    }
    if (addr->immediate && instruction->mnemonic == STR) {
        res |= SDT_I_FLAG;
    }

    res |= addr->offset;
    res |= addr->address << SDT_RN_SHIFT;
    free(addr);
    return res;
}

uint32_t assembleBranch(symbolTable symbolTable, instruction *instruction, ldrExpr_t *ldrExpr) {
    uint32_t binary = 0;

    binary |= ALL_BRANCH_MASK;

    switch (instruction->line[1]) {
        case 'n':
            binary |= BNE_MASK;
            break;
        case 'e':
            break;
        case 'g':
            switch (instruction->line[2]) {
                case 'e':
                    binary |= BGE_MASK;
                    break;
                case 't':
                    binary |= BGT_MASK;
                    break;
            }
            break;
        case 'l':
            switch (instruction->line[2]) {
                case 'e':
                    binary |= BLE_MASK;
                    break;
                case 't':
                    binary |= BLT_MASK;
                    break;
            }
            break;
        case ' ':
            binary |= B_MASK;
            break;
    }

    int pos = 0;
    for (; instruction->line[pos] != ' '; pos++);

    char *target = instruction->line + pos + 1;

    int offset = 0;

    if (target[0] == '0' && target[1] == 'x') {
        target += 2;
        offset += convertToDecimal(atoi(target));
    } else {
        offset = (int) (getSymbol(symbolTable, target)->data.address) - (int) instruction->address - PC_PIPELINE_OFFSET;
    }
    offset >>= 2;
    binary += (offset & BRANCH_OFFSET_MASK);

    return binary;
}

uint32_t assembleAndeq(symbolTable symbolTable, instruction *instruction, ldrExpr_t *ldrExpr) {
    return 0x00000000u;
}

uint32_t assembleLSL(symbolTable symbolTable, instruction *instruction, ldrExpr_t *ldrExpr) {
    uint32_t result = 0;
    result |= DP_COND_MASK;
    result &= LSL_I_FLAG;
    uint32_t rn = firstRegister(instruction->line);
    uint32_t opcode = enumToOpcode(MOV) << LSL_OPCODE_SHIFT;
    result |= opcode;
    result |= (rn << LSL_RN_SHIFT);
    char *shiftAmount = operand2(instruction->line);
    uint32_t shiftAmountAsInt = numExprToInt(shiftAmount);
    shiftAmountAsInt <<= LSL_SHIFTAMOUNT_SHIFT;
    result |= shiftAmountAsInt;
    result |= rn;
    return result;
}


