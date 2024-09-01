/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeB_H_
#define RiscvEmulatorTypeB_H_

#include <stdint.h>

/**
 * B-type instruction.
 */
typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t imm11 : 1;
    uint8_t imm4_1 : 4;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint8_t rs2 : 5;
    uint8_t imm10_5 : 6;
    uint8_t imm12 : 1;
} RiscvInstruction32TypeB_t;

typedef struct __attribute__((packed)) {
    uint8_t imm0 : 1;
    uint8_t imm4_1 : 4;
    uint8_t imm10_5 : 6;
    uint8_t imm11 : 1;
    uint8_t imm12 : 1;
} RiscvInstruction32TypeBDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    int16_t imm : 13;
} RiscvInstruction32TypeBDecoderImmOut_t;

/**
 * Union for decoding imm field of a B-type instruction.
 */
typedef union {
    RiscvInstruction32TypeBDecoderImmIn_t input;
    RiscvInstruction32TypeBDecoderImmOut_t output;
} RiscvInstruction32TypeBDecoderImm_u;

#endif
