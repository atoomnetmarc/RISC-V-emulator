/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeJ_H_
#define RiscvEmulatorTypeJ_H_

#include <stdint.h>

/**
 * J-type instruction.
 */
typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint8_t imm19_12 : 8;
    uint8_t imm11 : 1;
    uint16_t imm10_1 : 10;
    uint8_t imm20 : 1;
} RiscvInstructionTypeJ_t;

typedef struct __attribute__((packed)) {
    uint8_t : 1;
    uint16_t imm10_1 : 10;
    uint8_t imm11 : 1;
    uint8_t imm19_12 : 8;
    uint8_t imm20 : 1;
} RiscvInstructionTypeJDecoderImmIn_t;

typedef struct __attribute__((packed)) {
    int32_t imm : 21;
} RiscvInstructionTypeJDecoderImmOut_t;

/**
 * Union for decoding imm field of a J-type instruction.
 */
typedef union {
    RiscvInstructionTypeJDecoderImmIn_t input;
    RiscvInstructionTypeJDecoderImmOut_t output;
} RiscvInstructionTypeJDecoderImm_u;

#endif
