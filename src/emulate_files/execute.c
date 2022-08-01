#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "execute.h"
#include "utils.h"
#include "constants.h"
#include "../common/common_utils.h"

bool condMet(ARM arm, enum condition cond){
    bool ret;
    switch(cond){
        case eq:
            ret = arm.cpsr[Z_FLAG];
            break;
        case ne:
            ret =  !arm.cpsr[Z_FLAG];
            break;
        case ge:
            ret =  arm.cpsr[N_FLAG] == arm.cpsr[V_FLAG];
            break;
        case lt:
            ret = arm.cpsr[N_FLAG] != arm.cpsr[V_FLAG];
            break;
        case gt:
            ret = !arm.cpsr[Z_FLAG] && (arm.cpsr[N_FLAG] == arm.cpsr[V_FLAG]);
            break;
        case le:
            ret = arm.cpsr[Z_FLAG] || arm.cpsr[N_FLAG] != arm.cpsr[V_FLAG];
            break;
        case al: // al
            return 1;
        default:
            return false;
    }
    return ret;
}

void execute(ARM *arm, instruction instr) {
    // If necessary (i.e. for a branch instruction), clear the pipeline.
    if (!arm->currentlyDecoded.isSet) {
        return;
    }
    // checking which bits are set based on condition
    if (condMet(*arm, instr.cond)) {
        switch (instr.type) {
            case dp:
                executeDP(arm, instr);
                break;
            case m:
                executeMultiply(arm, instr);
                break;
            case sdt:
                executeSDT(arm, instr);
                break;
            case b:
                executeBranch(arm, instr);
                break;
        }
    };
}

void executeMultiply(ARM *arm, instruction instr) {
    multiplyInstruction mul = instr.instructionType.multiplyInstruction;

    reg *rd = numToReg(arm, mul.rd);
    reg *rm = numToReg(arm, mul.rm);
    reg *rs = numToReg(arm, mul.rs);
    reg *rn = numToReg(arm, mul.rn);
    uint32_t *v = &((*rd).value);
    if (mul.A) {
        *v = (*rm).value * (*rs).value + (*rn).value;
    } else {
        *v = (*rm).value * (*rs).value;
    }
    if (mul.S) {
        // Update the N bit to be the 31st bit of the result.
        (*arm).cpsr[N_FLAG] = N_MASK & (*rd).value;
        // Set the Z bit iff the result is 0.
        if (*v == 0) {
            (*arm).cpsr[Z_FLAG] = 1;
        }
    }
}

uint32_t shiftedRegister(ARM *arm, uint32_t *shiftedreg, reg *rm) {

    uint32_t shiftAmount = (SHIFT_MASK & *shiftedreg) >> (BYTE_SIZE - 1);
    uint32_t shiftOperation = SHIFT_OPERATION_MASK & *shiftedreg;

    uint32_t highBit;
    uint32_t arithmeticMask;
    switch(shiftOperation) {
        case 0:
            *shiftedreg = rm->value << shiftAmount;
            break;
        case 1:
            *shiftedreg = rm->value >> shiftAmount;
            break;
        case 2:
            highBit = rm->value & N_MASK;
            arithmeticMask = 0;
            // If the sign bit is 1, we need to preserve it.
            if (highBit) {
                // The upper shiftAmount bits of arithmeticMask will be set to 1 if necessary.
                for (int i = 0; i < shiftAmount; i++) {
                    arithmeticMask += pow(2, (INSTRUCTION_SIZE - 1) - i);
                }
                *shiftedreg = rm->value >> shiftAmount | arithmeticMask;
            } else {
                *shiftedreg = rm->value >> shiftAmount;
            }
            break;
        case 3:
            /* Rotate right is the same as shifting right by a specified amount and if any of the bits lost were 1, this
             * can be restored using bitwise or and a shift of those lost bits to the upper end of the number. */
            *shiftedreg = rm->value >> shiftAmount | rm->value << (INSTRUCTION_SIZE - shiftAmount);
            break;
    }
    return *shiftedreg;
}

