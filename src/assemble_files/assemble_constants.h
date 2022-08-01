//In assemble.c
#define MAX_LINE_LENGTH 511
#define NUM_PREDEFINED_SYMBOLS 23

#define AND_OPCODE_MASK 0
#define EOR_OPCODE_MASK 1
#define SUB_OPCODE_MASK 2
#define RSB_OPCODE_MASK 3
#define ADD_OPCODE_MASK 4
#define ORR_OPCODE_MASK 12
#define MOV_OPCODE_MASK 13
#define TST_OPCODE_MASK 8
#define TEQ_OPCODE_MASK 9
#define CMP_OPCODE_MASK 10
#define DP_S_FLAG 0x100000
#define DP_I_FLAG 0x2000000
#define LSL_I_FLAG 0xfdffffff
#define DP_COND_MASK 0xE0000000u
#define SDT_COND_MASK 0xE4000000u
#define SDT_P_FLAG 0x01000000u
#define SDT_I_FLAG 0x02000000u
#define SDT_U_FLAG 0x00800000u
#define SDT_L_FLAG 0x00100000u
#define MULTIPLY_BIT_MASK 0x90
#define MULTIPLY_A_MASK 0x00200000

#define MAX_INSTRUCTION_NAME_SIZE 6
#define ADDRESS_CONVERSION 4
#define MAX_REG_NAME_SIZE 3

#define STANDARD_INSTRUCTION_SIZE 4

#define NUM_BITS_FOUR_BYTES 32

#define DP_OPCODE 21
#define DP_RD 12
#define DP_RN 16
#define DP_ROTATE_SHIFT 8
#define DP_IMMEDIATE_MASK 0xffu

#define MULTIPLY_RD_SHIFT 16
#define MULTIPLY_RN_SHIFT 12
#define MULTIPLY_RS_SHIFT 8

#define LSL_OPCODE_SHIFT 21
#define LSL_RN_SHIFT 12
#define LSL_SHIFTAMOUNT_SHIFT 7

#define SDT_SET_BIT 0x04000000
#define SDT_RD_SHIFT 12
#define SDT_OPCODE_SHIFT 21
#define SDT_PC 0xfu
#define SDT_RN_SHIFT 16
#define PC_PIPELINE_OFFSET 8

#define ALL_BRANCH_MASK 0xA000000u
#define BNE_MASK 0x10000000u
#define BGE_MASK 0xa0000000u
#define BLT_MASK 0xb0000000u
#define BGT_MASK 0xc0000000u
#define BLE_MASK 0xd0000000u
#define B_MASK 0xe0000000u
#define BRANCH_OFFSET_MASK 0xffffffu

//In symbolTable.c

#define MAX_TABLE_SIZE 100 //CHANGE
#define MAX_FILE_LINE_LENGTH 16
