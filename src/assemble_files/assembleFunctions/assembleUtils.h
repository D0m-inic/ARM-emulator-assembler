#ifndef ARM11_23_ASSEMBLEUTILS_H
#define ARM11_23_ASSEMBLEUTILS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    NUMERICCONSTANT, PREINDEXED, POSTINDEXED
} addressType;

typedef struct sdtAddress {
    addressType type;
    uint32_t address;
    int offset;
    bool immediate;
} sdtAddress;

uint32_t firstRegister(const char *line);
uint32_t secondRegister(const char *line);
char* operand2(char *line);
int numExprToInt(char *numConstant);
uint32_t regToNum(char *reg);
uint32_t roundDownToNearest4(uint32_t input);
uint32_t firstSetBit(uint32_t input);
uint32_t lastSetBit(uint32_t input);
uint32_t thirdRegister(const char *line);
sdtAddress *decodeAddress(char *line);
int convertToDecimal(int num);
bool exprFitsIntoMovArg(char *expr);

#endif //ARM11_23_ASSEMBLEUTILS_H
