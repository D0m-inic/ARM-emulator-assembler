#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../symbolTable/symbolTable.h"
#include "twoPassAssembly.h"
#include "../assemble_constants.h"

symbolTable* firstPass(symbolTable *symbolTable, fileLines_t *fileLines) {
    int labelCounter = 0;
    for (int i=0; fileLines->lines[i]; i++) {
        if (strchr(fileLines->lines[i], ':')) {
            char *name = calloc(MAX_INSTRUCTION_NAME_SIZE, sizeof(char));
            for (int j = 0; fileLines->lines[i][j] != ':'; j++){
                name[j] = fileLines->lines[i][j];
            }
            symbol *newSymbol = initialiseSymbol();
            newSymbol->name = name;
            newSymbol->type = LABEL;
            newSymbol->data.address = (i - labelCounter) * ADDRESS_CONVERSION;
            addSymbol(symbolTable, newSymbol);
            labelCounter++;
        }
    }
    return symbolTable;
}

void secondPass(symbolTable *symbolTable, fileLines_t *fileLines, FILE *binFile, ldrExpr_t *ldrExpr) {
    int labelCounter = 0;
    for (int i = 0; fileLines->lines[i]; i++) {
        if (strcmp(fileLines->lines[i], "") == 0) { //If the line is empty do nothing
            continue;
        }
        /*
        If the line isn't a label, extract the instruction name from the line and return from the symbol table the corresponding symobol. Then call the corresponding assemble function
        from that symbol on that line.
        */
        if (!(strchr(fileLines->lines[i], ':'))) {
            int pos;
            for (pos = 0; fileLines->lines[i][pos] != ' '; pos++);
            char *name = calloc(pos, sizeof(char));
            strncpy(name, fileLines->lines[i], pos);
            symbol *newSymbol = getSymbol(*symbolTable, name);
            instruction instruction;
            instruction.mnemonic = stringToMnemonic(name);
            instruction.opcode = enumToOpcode(instruction.mnemonic);
            instruction.line = fileLines->lines[i];
            instruction.address = (i - labelCounter) * 4;
            uint32_t data = newSymbol->data.assembleInstruction(*symbolTable, &instruction, ldrExpr);
            fileWriter(binFile, &data);
            free(name);
        } else {
            labelCounter++;
        }
    }
}