void executeDP(ARM *arm, instruction instr) {
    enum opcode op = instr.instructionType.dataProcessingInstruction.op;
    uint32_t op2 = instr.instructionType.dataProcessingInstruction.op2;
    reg *rn = numToReg(arm, instr.instructionType.dataProcessingInstruction.rn);
    reg *rd = numToReg(arm, instr.instructionType.dataProcessingInstruction.rd);
    bool I = instr.instructionType.dataProcessingInstruction.I;
    uint32_t carryBit = 0;
    if (I) {
        uint32_t immediateValue = op2 & IMM_MASK;
        uint32_t rotateAmount = ((op2 & ROTATE_MASK) * 2) >> BYTE_SIZE;

        op2 = immediateValue >> rotateAmount | immediateValue << (INSTRUCTION_SIZE - rotateAmount);

    } else {
        uint32_t shiftAmount = (SHIFT_MASK & op2) >> (BYTE_SIZE - 1);
        reg *rm = numToReg(arm, REGISTER_MASK & op2);
        uint32_t shiftOperation = SHIFT_OPERATION_MASK & op2;

        uint32_t highBit;
        uint32_t arithmeticMask;
        uint32_t last_shifted_bit_mask;
        switch(shiftOperation) {
            case 0:
                op2 = rm->value << shiftAmount;
                last_shifted_bit_mask = 1 << (INSTRUCTION_SIZE - shiftAmount + 1);
                carryBit = (rm->value & last_shifted_bit_mask) >> (INSTRUCTION_SIZE - shiftAmount + 1);
                break;
            case 1:
                op2 = rm->value >> shiftAmount;
                last_shifted_bit_mask = 1 << (shiftAmount - 1);
                carryBit = (rm->value & last_shifted_bit_mask) >> (shiftAmount - 1);
                break;
            case 2:
                highBit = rm->value & N_MASK;
                arithmeticMask = 0;
                if (highBit) {
                    for (int i = 0; i < shiftAmount; i++) {
                        arithmeticMask += pow(2, (INSTRUCTION_SIZE - 1) - i);
                    }
                    op2 = rm->value >> shiftAmount | arithmeticMask;
                } else {
                    op2 = rm->value >> shiftAmount;
                }
                last_shifted_bit_mask = 1 << (shiftAmount - 1);
                carryBit = (rm->value & last_shifted_bit_mask) >> (shiftAmount - 1);
                break;
            case 3:
                op2 = rm->value >> shiftAmount | rm->value << (INSTRUCTION_SIZE - shiftAmount);
                carryBit = op2 & N_MASK;
                carryBit >>= (INSTRUCTION_SIZE - 1);
                break;
        }
        if (shiftAmount == 0) {
            carryBit = 0;
        }
    }
    int compareResult;
    switch (op) {
        case and:
            rd->value = rn->value & op2;
            break;
        case eor:
            rd->value = rn->value ^ op2;
            break;
        case sub:
            rd->value = rn->value - op2;
            // If borrow is required...
            if (op2 > rn->value) {
                carryBit = 0;
            } else {
                carryBit = 1;
            }
            break;
        case rsb:
            rd->value = op2 - rn->value;
            // If borrow is required...
            if (rn->value > op2) {
                carryBit = 0;
            } else {
                carryBit = 1;
            }
            break;
        case add:
            rd->value = rn->value + op2;
            // If there is overflow...
            if ((rd->value < 0 && rn->value > 0 && op2 > 0) || (rd->value > 0 && rn->value < 0 && op2 < 0)) {
                carryBit = 1;
            } else {
                carryBit = 0;
            }
            break;
        case tst:
            compareResult = ((int) rn->value) & ((int) op2);
            break;
        case teq:
            compareResult = ((int) rn->value) ^ ((int) op2);
            break;
        case cmp:
            compareResult = ((int) rn->value) - ((int) op2);
            if (compareResult < 0) {
                carryBit = 0;
            } else {
                carryBit = 1;
            }
            break;
        case orr:
            rd->value = rn->value | op2;
            break;
        case mov:
            rd->value = op2;
            break;
    }
    if (instr.instructionType.dataProcessingInstruction.S) {
        if (op == cmp || op == tst || op == teq) {
            arm->cpsr[N_FLAG] = N_MASK & compareResult; // N
            if (compareResult == 0) { // used to be rd value
                arm->cpsr[Z_FLAG] = 1; //Z
            } else {
                arm->cpsr[Z_FLAG] = 0;
            }
            arm->cpsr[C_FLAG] = carryBit; // C
        } else {
            arm->cpsr[N_FLAG] = N_MASK & rd->value; // N
            if (rd->value == 0) {
                arm->cpsr[Z_FLAG] = 1; //Z
            } else {
                arm->cpsr[Z_FLAG] = 0;
            }
            arm->cpsr[C_FLAG] = carryBit; // C
        }
    }
}

