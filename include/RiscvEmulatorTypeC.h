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
 * Compressed Register instruction format.
 *
 * Valid for jr, mv, ebreak, jalr and add.
 */
typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint8_t rs2 : 5;
    uint8_t rd : 5;
    uint8_t funct4 : 3;
} RiscvInstructionTypeCR_t;

/**
 * Compressed Immediate instruction format.
 *
 * Valid for addi and li.
 */
typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint8_t imm4_0 : 5;
    uint8_t rd : 5;
    uint8_t imm5 : 1;
    uint8_t funct3 : 3;
} RiscvInstructionTypeCI_t;

/**
 * Compressed Immediate instruction format.
 *
 * Valid for lui.
 */
typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint8_t imm16_12 : 5;
    uint8_t rd : 5;
    uint8_t imm17 : 1;
    uint8_t funct3 : 3;
} RiscvInstructionTypeCILui_t;

/**
 * Compressed Store instruction format.
 *
 * Valid for sw, fsw.
 */
typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint8_t rs2p : 3;
    uint8_t uimm6 : 1;
    uint8_t uimm2 : 1;
    uint8_t rs1p : 3;
    uint8_t uimm5_3 : 3;
    uint8_t funct3 : 3;
} RiscvInstructionTypeCS_t;

typedef struct __attribute__((packed)) {
    uint8_t imm4_0 : 5;
    uint8_t imm5 : 1;
    uint32_t imm31_6 : 26;
} RiscvInstructionTypeCIDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    uint32_t imm;
} RiscvInstructionTypeCIDecoderImmOut_t;

/**
 * Union for decoding imm of TypeCI.
 */
typedef union {
    RiscvInstructionTypeCIDecoderImmIn_t input;
    RiscvInstructionTypeCIDecoderImmOut_t output;
} RiscvInstructionTypeCIDecoderImm_u;

typedef struct __attribute__((packed)) {
    uint16_t : 12;
    uint8_t imm16_12 : 5;
    uint8_t imm17 : 1;
    uint16_t imm31_18 : 14;
} RiscvInstructionTypeCILuiDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    uint32_t imm;
} RiscvInstructionTypeCILuiDecoderImmOut_t;

/**
 * Union for decoding imm of lui.
 */
typedef union {
    RiscvInstructionTypeCILuiDecoderImmIn_t input;
    RiscvInstructionTypeCILuiDecoderImmOut_t output;
} RiscvInstructionTypeCILuiDecoderImm_u;

typedef struct __attribute__((packed)) {
    uint8_t : 2;
    uint8_t uimm2 : 1;
    uint8_t uimm5_3 : 3;
    uint8_t uimm6 : 1;
    uint32_t : 25;
} RiscvInstructionTypeCSDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    uint32_t uimm;
} RiscvInstructionTypeCSDecoderImmOut_t;

/**
 * Union for decoding imm of TypeCS.
 */
typedef union {
    RiscvInstructionTypeCSDecoderImmIn_t input;
    RiscvInstructionTypeCSDecoderImmOut_t output;
} RiscvInstructionTypeCSDecoderImm_u;

typedef struct __attribute__((packed)) {
    uint16_t L;
    uint16_t H;
} RiscvInstruction_t;

/**
 * Easier access to the opcode of an 16-bit instruction when you do not know the instruction yet.
 */
typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint16_t : 11;
    uint8_t funct3 : 3;
} RiscvInstructionOpcodeC_t;

typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint8_t funct3 : 3;
} RiscvInstructionTypeCDecoderOpcodeIn_t;

typedef struct __attribute__((packed)) {
    uint8_t opfunct3 : 5;
} RiscvInstructionTypeCDecoderOpcodeOut_t;

/**
 * Union for decoding opcode of a compressed instruction.
 */
typedef union {
    RiscvInstructionTypeCDecoderOpcodeIn_t input;
    RiscvInstructionTypeCDecoderOpcodeOut_t output;
} RiscvInstructionTypeCDecoderOpcode_u;

#endif

#endif