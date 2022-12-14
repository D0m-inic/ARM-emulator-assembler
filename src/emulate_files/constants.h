#define N_MASK 0x80000000u

// Emulate constants:
#define MEMORY_INSTRUCTION_OFFSET 2


// Execute constants:
#define SHIFT_MASK 0xf80
#define SHIFT_OPERATION_MASK 0x60
#define IMM_MASK 0xff
#define ROTATE_MASK 0xf00
#define REGISTER_MASK 0xf
#define BRANCH_TWOS_COMPLEMENT_MASK 0x800000u
#define SDT_64BIT_MASK 0xffffffffu
#define TWOS_COMP_SHIFT 23

//Decode Constants
#define DECODE_BRANCH_MASK 0x8000000u
#define DECODE_SDT_MASK 0x4000000u
#define DECODE_MULTIPLY_MASK1 0X2000000U
#define DECODE_MULTIPLY_MASK2 0X90u
#define DECODE_CONDITION_MASK 0xf0000000u
#define BRANCH_OFFSET_MASK 0xffffffu
#define SDT_IFLAG_MASK 0x2000000u
#define SDT_PFLAG_MASK 0x1000000u
#define SDT_UFLAG_MASK 0x800000u
#define SDT_LFLAG_MASK 0x100000u
#define SDT_RN_MASK 0xf0000u
#define SDT_RD_MASK 0xf000u
#define SDT_OFFSET_MASK 0xfffu
#define DATAPROCESSING_IFLAG_MASK 0x2000000u
#define DATAPROCESSING_SFLAG_MASK 0x100000u
#define DATAPROCESSING_RN_MASK 0xf0000u
#define DATAPROCESSING_RD_MASK 0xf000u
#define DATAPROCESSING_OP2_MASK 0xfffu
#define DATAPROCESSING_OP_MASK 0x1e00000u
#define MULTIPLY_AFLAG_MASK 0x200000u
#define MULTIPLY_SFLAG_MASK 0x100000u
#define MULTIPLY_RD_MASK 0xf0000u
#define MULTIPLY_RN_MASK 0xf000u
#define MULTIPLY_RS_MASK 0xf00u
#define MULTIPLY_RM_MASK 0xfu

#define SIZE_INSTRUCTION_BYTES 4
#define SIZE_INSTRUCTION_NIBBLES 8

#define V_FLAG 28
#define C_FLAG 29
#define Z_FLAG 30
#define N_FLAG 31

#define MULTIPLY_INSTR_CODE 9

#define MUL_SHIFT 8
#define RD_SHIFT 12
#define RN_SHIFT 16
#define L_SHIFT 20
#define OP_SHIFT 21
#define U_SHIFT 23
#define P_SHIFT 24
#define I_SHIFT 25
#define COND_SHIFT 28

#define BYTE_SIZE 8
#define INSTRUCTION_SIZE 32

#define OFFSET_SHIFT 2
#define MEM_CONVERSION 4
#define SIZE_OF_MEMORY 65538 * 8
#define PC_REG 15

#define RESERVED_MEMORY 2
#define NUM_REGISTERS 16

#define CPSR_END 28