uint32_t convertToSignedInt(uint32_t value) {
    uint32_t newValue = (~value) + 1;
    return newValue;
}

void executeBranch(ARM *arm, instruction instr) {
    /* We now know we need to execute the branch instruction (any conditions have been satisfied), so we need to
     * re-initialise the pipeline.
     * */
    arm->fetchedInstruction.isSet = false;
    arm->fetchedInstruction.instruction = 0;
    arm->currentlyDecoded.isSet = 0;

    uint32_t offset = instr.instructionType.branchInstruction.offset;
    if ((offset & BRANCH_TWOS_COMPLEMENT_MASK) >> TWOS_COMP_SHIFT) {
        offset = (convertToSignedInt(offset) & BRANCH_OFFSET_MASK);
        offset = offset << OFFSET_SHIFT;
        offset /= MEM_CONVERSION;
        uint32_t *v = &(arm->pc.value);
        *v -= offset;
    } else {
        offset = offset << OFFSET_SHIFT;
        uint32_t *v = &(arm->pc.value);
        *v += offset;
    }
}

#define GPIO_START 0x20200000u
#define GPIO_END 0x20200028u

void inputOrOutput(uint32_t pins, int bitPos, bool str) {
    if (str) {
        pins |= bitPos;
    }
}

void clearPin(ARM *arm, int bitPos) {
    arm->gpMem.clearGPIO |= bitPos;
}

void setPin(ARM *arm, int bitPos) {
    arm->gpMem.setGPIO |= bitPos;
}

void gpio(ARM *arm, bool str, uint32_t pos) {
    switch (pos) {
        case 0x20200000u:
            printf("One GPIO pin from 0 to 9 has been accessed\n");
            inputOrOutput(arm->gpMem.bits0to9, arm->reg1.value, str);
            break;
        case 0x20200004u:
            inputOrOutput(arm->gpMem.bits10to19, arm->reg1.value, str);
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            break;
        case 0x20200008u:
            inputOrOutput(arm->gpMem.bits20to29, arm->reg1.value, str);
            printf("One GPIO pin from 20 to 29 has been accessed\n");
            break;
        case 0x20200028u:
            printf("PIN OFF\n");
            clearPin(arm, arm->reg1.value);
            break;
        case 0x2020001cu:
            printf("PIN ON\n");
            clearPin(arm, arm->reg1.value);
            setPin(arm, arm->reg1.value);
            break;
    }
}

uint32_t loadFromMemory(ARM *arm, int pos) {
    if (pos >= GPIO_START && pos <= GPIO_END) {
        gpio(arm, false, pos);
        return pos;
    }
    if (pos > SIZE_OF_MEMORY) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", pos);
        return 0;
    }
    /* The memory is byte-addressable, but since all instructions are aligned on the 4-byte boundary, we have chosen to
     * represent our memory in 4-byte chunks. So, to be able to access memory not aligned on the 4-byte boundary, we
     * have to read memory from two contiguous chunks in memory. memPos will give the first memory chunk from which we
     * need to read data from. */
    int memPos = pos / MEM_CONVERSION;
    // The aim is to store both contiguous chunks of memory as a 64-bit number and extract the relevant bits.
    // shiftAmount will help us extract the relevant bits.
    int shiftAmount = (4 - (pos % MEM_CONVERSION)) * 8;
    /* Due to the little endianness of the architecture, we have to reverse the order of the individual bytes.
     * We also take the offset RESERVED_MEMORY into account, since we have to ignore the first two entries in the memory
     * array. */
    uint32_t form1 = convertFormat(arm->mem[memPos + RESERVED_MEMORY]);
    uint32_t form2 = convertFormat(arm->mem[memPos + RESERVED_MEMORY + 1]);
    u_int64_t temp = form1 * pow(16, 8) + form2;
    uint64_t mask64 = (uint64_t) SDT_64BIT_MASK << shiftAmount;
    uint32_t bits = (mask64 & temp) >> shiftAmount;
    uint32_t result;
    result = convertFormat(bits);
    return result;
}

