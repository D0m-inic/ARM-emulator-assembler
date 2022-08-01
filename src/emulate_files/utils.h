#ifndef ARM11_23_UTILS_H
#define ARM11_23_UTILS_H

#include <stdint.h>
#include <stdbool.h>

#define SIZE_OF_MEMORY 65538 * 8

#define ERROR_CHECK_NULL(ptr, msg) { \
if (ptr == NULL) {             \
    perror(msg);               \
    exit(EXIT_FAILURE);        \
    }                          \
    }

#define ERROR_CHECK_NONZERO(code, msg) { \
if (code != 0) {             \
    perror(msg);               \
    exit(EXIT_FAILURE);        \
    }                          \
    }

#define ERROR_CHECK_MINUSONE(code, msg) { \
if (code == -1) {             \
    perror(msg);               \
    exit(EXIT_FAILURE);        \
    }                          \
    }


typedef struct reg {
    int numReg;
    uint32_t value;
} reg;

typedef struct {
    bool isSet;
    uint32_t instruction;
} currentlyDecoded;

typedef struct {
    bool isSet;
    uint32_t instruction;
} currentlyFetched;

typedef struct gpioMemory {
    uint32_t bits0to9, bits10to19, bits20to29, setGPIO, clearGPIO;
} gpioMemory;


typedef struct ARM {
    uint32_t mem[65536 * 8];
    reg reg0;
    reg reg1;
    reg reg2;
    reg reg3;
    reg reg4;
    reg reg5;
    reg reg6;
    reg reg7;
    reg reg8;
    reg reg9;
    reg reg10;
    reg reg11;
    reg reg12;
    reg pc; // reg 15
    bool cpsr[32]; // reg 16
    currentlyFetched fetchedInstruction;
    currentlyDecoded currentlyDecoded;
    gpioMemory gpMem;
} ARM;

enum condition {eq, ne, ge, lt , gt , le, al};

enum instrType {dp, m, sdt, b};

enum opcode {and, eor, sub, rsb, add, tst, teq, cmp, orr, mov};

typedef struct dataProcessingInstruction {
    enum opcode op;
    bool I, S;
    uint32_t rn, rd, op2;
} dataProcessingInstruction;

typedef struct multiplyInstruction {
    bool A, S;
    uint32_t rd, rn, rs, rm;
} multiplyInstruction;

typedef struct sdtInstruction {
    bool I, P, U, L;
    uint32_t rn, rd, offset, gpioAddr;
} sdtInstruction;

typedef struct branchInstruction {
    uint32_t offset;
} branchInstruction;


typedef struct instruction {
    enum condition cond;
    enum instrType type;
    union instructionType {
        dataProcessingInstruction dataProcessingInstruction;
        multiplyInstruction multiplyInstruction;
        sdtInstruction sdtInstruction;
        branchInstruction branchInstruction;
        //endInstruction endInstruction;
    } instructionType;
    bool isSet;
} instruction;

extern reg * numToReg( ARM * arm, uint32_t i );
#endif //ARM11_23_UTILS_H
