#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "assembleUtils.h"
#include "../assemble_constants.h"

uint32_t firstRegister(const char *line) { //Returns first register from line
    char reg[MAX_REG_NAME_SIZE];
    for (int i = STANDARD_INSTRUCTION_SIZE; line[i] != ','; i++) {
        reg[i - STANDARD_INSTRUCTION_SIZE] = line[i];
    }
    int i;
    for (i = 0; reg[i] == ' '; i++);
    if (reg[0] == ' ') {
        for (int j = 0; j < sizeof(reg); j++) {
            if (i + j < sizeof(reg)) {
                reg[j] = reg[i + j];
                reg[i + j] = '\0';
            }
        }
    }
    return regToNum(reg);
}

uint32_t secondRegister(const char *line) { //Returns second register from line
    int indexFirstComma;
    int indexSecondComma;
    for (indexFirstComma = 0; line[indexFirstComma] != ','; indexFirstComma++);
    char *secondReg = calloc(MAX_REG_NAME_SIZE, sizeof(char));
    // now we have to go from indexFirstComma to the next comma
    for (indexSecondComma = indexFirstComma + 1; line[indexSecondComma] != ','; indexSecondComma++) {
        secondReg[indexSecondComma - (indexFirstComma + 1)] = line[indexSecondComma];
    }
    uint32_t num = regToNum(secondReg);
    free(secondReg);
    return num;
}

uint32_t thirdRegister(const char *line){ //Returns third register from line
    int commaCnt = 0;
    int i;
    char thirdReg[MAX_REG_NAME_SIZE];
    for (i = 0; line[i] != '\0' && commaCnt != 2; i++){
        if (line[i] == ','){
            commaCnt++;
        }
    }
    // i holds pos of second comma
    int j;
    for (j = i; line[j] != ','; j++){
        thirdReg[j - i] = line[j];
    }
    return regToNum(thirdReg);
}

char* operand2(char *line) { //Returns everything after last comma in line
    int pos = 0;
    int i;
    for (i = 0; line[i] != '\0'; i++)  {
        if (line[i] == ','){
            pos = i;
        }
    }
    return line + pos + 1;
}

uint32_t firstSetBit(uint32_t input) {
    uint32_t mask = 0;
    for (int i = 0; i < NUM_BITS_FOUR_BYTES; i++) {
        mask = pow(2, i);
        if (input & mask) {
            return i;
        }
    }
    return 31;
}

uint32_t lastSetBit(uint32_t input) {
    uint32_t mask = 0;
    for (int i = NUM_BITS_FOUR_BYTES - 1; i >= 0; i--) {
        mask = pow(2, i);
        if (input & mask) {
            return i;
        }
    }
    return 0;
}

uint32_t roundDownToNearest4(uint32_t input) {
//    return input - (input % 4);
    switch(input % 4) {
        case 0:
            return input;
        case 1:
            return input - 1;
        case 2:
            return input - 2;
        case 3:
            return input - 3;
    }
    return input;
}

int convertToDecimal(int num) {
    int result = 0;
    int counter = 0;
    int adjustment = 1;
    if (num < 0) {
        adjustment = -1;
        num *= -1;
    }
    while (num != 0) {
        result += (num % 10) * pow(16, counter);
        num /= 10;
        counter++;
    }
    return adjustment * result;
}

