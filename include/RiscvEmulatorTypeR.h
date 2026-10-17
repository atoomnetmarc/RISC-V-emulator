/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
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
 * Combine funct3 and funct7 field of R-type instruction into a single switch key.
 */
static inline uint16_t RiscvEmulatorDecodeFunct7Funct3(uint8_t funct7, uint8_t funct3) {
    return (uint16_t)(((uint16_t)funct7 << 3) | (uint16_t)funct3);
}

#if ((RVE_E_ZAAMO == 1) || (RVE_E_ZALRSC == 1))
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
 * Combine funct3 and funct5 field of R-type instruction into a single switch key.
 */
static inline uint8_t RiscvEmulatorDecodeFunct5Funct3(uint8_t funct5, uint8_t funct3) {
    return (uint8_t)(((uint16_t)funct5 << 3) | (uint16_t)funct3);
}
#endif

#if ((RVE_E_ZBB == 1) || (RVE_E_ZBKB == 1))
/**
 * Combine funct3, rs2 and funct7 field of R-type instruction into a single switch key.
 */
static inline uint16_t RiscvEmulatorDecodeFunct3Rs2Funct7(uint8_t funct3, uint8_t rs2, uint8_t funct7) {
    return (uint16_t)(((uint16_t)funct7 << 8) | ((uint16_t)rs2 << 3) | (uint16_t)funct3);
}
#endif

#endif
