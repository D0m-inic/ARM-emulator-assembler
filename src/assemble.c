#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "assemble_files/symbolTable/symbolTable.h"
#include "common/common_utils.h"
#include "assemble_files/assemble_constants.h"
#include "assemble_files/assembleFunctions/assembleFunctions.h"
#include "assemble_files/fileIO.h"
#include "assemble_files/twoPassAssembly/twoPassAssembly.h"
#include "assemble_files/twoPassAssembly/twoPassAssemblyUtils.h"


int main(int argc, char **argv) {
    // initialise/define symbol table
    // enter in predefined symbols
    symbolTable *symbolTable = createTable();
    symbol predefined[NUM_PREDEFINED_SYMBOLS] = {
            {"add",   INSTRUCTION, .data.assembleInstruction = assembleDp},
            {"sub",   INSTRUCTION, .data.assembleInstruction = assembleDp},
            {"rsb",   INSTRUCTION, .data.assembleInstruction = assembleDp},
            {"and",   INSTRUCTION, .data.assembleInstruction = assembleDp},
            {"eor",   INSTRUCTION, .data.assembleInstruction = assembleDp},
            {"orr",   INSTRUCTION, .data.assembleInstruction = assembleDp},
            {"mov",   INSTRUCTION, .data.assembleInstruction = assembleDp},
            {"tst",   INSTRUCTION, .data.assembleInstruction = assembleDp},
            {"teq",   INSTRUCTION, .data.assembleInstruction = assembleDp},
            {"cmp",   INSTRUCTION, .data.assembleInstruction = assembleDp},
            {"mul",   INSTRUCTION, .data.assembleInstruction = assembleMul},
            {"mla",   INSTRUCTION, .data.assembleInstruction = assembleMul},
            {"ldr",   INSTRUCTION, .data.assembleInstruction = assembleSdt},
            {"str",   INSTRUCTION, .data.assembleInstruction = assembleSdt},
            {"beq",   INSTRUCTION, .data.assembleInstruction = assembleBranch},
            {"bne",   INSTRUCTION, .data.assembleInstruction = assembleBranch},
            {"bge",   INSTRUCTION, .data.assembleInstruction = assembleBranch},
            {"blt",   INSTRUCTION, .data.assembleInstruction = assembleBranch},
            {"bgt",   INSTRUCTION, .data.assembleInstruction = assembleBranch},
            {"ble",   INSTRUCTION, .data.assembleInstruction = assembleBranch},
            {"b",     INSTRUCTION, .data.assembleInstruction = assembleBranch},
            {"lsl",   INSTRUCTION, .data.assembleInstruction = assembleLSL},
            {"andeq", INSTRUCTION, .data.assembleInstruction = assembleAndeq}
    };
    addSymbols(symbolTable, predefined, 23);
    fileLines_t *fileLines = initialisefileLines();
    FILE *inputFile = fopen(argv[1], "r");
    convertFile(inputFile, fileLines);
    int labelCounter = 0;
    for (int i = 0; fileLines->lines[i]; i++) {
        if (strchr(fileLines->lines[i], ':')) {
            labelCounter++;
        }
    }
    ldrExpr_t *ldrExpr = initialiseLdrExpr(fileLines->currentLine - labelCounter);
    //read file and put contents of file into an array of file lines
    //close file
    fclose(inputFile);

    FILE *binFile = fopen(argv[2], "ab");
    symbolTable = firstPass(symbolTable, fileLines);
    secondPass(symbolTable, fileLines, binFile, ldrExpr);

    for (int i = 0; i < ldrExpr->curr; i++) {
        uint32_t data = ldrExpr->valuesToWrite[i]; //Write the additonal values from ldr instructions at the bottom of the file
        fileWriter(binFile, &data);
    }
    fclose(binFile);
    freeFileLines(fileLines);
    freeSymbolTable(symbolTable, labelCounter);
    freeLdrExpr(ldrExpr);

    //free memory in symbol table
    //make free functions within symbolTable
    return EXIT_SUCCESS;
}


