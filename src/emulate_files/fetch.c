#include <stdio.h>
#include <stdlib.h>
#include "fetch.h"
#include "utils.h"
#include "constants.h"


long fileSize(const char* filename){ // this function returns the file size in BYTES
    long size;
    FILE *f;
    f = fopen(filename, "rb");
    ERROR_CHECK_NULL(f, "error opening file");
    ERROR_CHECK_MINUSONE(fseek(f, 0, SEEK_END), "error in finding size of file");
    size = ftell(f);
    ERROR_CHECK_MINUSONE(size, "error in finding size of file");
    ERROR_CHECK_NONZERO(fclose(f), "error in closing file");
    return size;
}


void loadFileToMemory(ARM* arm, char* fileName) {
    FILE *file;
    file = fopen(fileName, "rb");
    ERROR_CHECK_NULL(file, "error opening file");
    long filesize;
    filesize = fileSize(fileName); // file size in bytes
    long numInstructions = filesize / MEM_CONVERSION; // this is the number of 4 byte (ie 32 bit) chunks

    /* first arg is where we store, so we store in mem
     * second arg is size of each element to be read in BYTES (4 bytes = 32 bit, each instruction is 32 bit)
     * third arg is num of elements, which is just total no of bytes divided by 4
     * 4th arg is where we read from
     * */

    int error = fread(arm->mem, SIZE_INSTRUCTION_BYTES, numInstructions, file);
    (void) error;

    /* We shift all instructions stored in the array representing memory left by 2 to create space for storing the
     * number of instructions and the next available address in memory for storage.
     * */

    for (long i = numInstructions - 1; i >= 0; i--){
        arm->mem[i + RESERVED_MEMORY] = arm->mem[i];
    }

    arm->mem[0] = numInstructions + MEMORY_INSTRUCTION_OFFSET;
    /* Initially, only instructions have been stored in memory, so the next available address in memory can be calculated
     * directly from the number of instructions.
     * */
    arm->mem[1] = numInstructions + MEMORY_INSTRUCTION_OFFSET;
}

void initialiseArm(ARM *arm, char *fileName) {
    // Initialise memory
    loadFileToMemory(arm, fileName);

    // Initialise registers
    for (int i = 0; i < NUM_REGISTERS; i++) {
        // SP and LR registers ignored.
        if (!((i == 13) || (i == 14))) {
            reg *r = numToReg(arm, i);
            // numReg will be used later to help handle special cases for certain registers.
            r->numReg = i;
            r->value = 0;
        }
    }
    for (int i = 0; i < INSTRUCTION_SIZE; i++) {
        arm->cpsr[i] = false;
    }
    arm->gpMem.bits0to9 = arm->gpMem.bits10to19 = arm->gpMem.bits20to29 = arm->gpMem.setGPIO = arm->gpMem.clearGPIO = 0;
}

void fetch(ARM* arm) {
    /* When we fetch an instruction, we need to check if it's the all-zero instruction, so we store the binary
     * representation of the instruction to be checked while decoding. */
    arm->fetchedInstruction.instruction = arm->mem[(arm->pc.value) + 2];
    arm->pc.value++;
}
