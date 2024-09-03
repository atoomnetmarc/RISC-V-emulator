/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeI_H_
#define RiscvEmulatorTypeI_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

/**
 * I-type instruction.
 */
typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    int16_t imm : 12;
} RiscvInstructionTypeI_t;

typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint16_t funct12 : 12;
} RiscvInstructionTypeIStystem_t;

typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t custom5 : 5;
    uint8_t funct3 : 3;
    uint16_t custom11 : 11;
    uint16_t funct6 : 6;
} RiscvInstructionTypeIStystemCustom_t;

#if (RVE_E_ZICSR == 1)
typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint16_t csr : 12;
} RiscvInstructionTypeICSR_t;

typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3 : 3;
    uint8_t imm : 5;
    uint16_t csr : 12;
} RiscvInstructionTypeICSRImm_t;
#endif

typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint8_t shamt : 5;
    uint8_t imm11_5 : 7;
} RiscvInstructionTypeIShiftByConstant_t;

typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint8_t sw : 1;
    uint8_t sr : 1;
    uint8_t so : 1;
    uint8_t si : 1;
    uint8_t pw : 1;
    uint8_t pr : 1;
    uint8_t po : 1;
    uint8_t pi : 1;
    uint8_t fm : 4;
} RiscvInstructionTypeIMiscMemt_t;

/**
 * Feed imm11_5 and funct3.
 */
typedef struct __attribute__((packed)) {
    uint8_t funct3 : 3;
    int16_t imm11_5 : 7;
} RiscvInstructionTypeIDecoderImm11_7Funct3In_r;

/**
 * Get combined imm11_5 and funct3.
 */
typedef struct __attribute__((packed)) {
    uint16_t imm11_5funct3 : 10;
} RiscvInstructionTypeIDecoderImm11_7Funct3Out_r;

/**
 * Union for combining imm11_5 and funct3 field of R-type instruction.
 */
typedef union {
    RiscvInstructionTypeIDecoderImm11_7Funct3In_r input;
    RiscvInstructionTypeIDecoderImm11_7Funct3Out_r output;
} RiscvInstructionTypeIDecoderImm11_7Funct3Imm11_7Funct3_u;

#if (RVE_E_ZBB == 1)
/**
 * Feed imm and funct3.
 */
typedef struct __attribute__((packed)) {
    uint8_t funct3 : 3;
    int16_t imm : 12;
} RiscvInstructionTypeIDecoderImmFunct3In_r;

/**
 * Get combined imm and funct3.
 */
typedef struct __attribute__((packed)) {
    uint16_t immfunct3 : 15;
} RiscvInstructionTypeIDecoderImmFunct3Out_r;

/**
 * Union for combining imm and funct3 field of R-type instruction.
 */
typedef union {
    RiscvInstructionTypeIDecoderImmFunct3In_r input;
    RiscvInstructionTypeIDecoderImmFunct3Out_r output;
} RiscvInstructionTypeIDecoderImmFunct3ImmFunct3_u;
#endif

#endif
