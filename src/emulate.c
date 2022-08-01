#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "emulate_files/decode.h"
#include "emulate_files/execute.h"
#include "emulate_files/fetch.h"
#include "emulate_files/utils.h"
#include "emulate_files/constants.h"
#include "emulate_files/printOutput.h"

int main(int argc, char **argv) {
    ARM arm;
    char *fileName = argv[1];
    initialiseArm(&arm, fileName);

    //Fetch decode execute pipeline
    instruction decodedInstruction;
    arm.pc.value = 0;

    bool endInstruction = false;
    while (((arm.currentlyDecoded.isSet && arm.currentlyDecoded.instruction) || !arm.currentlyDecoded.isSet) && !endInstruction) {
        execute(&arm, decodedInstruction);
        decode(&arm, &decodedInstruction, &endInstruction);
        fetch(&arm);

    }

    printState(arm);

    return EXIT_SUCCESS;
}

reg *numToReg(ARM *arm, uint32_t i){
    // a function that helps to pick the right register (pointer to it) with a number input
    switch(i){
        case 0:
            return &(arm->reg0);
        case 1:
            return &(arm->reg1);
        case 2:
            return &(arm->reg2);
        case 3:
            return &(arm->reg3);
        case 4:
            return &(arm->reg4);
        case 5:
            return &(arm->reg5);
        case 6:
            return &(arm->reg6);
        case 7:
            return &(arm->reg7);
        case 8:
            return &(arm->reg8);
        case 9:
            return &(arm->reg9);
        case 10:
            return &(arm->reg10);
        case 11:
            return &(arm->reg11);
        case 12:
            return &(arm->reg12);
        case 15:
            return &(arm->pc);
        default:
            return NULL;
    }
}















