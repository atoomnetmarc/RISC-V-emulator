/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorHook_H_
#define RiscvEmulatorHook_H_

#include <stdint.h>

#include "RiscvEmulatorDefine.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * Hook function for trap.
 */
__attribute__((weak)) void RiscvEmulatorTrapHookBegin(
    const RiscvEmulatorState_t *state) {
}

/**
 * Generic hook function for Integer Register-Register Operations.
 */
__attribute__((weak)) void RiscvEmulatorIntRegRegHookBegin(
    const char *instruction,
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num,
    const void *rs1,
    const uint8_t rs2num,
    const void *rs2) {
}

/**
 * Generic hook function for Integer Register-Register Operations.
 */
__attribute__((weak)) void RiscvEmulatorIntRegRegHookEnd(
    const char *instruction,
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num,
    const void *rs1,
    const uint8_t rs2num,
    const void *rs2) {
}

/**
 * Generic hook function for Integer Register-Immediate Operations.
 */
__attribute__((weak)) void RiscvEmulatorIntRegImmHookBegin(
    const char *instruction,
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num,
    const void *rs1,
    const uint32_t imm) {
}

/**
 * Generic hook function for Integer Register-Immediate Operations.
 */
__attribute__((weak)) void RiscvEmulatorIntRegImmHookEnd(
    const char *instruction,
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num,
    const void *rs1,
    const uint32_t imm) {
}

/**
 * Generic hook function for Branch Operations.
 */
__attribute__((weak)) void RiscvEmulatorBranchHookBegin(
    const char *instruction,
    const RiscvEmulatorState_t *state,
    const uint8_t rs1num,
    const void *rs1,
    const uint8_t rs2num,
    const void *rs2,
    const int16_t imm) {
}

/**
 * Generic hook function for Branch Operations.
 */
__attribute__((weak)) void RiscvEmulatorBranchHookEnd(
    const RiscvEmulatorState_t *state) {
}

/**
 * AUIPC hook function.
 */
__attribute__((weak)) void RiscvEmulatorAUIPCHookBegin(
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint32_t imm31_12,
    const uint32_t imm) {
}

/**
 * AUIPC hook function.
 */
__attribute__((weak)) void RiscvEmulatorAUIPCHookEnd(
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint32_t imm31_12,
    const uint32_t imm) {
}

/**
 * LUI hook function.
 */
__attribute__((weak)) void RiscvEmulatorLUIHookBegin(
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint32_t imm) {
}

/**
 * LUI hook function.
 */
__attribute__((weak)) void RiscvEmulatorLUIHookEnd(
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint32_t imm) {
}

/**
 * JAL hook function.
 */
__attribute__((weak)) void RiscvEmulatorJALHookBegin(
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint32_t imm) {
}

/**
 * JAL hook function.
 */
__attribute__((weak)) void RiscvEmulatorJALHookEnd(
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint32_t imm) {
}

/**
 * JALR hook function.
 */
__attribute__((weak)) void RiscvEmulatorJALRHookBegin(
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num,
    const void *rs1,
    const int16_t imm) {
}

/**
 * JALR hook function.
 */
__attribute__((weak)) void RiscvEmulatorJALRHookEnd(
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num,
    const void *rs1,
    const int16_t imm) {
}

/**
 * CSRRW/CSRRS/CSRRC hook function.
 */
__attribute__((weak)) void RiscvEmulatorCSRR_HookBegin(
    const char *instruction,
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num,
    const void *rs1,
    const uint16_t csrnum,
    const void *csr) {
}

/**
 * CSRRW/CSRRS/CSRRC hook function.
 */
__attribute__((weak)) void RiscvEmulatorCSRR_HookEnd(
    const char *instruction,
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num,
    const void *rs1,
    const uint16_t csrnum,
    const void *csr) {
}

#pragma GCC diagnostic pop

#endif