void storeToMemory(ARM *arm, uint32_t value, int location) {
    // Similar reasoning to loadFromMemory.
    if (location >= GPIO_START && location <= GPIO_END) {
        gpio(arm, true, location);
        return;
    }
    if (location > SIZE_OF_MEMORY) {
        printf("Error: Out of bounds memory access at address 0x%08x\n", location);
        return;
    }
    int whichElement = location / MEM_CONVERSION;
    int whereInElement = location % MEM_CONVERSION;

    uint32_t *armPointer =  arm->mem + RESERVED_MEMORY;
    armPointer += whichElement;
    // We circumvent the type system to be able to write to the correct location in memory.
    armPointer = (uint32_t*) (((char*) armPointer + whereInElement));
    *armPointer = value;
    /* We have already taken into account the memory storage requirements for the instructions, but if we are storing to
     * new memory, we should update the value for the next available address. */
    if (whichElement >= arm->mem[0]) {
        arm->mem[1]++;
    }
}

void loadHelper(ARM *arm, uint32_t rdInt, uint32_t rnInt, uint32_t offset, bool sdtU, instruction instr) {
    reg *rd = numToReg(arm, rdInt);
    reg *rn = numToReg(arm, rnInt);
    sdtInstruction sdt = instr.instructionType.sdtInstruction;

    if (sdt.P) {
        if (sdtU) {
            /* Since instructions are aligned on the 4-byte boundary, but we only increment our program counter by 1, we
             * must multiply the value in the base register by 4 to get the correct address. */
            if (rn->numReg == PC_REG) {
                rd->value = loadFromMemory(arm, rn->value * MEM_CONVERSION + offset);
            } else {
                rd->value = loadFromMemory(arm, rn->value + offset);
            }
        } else {
            if (rn->numReg == PC_REG) {
                rd->value = loadFromMemory(arm, rn->value * MEM_CONVERSION - offset);
            } else {
                rd->value = loadFromMemory(arm, rn->value - offset);
            }
        }
    } else {
        if (rn->numReg == PC_REG) {
            rd->value = loadFromMemory(arm, rn->value * MEM_CONVERSION);
        } else {
            rd->value = loadFromMemory(arm, rn->value);
        }
        if (sdtU) {
            rn->value += offset;
        } else {
            rn->value -= offset;
        }
    }
}

void storeHelper(ARM *arm, uint32_t rdInt, uint32_t rnInt, uint32_t offset, bool sdtU, instruction instr) {
    reg *rd = numToReg(arm, rdInt);
    reg *rn = numToReg(arm, rnInt);
    sdtInstruction sdt = instr.instructionType.sdtInstruction;
    if (sdt.P) {
        if (sdtU) {
            storeToMemory(arm, rd->value, rn->value + offset);
        } else {
            storeToMemory(arm, rd->value, rn->value - offset);
        }
    } else {
        storeToMemory(arm, rd->value, rn->value);
        if (sdtU) {
            rn->value += offset;
        } else {
            rn->value -= offset;
        }
    }
}

void executeSDT(ARM *arm, instruction instr){
    sdtInstruction sdt = instr.instructionType.sdtInstruction;
    if (sdt.L) {
        if (sdt.I) {
            reg *rm = numToReg(arm, REGISTER_MASK & sdt.offset);
            sdt.offset = shiftedRegister(arm, &(sdt.offset), rm);
            loadHelper(arm, sdt.rd, sdt.rn, sdt.offset, sdt.U, instr);
        } else {
            loadHelper(arm, sdt.rd, sdt.rn, sdt.offset, sdt.U, instr);
        }
    } else {
        if (sdt.I) {
            reg *rm = numToReg(arm, REGISTER_MASK & sdt.offset);
            sdt.offset = shiftedRegister(arm, &(sdt.offset), rm);
            storeHelper(arm, sdt.rd, sdt.rn, sdt.offset, sdt.U, instr);
        } else {
            storeHelper(arm, sdt.rd, sdt.rn, sdt.offset, sdt.U, instr);
        }
    }
}


