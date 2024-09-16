/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeR_H_
#define RiscvEmulatorTypeR_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

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

#if (RVE_E_ZBA == 1)
/**
 * R-type instruction, but with helper field on funct3 for number if left shifts for shadd function.
 */
typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3_1 : 1;
    uint8_t funct3_shifts : 2;
    uint8_t rs1 : 5;
    uint8_t rs2 : 5;
    uint8_t funct7 : 7;
} RiscvInstructionTypeRShift_t;
#endif

/**
 * Union for combining funct3 and funct7 field of R-type instruction.
 */
typedef union {
    struct __attribute__((packed)) {
        uint8_t funct3 : 3;
        uint8_t funct7 : 7;
    };

    struct __attribute__((packed)) {
        uint16_t funct7_3 : 10;
    };
} RiscvInstructionTypeRDecoderFunct7Funct3_u;

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
 * Union for combining funct3 and funct5 field of R-type instruction.
 */
typedef union {
    struct __attribute__((packed)) {
        uint8_t funct3 : 3;
        uint8_t funct5 : 5;
    };

    struct __attribute__((packed)) {
        uint16_t funct5_3 : 8;
    };
} RiscvInstructionTypeRDecoderFunct5Funct3_u;
#endif

#if (RVE_E_ZBB == 1)
/**
 * Union for combining funct3, rs2 and funct7 field of R-type instruction.
 */
typedef union {
    struct __attribute__((packed)) {
        uint8_t funct3 : 3;
        uint8_t rs2 : 5;
        uint8_t funct7 : 7;
    };

    struct __attribute__((packed)) {
        uint16_t funct3_rs2_funct7 : 15;
    };
} RiscvInstructionTypeRDecoderFunct3Rs2Funct7_u;
#endif

#endif
