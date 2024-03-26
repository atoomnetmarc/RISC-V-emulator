/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeR_H_
#define RiscvEmulatorTypeR_H_

#include <stdint.h>

/**
 * R-type instruction.
 */
typedef struct __attribute__((packed)) {
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
typedef struct __attribute__((packed)) {
    uint8_t funct3 : 3;
    uint8_t funct7 : 7;
} RiscvInstructionTypeRDecoderFunct7_3In_r;

/**
 * Get combined funct7_3.
 */
typedef struct __attribute__((packed)) {
    uint16_t funct7_3 : 10;
} RiscvInstructionTypeRDecoderFunct7_3Out_r;

/**
 * Union for combining funct3 and funct7 field of R-type instruction.
 */
typedef union {
    RiscvInstructionTypeRDecoderFunct7_3In_r input;
    RiscvInstructionTypeRDecoderFunct7_3Out_r output;
} RiscvInstructionTypeRDecoderFunct7_3Funct7_3_u;

#if (RVE_E_A == 1)
/**
 * Atomic memory instruction.
 */
typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint8_t rs2 : 5;
    uint8_t rl : 1;
    uint8_t aq : 1;
    uint8_t funct5 : 5;
} RiscvInstructionTypeRAtomicMemoryOperation_t;

/**
 * Feed funct3 and funct5.
 */
typedef struct __attribute__((packed)) {
    uint8_t funct3 : 3;
    uint8_t funct5 : 5;
} RiscvInstructionTypeRDecoderFunct5_3In_r;

/**
 * Get combined funct5_3.
 */
typedef struct __attribute__((packed)) {
    uint16_t funct5_3 : 8;
} RiscvInstructionTypeRDecoderFunct5_3Out_r;

/**
 * Union for combining funct3 and funct5 field of R-type instruction.
 */
typedef union {
    RiscvInstructionTypeRDecoderFunct5_3In_r input;
    RiscvInstructionTypeRDecoderFunct5_3Out_r output;
} RiscvInstructionTypeRDecoderFunct5_3Funct5_3_u;
#endif

#endif
