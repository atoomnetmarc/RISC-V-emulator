/*

Copyright 2023 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeU_H_
#define RiscvEmulatorTypeU_H_

#include <stdint.h>

/**
 * U-type instruction.
 */
typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint8_t rd : 5;
    uint32_t imm31_12 : 20;
} RiscvInstructionTypeU_t;

typedef struct __attribute__((packed))
{
    uint16_t imm11_0 : 12;
    uint32_t imm31_12 : 20;
} RiscvInstructionTypeUDecoderImmIn_t;

typedef struct __attribute__((packed))
{
    uint32_t imm : 32;
} RiscvInstructionTypeUDecoderImmOut_t;

/**
 * Union for decoding imm field of J-type instruction.
 */
typedef union
{
    RiscvInstructionTypeUDecoderImmIn_t input;
    RiscvInstructionTypeUDecoderImmOut_t output;
} RiscvInstructionTypeUDecoderImm_u;

#endif
