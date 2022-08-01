#ifndef ARM11_23_TWOPASSASSEMBLY_H
#define ARM11_23_TWOPASSASSEMBLY_H

#include <stdint.h>
#include <stdio.h>
#include "../symbolTable/symbolTable.h"
#include "../fileIO.h"

mnemonic stringToMnemonic(char *string);
symbolTable* firstPass(symbolTable *symbolTable, fileLines_t *fileLines);
void secondPass(symbolTable *symbolTable, fileLines_t *fileLines, FILE *binFile, ldrExpr_t *ldrExpr);


#endif //ARM11_23_TWOPASSASSEMBLY_H
