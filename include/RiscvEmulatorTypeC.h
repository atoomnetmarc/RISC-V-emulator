/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeC_H_
#define RiscvEmulatorTypeC_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_C == 1)

#include <stdint.h>

/**
 * Compressed instructions quadrant 1, variant 1.
 *
 * Valid for addi and li.
 */
typedef struct __attribute__((packed)) {
    uint8_t inst1_0 : 2;
    uint8_t imm4_0 : 5;
    uint8_t rd : 5;
    uint8_t imm5 : 1;
    uint8_t inst15_13 : 3;
} RiscvInstruction16TypeCQ1v1_t;

/**
 * Compressed instructions quadrant 1, lui.
 */
typedef struct __attribute__((packed)) {
    uint8_t inst1_0 : 2;
    uint8_t imm16_12 : 5;
    uint8_t rd : 5;
    uint8_t imm17 : 1;
    uint8_t inst15_13 : 3;
} RiscvInstruction16TypeCQ1lui_t;

/**
 * Compressed instructions quadrant 2, variant 1.
 *
 * Valid for jr, mv, ebreak, jalr and add.
 */
typedef struct __attribute__((packed)) {
    uint8_t inst1_0 : 2;
    uint8_t rs2 : 5;
    uint8_t rd : 5;
    uint8_t inst12 : 1;
    uint8_t inst15_13 : 3;
} RiscvInstruction16TypeCQ2v1_t;

typedef struct __attribute__((packed)) {
    uint8_t imm4_0 : 5;
    uint8_t imm5 : 1;
} RiscvInstruction16TypeCQ1v1DecoderImmIn_t;

typedef struct __attribute__((packed)) {
    uint8_t imm : 6;
} RiscvInstruction16TypeCQ1v1DecoderImmOut_t;

/**
 * Union for decoding imm of TypeCQ1v1.
 */
typedef union {
    RiscvInstruction16TypeCQ1v1DecoderImmIn_t input;
    RiscvInstruction16TypeCQ1v1DecoderImmOut_t output;
} RiscvInstruction16TypeCQ1v1DecoderImm_u;

typedef struct __attribute__((packed)) {
    uint16_t : 12;
    uint8_t imm16_12 : 5;
    uint8_t imm17 : 1;
    uint16_t imm31_18 : 14;
} RiscvInstruction16TypeCQ1luiDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    uint32_t imm;
} RiscvInstruction16TypeCQ1luiDecoderImmOut_t;

/**
 * Union for decoding imm of lui.
 */
typedef union {
    RiscvInstruction16TypeCQ1luiDecoderImmIn_t input;
    RiscvInstruction16TypeCQ1luiDecoderImmOut_t output;
} RiscvInstruction16TypeCQ1luiDecoderImm_u;

typedef struct __attribute__((packed)) {
    uint16_t L;
    uint16_t H;
} RiscvInstruction16_t;

/**
 * Easier access to the opcode of an 16-bit instruction when you do not know the instruction yet.
 */
typedef struct __attribute__((packed)) {
    uint8_t inst1_0 : 2;
    uint16_t : 11;
    uint8_t inst15_13 : 3;
} RiscvInstruction16Opcode_t;

typedef struct __attribute__((packed)) {
    uint8_t inst1_0 : 2;
    uint8_t inst15_13 : 3;
} RiscvInstruction16TypeCDecoderOpcodeIn_t;

typedef struct __attribute__((packed)) {
    uint8_t opcode : 5;
} RiscvInstruction16TypeCDecoderOpcodeOut_t;

/**
 * Union for decoding opcode of a compressed instruction.
 */
typedef union {
    RiscvInstruction16TypeCDecoderOpcodeIn_t input;
    RiscvInstruction16TypeCDecoderOpcodeOut_t output;
} RiscvInstruction16TypeCDecoderOpcode_u;

/**
 * Easy access to the 16bit instruction quadrant bits.
 */
typedef struct __attribute__((packed)) {
    uint8_t quadrant : 2;
    uint16_t : 14;
} RiscvInstruction16OpcodeQuadrant_t;
#endif

#endif