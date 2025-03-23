/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeHook_H_
#define RiscvEmulatorTypeHook_H_

#include <stdint.h>

/**
 * Generic hook function context.
 */
typedef struct {
    uint8_t hook;
    const char *instruction;
    uint8_t rs1num;
    const void *rs1;
    uint8_t rs2num;
    const void *rs2;
    uint8_t rdnum;
    const void *rd;
    uint16_t csrnum;
    const void *csr;
    char *immname;
    uint8_t immlength;
    uint8_t immissigned;
    uint32_t imm;
    uint32_t upperimmediate;
    uint32_t memorylocation;
    uint8_t length;
} RiscvEmulatorHookContext_t;

#endif
