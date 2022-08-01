#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../symbolTable/symbolTable.h"

ldrExpr_t *initialiseLdrExpr(uint32_t next) {
    ldrExpr_t *ldrExpr = malloc(sizeof(ldrExpr));
    ldrExpr->curr = 0;
    ldrExpr->nextPos = next;
    return ldrExpr;
}

mnemonic stringToMnemonic(char *string) {
    if (strcmp("add", string) == 0) {
        return ADD;
    } else if (strcmp("sub", string) == 0) {
        return SUB;
    } else if (strcmp("rsb", string) == 0) {
        return RSB;
    } else if (strcmp("and", string) == 0) {
        return AND;
    } else if (strcmp("eor", string) == 0) {
        return EOR;
    } else if (strcmp("orr", string) == 0) {
        return ORR;
    } else if (strcmp("mov", string) == 0) {
        return MOV;
    } else if (strcmp("tst", string) == 0) {
        return TST;
    } else if (strcmp("teq", string) == 0) {
        return TEQ;
    } else if (strcmp("cmp", string) == 0) {
        return CMP;
    } else if (strcmp("mul", string) == 0) {
        return MUL;
    } else if (strcmp("mla", string) == 0) {
        return MLA;
    } else if (strcmp("ldr", string) == 0) {
        return LDR;
    } else if (strcmp("str", string) == 0) {
        return STR;
    } else if (strcmp("beq", string) == 0) {
        return BEQ;
    } else if (strcmp("bne", string) == 0) {
        return BNE;
    } else if (strcmp("bge", string) == 0) {
        return BGE;
    } else if (strcmp("blt", string) == 0) {
        return BLT;
    } else if (strcmp("bgt", string) == 0) {
        return BGT;
    } else if (strcmp("ble", string) == 0) {
        return BLE;
    } else if (strcmp("b", string) == 0) {
        return B;
    } else if (strcmp("lsl", string) == 0) {
        return LSL;
    }
    return ANDEQ;
}