sdtAddress *decodeAddress(char *line){
    sdtAddress *sdtAddr = malloc(sizeof (sdtAddress));
    sdtAddr->immediate = false;
    sdtAddr->offset = 0;
    int i;
    for (i = 0; line[i] != ','; i++);
    i++; // now i holds the address at the start of the address.
    if (line[i] == ' ') {
        i++;
    }
    if (line[i] == '='){
        // we have a numeric constant
        char *numExpr = malloc(sizeof(char) * strlen(line)); // change the size! too big
        for (int j = i + 1; line[i] != '\0'; i++){
            numExpr[j - (i + 1)] = line[j];
        }
        uint32_t num = numExprToInt(numExpr);
        free(numExpr);
        sdtAddr->type = NUMERICCONSTANT;
        sdtAddr->address = num;
        return sdtAddr;
    }else{
        // so now line[i] holds a '['
        int j;
        for (j = i; line[j] != '\0'; j++){
            if (line[j] == ']' || line[j] == ','){
                break;
            }
        }
        if (line[j] == ']'){
            // pre index with no offset OR a post index
            if (line[j+1] == ','){
                // post ind
                sdtAddr->type = POSTINDEXED;
                sdtAddr->immediate = true;
                sdtAddr->offset = regToNum(&line[j+2]);
            }else{
                // pre index with no offset
                sdtAddr->type = PREINDEXED;
            }
        }else{
            //line[j] == ',', pre index with an offset
            sdtAddr->type = PREINDEXED;
            char offset[strlen(line)];
            for (int m = j + 1; line[m] != ']'; m++){
                offset[m - (j + 1)] = line[m];
            }
            if (offset[0] == ' ') {
                for (int k = 0; k < sizeof(offset) - 1; k++) {
                    offset[k] = offset[k + 1];
                }
            }
            if (offset[0] == 'r') {
                sdtAddr->immediate = true;
            }
            sdtAddr->offset = numExprToInt(offset);
        }
        char *reg = calloc(3, sizeof(char));
        for (int k = i + 1; k < j; k++){
            reg[k - (i + 1)] = line[k];
        }
        sdtAddr->address = regToNum(reg);
        free(reg);
        return sdtAddr;
    }
}

bool exprFitsIntoMovArg(char *expr) {
    if (expr[0] != '#' && expr[0] != '='){
        return false;
    }
    if (numExprToInt(expr) <= 0xFF) {
        return true; // use a mov instruction
    } else {
        // cannot use a mov instruction.
        return false;
    }
}

int numExprToInt(char *numConstant) {
    if (numConstant[0] == 'r') {
        return regToNum(numConstant);
    }
    if (numConstant[1] == '-') {
        // coming into this part it is assumed that the first character in the array is a HASH or =
        if (numConstant[3] == 'x') {
            char* hex = numConstant + 4;
            int length = strlen(hex);
            int ret = 1;
            int decimal = 0;
            for (int i = length--; i >= 0; i--){
                if(hex[i] >= '0' && hex[i] <= '9')
                {
                    decimal += (hex[i] - 48) * ret;
                    ret *= 16;
                }
                else if(hex[i] >= 'A' && hex[i] <= 'F')
                {
                    decimal += (hex[i] - 55) * ret;
                    ret *= 16;
                }
                else if(hex[i] >= 'a' && hex[i] <= 'f')
                {
                    decimal += (hex[i] - 87) * ret;
                    ret *= 16;
                }
            }
            // the number is in hex. we will have to convert it to decimal.
            return decimal;
        } else {
            char* pointer = numConstant + 1;
            return atoi(pointer);
        }
    } else {
        // coming into this part it is assumed that the first character in the array is a HASH or =
        if (numConstant[2] == 'x' ) {
            char* hex = numConstant + 3;
            int length = strlen(hex);
            int ret = 1;
            int decimal = 0;
            for (int i = length--; i >= 0; i--){
                if(hex[i] >= '0' && hex[i] <= '9')
                {
                    decimal += (hex[i] - 48) * ret;
                    ret *= 16;
                }
                else if(hex[i] >= 'A' && hex[i] <= 'F')
                {
                    decimal += (hex[i] - 55) * ret;
                    ret *= 16;
                }
                else if(hex[i] >= 'a' && hex[i] <= 'f')
                {
                    decimal += (hex[i] - 87) * ret;
                    ret *= 16;
                }
            }
            // the number is in hex. we will have to convert it to decimal.
            return decimal;
        } else {
            char* pointer = numConstant + 1;
            return atoi(pointer);
        }
    }
}

uint32_t regToNum(char *reg) {
    return atoi(reg + 1); // + 1 to drop the 'r'
}


