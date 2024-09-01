/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeS_H_
#define RiscvEmulatorTypeS_H_

#include <stdint.h>

/**
 * S-type instruction.
 */
typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t imm4_0 : 5;
    uint8_t funct3 : 3;
    uint8_t rs1 : 5;
    uint8_t rs2 : 5;
    uint8_t imm11_5 : 7;
} RiscvInstruction32TypeS_t;

typedef struct __attribute__((packed)) {
    uint8_t imm4_0 : 5;
    uint8_t imm11_5 : 7;
} RiscvInstruction32TypeSDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    int16_t imm : 12;
} RiscvInstruction32TypeSDecoderImmOut_t;

/**
 * Union for decoding imm field of a S-type instruction.
 */
typedef union {
    RiscvInstruction32TypeSDecoderImmIn_t input;
    RiscvInstruction32TypeSDecoderImmOut_t output;
} RiscvInstruction32TypeSDecoderImm_u;

#endif
