#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"
#include "../assemble_constants.h"

#define MAX_TABLE_SIZE 100 //CHANGE

symbol *initialiseSymbol(void) {
    symbol *newSymbol = malloc(sizeof(symbol));
    return newSymbol;
}

symbol **initialiseSymbols (int num) {
    symbol **symbols = malloc(num * sizeof(symbol*));
    return symbols;
}

symbolTable *createTable(void) {
    symbolTable *newTable = malloc(sizeof(symbolTable *));
    newTable->symbols = initialiseSymbols(MAX_TABLE_SIZE);
    newTable->currentSymbol = 0;
    newTable->maxNumOfSymbols = MAX_TABLE_SIZE;
    return newTable;

}

void addSymbol (symbolTable *table, symbol *symbol) {
    if (table->currentSymbol >= table->maxNumOfSymbols) {
        perror("Table is full");
        exit(EXIT_FAILURE);
    }
    table->symbols[table->currentSymbol] = symbol;
    table->currentSymbol++;
}

void addSymbols(symbolTable *table, symbol symbols[], int length) {
    for (int i=0; i<length; i++) {
        if (table->currentSymbol >= table->maxNumOfSymbols) {
            perror("Table is full");
            exit(EXIT_FAILURE);
        }
        table->symbols[table->currentSymbol] = &(symbols[i]);
        table->currentSymbol++;
    }
}

symbol *getSymbol (symbolTable table, char *name) {
    for (int i=0; i<table.currentSymbol; i++) {
        if (strcmp(name, table.symbols[i]->name) == 0) {
            return table.symbols[i];
        }
    }
    printf("Not in symbol table"); //delete
    return NULL;
}

uint32_t enumToOpcode(mnemonic mnem) {
    switch (mnem) {
        case ADD:
            return ADD_OPCODE_MASK;
        case SUB:
            return SUB_OPCODE_MASK;
        case RSB:
            return RSB_OPCODE_MASK;
        case AND:
            return AND_OPCODE_MASK;
        case EOR:
            return EOR_OPCODE_MASK;
        case ORR:
            return ORR_OPCODE_MASK;
        case MOV:
            return MOV_OPCODE_MASK;
        case TST:
            return TST_OPCODE_MASK;
        case TEQ:
            return TEQ_OPCODE_MASK;
        case CMP:
            return CMP_OPCODE_MASK;
        default:
            return -1;
    }
}

void freeSymbolTable(symbolTable *symbolTable, int labelCounter) {
    for (int i = symbolTable->currentSymbol - labelCounter; symbolTable->symbols[i]; i++) {
        free(symbolTable->symbols[i]->name);
        free(symbolTable->symbols[i]);
    }
    free(symbolTable->symbols);
    free(symbolTable);
}

void freeLdrExpr(ldrExpr_t *ldrExpr) {
    free(ldrExpr);
}
