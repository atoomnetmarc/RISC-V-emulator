/*

Copyright 2023 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#include <stdint.h>

#ifndef RiscvEmulatorType_H_
#define RiscvEmulatorType_H_

/**
 * Named registers.
 */
typedef struct __attribute__((packed))
{
    uint32_t x0;
    uint32_t x1;
    uint32_t x2;
    uint32_t x3;
    uint32_t x4;
    uint32_t x5;
    uint32_t x6;
    uint32_t x7;
    uint32_t x8;
    uint32_t x9;
    uint32_t x10;
    uint32_t x11;
    uint32_t x12;
    uint32_t x13;
    uint32_t x14;
    uint32_t x15;
    uint32_t x16;
    uint32_t x17;
    uint32_t x18;
    uint32_t x19;
    uint32_t x20;
    uint32_t x21;
    uint32_t x22;
    uint32_t x23;
    uint32_t x24;
    uint32_t x25;
    uint32_t x26;
    uint32_t x27;
    uint32_t x28;
    uint32_t x29;
    uint32_t x30;
    uint32_t x31;

} RiscvRegisterName_t;

/**
 * Symbolic registers.
 */
typedef struct __attribute__((packed))
{
    /**
     * Always zero.
     */
    uint32_t Zero;

    /**
     * Return address.
     */
    uint32_t ra;

    /**
     * Stack pointer.
     */
    uint32_t sp;

    /**
     * Global pointer.
     */
    uint32_t gp;

    /**
     * Thread pointer.
     */
    uint32_t tp;

    /**
     * Temporary or alternate link register.
     */
    uint32_t t0;

    /**
     * Temporary register.
     */
    uint32_t t1;

    /**
     * Temporary register.
     */
    uint32_t t2;

    /**
     * Saved register or frame pointer.
     */
    uint32_t s0_fp;

    /**
     * Saved register.
     */
    uint32_t s1;

    /**
     * Function argument or return value.
     */
    uint32_t a0;

    /**
     * Function argument or return value.
     */
    uint32_t a1;

    /**
     * Function argument.
     */
    uint32_t a2;

    /**
     * Function argument.
     */
    uint32_t a3;

    /**
     * Function argument.
     */
    uint32_t a4;

    /**
     * Function argument.
     */
    uint32_t a5;

    /**
     * Function argument.
     */
    uint32_t a6;

    /**
     * Function argument.
     */
    uint32_t a7;

    /**
     * Saved register.
     */
    uint32_t s2;

    /**
     * Saved register.
     */
    uint32_t s3;

    /**
     * Saved register.
     */
    uint32_t s4;

    /**
     * Saved register.
     */
    uint32_t s5;

    /**
     * Saved register.
     */
    uint32_t s6;

    /**
     * Saved register.
     */
    uint32_t s7;

    /**
     * Saved register.
     */
    uint32_t s8;

    /**
     * Saved register.
     */
    uint32_t s9;

    /**
     * Saved register.
     */
    uint32_t s10;

    /**
     * Saved register.
     */
    uint32_t s11;

    /**
     * Temporary register.
     */
    uint32_t t3;

    /**
     * Temporary register.
     */
    uint32_t t4;

    /**
     * Temporary register.
     */
    uint32_t t5;

    /**
     * Temporary register.
     */
    uint32_t t6;
} RiscvRegisterSymbolic_t;

/**
 * Unsigned array of registers.
 */
typedef struct __attribute__((packed))
{
    uint32_t location[32];
} RiscvRegisterArray_t;

/**
 * Signed array of registers.
 */
typedef struct __attribute__((packed))
{
    int32_t location[32];
} RiscvRegisterArraySigned_t;

/**
 * Union of all the ways a register can be accessed.
 */
typedef union
{
    RiscvRegisterName_t name;
    RiscvRegisterSymbolic_t symbolic;
    RiscvRegisterArray_t array;
    RiscvRegisterArraySigned_t array_signed;
} RiscvRegister_u;

/**
 * R-type instruction.
 */
typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint8_t rs2 : 5;
    uint8_t funct7 : 7;
} RiscvInstructionTypeR_t;

/**
 * Feed funct3 and funct7.
 */
typedef struct __attribute__((packed))
{
    uint8_t funct3 : 3;
    uint8_t funct7 : 7;
} RiscvInstructionTypeRDecoderFunct7_3In_r;

/**
 * Get combined funct7_4.
 */
typedef struct __attribute__((packed))
{
    uint16_t funct7_3 : 10;
} RiscvInstructionTypeRDecoderFunct7_3Out_r;

/**
 * Union for combining funct3 and funct7 field of R-type instruction.
 */
typedef union
{
    RiscvInstructionTypeRDecoderFunct7_3In_r input;
    RiscvInstructionTypeRDecoderFunct7_3Out_r output;
} RiscvInstructionTypeRDecoderFunct7_3Funct7_3_u;

