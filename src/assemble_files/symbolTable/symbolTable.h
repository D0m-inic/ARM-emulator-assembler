#ifndef ARM11_23_SYMBOLTABLE_H
#define ARM11_23_SYMBOLTABLE_H

#include <stdint.h>

#define MAX_FILE_LINE_LENGTH 16

typedef enum {LABEL, INSTRUCTION} symbolType;
typedef enum {ADD, SUB, RSB, AND, EOR, ORR, MOV, TST, TEQ, CMP, MUL, MLA, LDR, STR, BEQ, BNE, BGE, BLT, BGT, BLE, B, LSL, ANDEQ} mnemonic;

typedef struct instruction {
    uint32_t opcode;
    char* line;
    mnemonic mnemonic;
    uint32_t address;
} instruction;

typedef struct ldrExpr {
    uint32_t curr;
    uint32_t valuesToWrite[MAX_FILE_LINE_LENGTH];
    uint32_t nextPos;
} ldrExpr_t;

typedef struct symbolTable symbolTable;

typedef struct symbol {
    char *name;
    symbolType type;
    union {
        uint32_t address; //FOR LABEL
        uint32_t (*assembleInstruction) (symbolTable, instruction*, ldrExpr_t*); //function pointer to assembly function - eg. assembleDp, assembleMul
    } data;
} symbol;

struct symbolTable {
    symbol **symbols; //Array of symbol pointers
    int maxNumOfSymbols, currentSymbol;
};

symbolTable *createTable(void);
void addSymbols(symbolTable *table, symbol symbols[], int length);
symbol *initialiseSymbol(void);
instruction *initialiseInstruction(void);
void addSymbol (symbolTable *table, symbol *symbol);
void freeSymbolTable(symbolTable *symbolTable, int labelCounter);
void freeLdrExpr(ldrExpr_t *ldrExpr);
symbol *getSymbol (symbolTable table, char *name);
uint32_t enumToOpcode(mnemonic mnem);

#endif //ARM11_23_SYMBOLTABLE_H
