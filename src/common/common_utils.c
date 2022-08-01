#include "common_utils.h"
#include "../emulate_files/constants.h"
#include <math.h>

// This function reverses the order of bytes in a 32-bit number.

uint32_t convertFormat(uint32_t registerValue) {
    uint32_t newValue = 0;
    for (int i = SIZE_INSTRUCTION_BYTES; i > 0; i--) {
        uint32_t bitMask = 0;
        for (int j = i * SIZE_INSTRUCTION_NIBBLES - 1; j >= (i - 1) * SIZE_INSTRUCTION_NIBBLES; j--) {
            bitMask += pow(2, j);
        }
        uint32_t tempValue = bitMask & registerValue;
        tempValue >>= (i - 1) * SIZE_INSTRUCTION_NIBBLES;
        newValue += tempValue * pow(16, 2 * (SIZE_INSTRUCTION_BYTES - i));
    }
    return newValue;
}