/**
 * I-type instruction.
 */
typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    int16_t imm : 12;
} RiscvInstructionTypeI_t;

typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint16_t rd_funct3_rs1 : 13;
    uint16_t funct12 : 12;
} RiscvInstructionTypeIStystem_t;

typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint8_t shamt : 5;
    uint8_t imm11_5 : 7;
} RiscvInstructionTypeIShiftByConstant_t;

/**
 * S-type instruction.
 */
typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint8_t imm4_0 : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint8_t rs2 : 5;
    uint8_t imm11_5 : 7;
} RiscvInstructionTypeS_t;

typedef struct __attribute__((packed))
{
    uint8_t imm4_0 : 5;
    uint8_t imm11_5 : 7;
} RiscvInstructionTypeSDecoderImmIn_t;

typedef struct __attribute__((packed))
{
    int16_t imm : 12;
} RiscvInstructionTypeSDecoderImmOut_t;

typedef union
{
    RiscvInstructionTypeSDecoderImmIn_t input;
    RiscvInstructionTypeSDecoderImmOut_t output;
} RiscvInstructionTypeSDecoderImm_u;

/**
 * B-type instruction.
 */
typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint8_t imm11 : 1;
    uint8_t imm4_1 : 4;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint8_t rs2 : 5;
    uint8_t imm10_5 : 6;
    uint8_t imm12 : 1;
} RiscvInstructionTypeB_t;

typedef struct __attribute__((packed))
{
    uint8_t imm0 : 1;
    uint8_t imm4_1 : 4;
    uint8_t imm10_5 : 6;
    uint8_t imm11 : 1;
    uint8_t imm12 : 1;
} RiscvInstructionTypeBDecoderImmIn_t;

typedef struct __attribute__((packed))
{
    int16_t imm : 13;
} RiscvInstructionTypeBDecoderImmOut_t;

typedef union
{
    RiscvInstructionTypeBDecoderImmIn_t input;
    RiscvInstructionTypeBDecoderImmOut_t output;
} RiscvInstructionTypeBDecoderImm_u;

/**
 * U-type instruction.
 */
typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint32_t imm31_12 : 20;
} RiscvInstructionTypeU_t;

typedef struct __attribute__((packed))
{
    uint16_t imm11_0 : 12;
    uint32_t imm31_12 : 20;
} RiscvInstructionTypeUDecoderImmIn_t;

typedef struct __attribute__((packed))
{
    uint32_t imm : 32;
} RiscvInstructionTypeUDecoderImmOut_t;

/**
 * Union for decoding imm field of J-type instruction.
 */
typedef union
{
    RiscvInstructionTypeUDecoderImmIn_t input;
    RiscvInstructionTypeUDecoderImmOut_t output;
} RiscvInstructionTypeUDecoderImm_u;

/**
 * J-type instruction.
 */
typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t imm19_12 : 8;
    uint8_t imm11 : 1;
    uint16_t imm10_1 : 10;
    uint8_t imm20 : 1;
} RiscvInstructionTypeJ_t;

typedef struct __attribute__((packed))
{
    uint8_t : 1;
    uint16_t imm10_1 : 10;
    uint8_t imm11 : 1;
    uint8_t imm19_12 : 8;
    uint8_t imm20 : 1;
} RiscvInstructionTypeJDecoderImmIn_t;

typedef struct __attribute__((packed))
{
    int32_t imm : 21;
} RiscvInstructionTypeJDecoderImmOut_t;

/**
 * Union for decoding imm field of J-type instruction.
 */
typedef union
{
    RiscvInstructionTypeJDecoderImmIn_t input;
    RiscvInstructionTypeJDecoderImmOut_t output;
} RiscvInstructionTypeJDecoderImm_u;

/**
 * Easy access to the opcode of an instruction when you do not know the instruction type yet.
 */
typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint32_t : 25;
} RiscvInstructionOpcode_t;

/**
 * All the instruction types combined.
 */
typedef union
{
    uint32_t value;
    RiscvInstructionOpcode_t opcode;
    RiscvInstructionTypeR_t rtype;
    RiscvInstructionTypeI_t itype;
    RiscvInstructionTypeIStystem_t itypesystem;
    RiscvInstructionTypeIShiftByConstant_t itypeshiftbyconstant;
    RiscvInstructionTypeS_t stype;
    RiscvInstructionTypeB_t btype;
    RiscvInstructionTypeU_t utype;
    RiscvInstructionTypeJ_t jtype;
} instruction_u;

/**
 * Riscv emulator state.
 */
typedef struct
{
    uint32_t programcounter;
    instruction_u instruction;
    RiscvRegister_u registers;
} RiscvEmulatorState_t;

#endif
