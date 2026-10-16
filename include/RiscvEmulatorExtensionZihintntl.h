/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZihintntl_H_
#define RiscvEmulatorExtensionZihintntl_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZIHINTNTL == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Non-temporal locality hints: ntl.p1, ntl.pall, ntl.s1 and ntl.all.
 * These do nothing in this emulator because there are no caches.
 * The variant is the rs2 field of the add x0, x0, rs2 encoding.
 */
static inline void RiscvEmulatorNTLOperation(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint16_t variant) {
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    switch (variant) {
        case FUNCT7_FUNCT3_RS2_NTL_P1:
            hc.instruction = "ntl.p1";
            break;
        case FUNCT7_FUNCT3_RS2_NTL_PALL:
            hc.instruction = "ntl.pall";
            break;
        case FUNCT7_FUNCT3_RS2_NTL_S1:
            hc.instruction = "ntl.s1";
            break;
        default: // FUNCT7_FUNCT3_RS2_NTL_ALL
            hc.instruction = "ntl.all";
            break;
    }
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
#endif
#endif
}

#endif

#endif
