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
    uint8_t funct4 : 4;
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
 * Compressed Immediate instruction format.
 *
 * Valid for lwsp and flwsp.
 */
typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint8_t imm7_6 : 2;
    uint8_t imm4_2 : 3;
    uint8_t rd : 5;
    uint8_t imm5 : 1;
    uint8_t funct3 : 3;
} RiscvInstructionTypeCILwsp_t;

/**
 * Compressed Stack-relative Store instruction format.
 *
 * Valid for swsp and fswsp.
 */
typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint8_t rs2 : 5;
    uint8_t imm7_6 : 2;
    uint8_t imm5_2 : 4;
    uint8_t funct3 : 3;
} RiscvInstructionTypeCSS_t;

/**
 * Compressed Wide Immediate instruction format.
 *
 * Valid for addi4spn.
 */
typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint8_t rdp : 3;
    uint8_t imm3 : 1;
    uint8_t imm2 : 1;
    uint8_t imm9_6 : 4;
    uint8_t imm5_4 : 2;
    uint8_t funct3 : 3;
} RiscvInstructionTypeCIW_t;

/**
 * Compressed Stack-relative Store instruction format.
 *
 * Valid for fsdsp.
 */
typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint8_t rs2 : 5;
    uint8_t imm8_6 : 3;
    uint8_t imm5_3 : 3;
    uint8_t funct3 : 3;
} RiscvInstructionTypeCSSFsdsp_t;

/**
 * Compressed Store instruction format.
 *
 * Valid for sw, fsw.
 */
typedef struct __attribute__((packed)) {
    uint8_t op : 2;
    uint8_t rs2p : 3;
    uint8_t imm6 : 1;
    uint8_t imm2 : 1;
    uint8_t rs1p : 3;
    uint8_t imm5_3 : 3;
    uint8_t funct3 : 3;
} RiscvInstructionTypeCS_t;

typedef struct __attribute__((packed)) {
    uint8_t imm4_0 : 5;
    uint8_t imm5 : 1;
} RiscvInstructionTypeCIDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    int32_t imm : 6;
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
} RiscvInstructionTypeCILuiDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    int32_t imm : 18;
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
    uint8_t imm4_2 : 3;
    uint8_t imm5 : 1;
    uint8_t imm7_6 : 2;
} RiscvInstructionTypeCILwspDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    uint8_t imm : 8;
} RiscvInstructionTypeCILwspDecoderImmOut_t;

/**
 * Union for decoding imm of lwsp and flwsp.
 */
typedef union {
    RiscvInstructionTypeCILwspDecoderImmIn_t input;
    RiscvInstructionTypeCILwspDecoderImmOut_t output;
} RiscvInstructionTypeCILwspDecoderImm_u;

typedef struct __attribute__((packed)) {
    uint8_t imm1_0 : 2;
    uint8_t imm5_2 : 4;
    uint8_t imm7_6 : 2;
} RiscvInstructionTypeCSSDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    uint8_t imm : 8;
} RiscvInstructionTypeCSSDecoderImmOut_t;

/**
 * Union for decoding imm of TypeCSS.
 */
typedef union {
    RiscvInstructionTypeCSSDecoderImmIn_t input;
    RiscvInstructionTypeCSSDecoderImmOut_t output;
} RiscvInstructionTypeCSSDecoderImm_u;

typedef struct __attribute__((packed)) {
    uint8_t imm1_0 : 2;
    uint8_t imm2 : 1;
    uint8_t imm3 : 1;
    uint8_t imm5_4 : 2;
    uint8_t imm9_6 : 4;
} RiscvInstructionTypeCIWDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    uint16_t imm : 10;
} RiscvInstructionTypeCIWDecoderImmOut_t;

/**
 * Union for decoding imm of TypeCIW.
 */
typedef union {
    RiscvInstructionTypeCIWDecoderImmIn_t input;
    RiscvInstructionTypeCIWDecoderImmOut_t output;
} RiscvInstructionTypeCIWDecoderImm_u;

typedef struct __attribute__((packed)) {
    uint8_t : 2;
    uint8_t imm2 : 1;
    uint8_t imm5_3 : 3;
    uint8_t imm6 : 1;
} RiscvInstructionTypeCSDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    uint8_t imm : 7;
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