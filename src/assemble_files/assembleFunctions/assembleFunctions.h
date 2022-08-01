#ifndef ARM11_23_ASSEMBLEFUNCTIONS_H
#define ARM11_23_ASSEMBLEFUNCTIONS_H

#include <stdint.h>
#include "../symbolTable/symbolTable.h"

uint32_t assembleDp(symbolTable symbolTable, instruction *instruction, ldrExpr_t *ldrExpr);
uint32_t assembleMul(symbolTable symbolTable, instruction *instruction ,ldrExpr_t *ldrExpr);
uint32_t assembleSdt(symbolTable symbolTable, instruction *instruction, ldrExpr_t *ldrExpr);
uint32_t assembleBranch(symbolTable symbolTable, instruction *instruction, ldrExpr_t *ldrExpr);
uint32_t assembleAndeq(symbolTable symbolTable, instruction *instruction, ldrExpr_t *ldrExpr);
uint32_t assembleLSL(symbolTable symbolTable, instruction *instruction, ldrExpr_t *ldrExpr);

#endif //ARM11_23_ASSEMBLEFUNCTIONS_H
