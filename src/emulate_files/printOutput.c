#include <stdio.h>
#include <math.h>
#include "printOutput.h"
#include "utils.h"
#include "constants.h"
#include "../common/common_utils.h"

int log_10(uint32_t value) {
    int counter = 0;
    while ((value /= 10) != 0) {
        counter++;
    }
    return counter;
}

void printState(ARM arm) {
    // Outputting the state of the registers
    printf("Registers:\n");
    for (int i = 0; i < 13; i++) {
        /* We need to make sure the correct number of spaces are printed depending on the register number and the sign
         * of the register value. We also pad the register value with up to 8 zeroes. */
        if (((int) numToReg(&arm, i)->value) < 0) {
            printf("$%d%*c:%*c%d (0x%08x)\n", i, 2 - log_10(i), ' ', 9 - log_10((-1) * numToReg(&arm, i)->value), ' ', numToReg(&arm, i)->value, numToReg(&arm, i)->value);
        } else {
            printf("$%d%*c:%*c%d (0x%08x)\n", i, 2 - log_10(i), ' ', 10 - log_10(numToReg(&arm, i)->value), ' ', numToReg(&arm, i)->value, numToReg(&arm, i)->value);
        }
    }
    // We have to multiply the value of the PC by 4 since we only ever increment it by 1.
    printf("PC  :         %d (0x%08x)\n", numToReg(&arm, PC_REG)->value * MEM_CONVERSION, numToReg(&arm, PC_REG)->value * MEM_CONVERSION);
    if (((int) cpsrOutput(arm)) < 0) {
        printf("CPSR:%*c%d (0x%08x)\n", 9 - log_10((-1) * cpsrOutput(arm)), ' ', cpsrOutput(arm), cpsrOutput(arm));
    } else {
        printf("CPSR:%*c%d (0x%08x)\n", 10 - log_10(cpsrOutput(arm)), ' ', cpsrOutput(arm), cpsrOutput(arm));
    }

    uint32_t elementsPrinted = 0;
    printf("Non-zero memory:\n");
    // We can stop iterating through the memory array once all instructions and data stored in memory have been printed.
    for (int i = RESERVED_MEMORY; (i < (SIZE_OF_MEMORY)) && elementsPrinted < arm.mem[1] - RESERVED_MEMORY; i++) {
        if (arm.mem[i] != 0) {
            printf("0x%08x: 0x%08x\n", (i - RESERVED_MEMORY) * MEM_CONVERSION, convertFormat(arm.mem[i]));
            elementsPrinted++;
        } else {
            if (i < arm.mem[0] - 1) {
                elementsPrinted++;
            }
        }
    }
}

uint32_t cpsrOutput(ARM arm) {
    uint32_t intOutput = 0;
    for (int i = 31; i >= CPSR_END; i--) {
        intOutput += arm.cpsr[i] * pow(2, i);
    }
    return intOutput;
}
