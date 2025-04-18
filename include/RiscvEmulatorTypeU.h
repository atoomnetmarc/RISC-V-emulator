/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeU_H_
#define RiscvEmulatorTypeU_H_

#include <stdint.h>

/**
 * U-type instruction.
 */
typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint32_t imm31_12 : 20;
} RiscvInstructionTypeU_t;

/**
 * Union for decoding imm field of a U-type instruction.
 */
typedef union {
    struct __attribute__((packed)) {
        uint16_t imm11_0 : 12;
        uint32_t imm31_12 : 20;
    } bit;

    struct __attribute__((packed)) {
        uint32_t imm : 32;
    };
} RiscvInstructionTypeUDecoderImm_u;

#endif
