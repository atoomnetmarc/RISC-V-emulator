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
 * Non-temporal locality hint: the following instruction accesses memory with program-order locality.
 * This does nothing in this emulator because there are no caches.
 */
static inline void RiscvEmulatorNTLP1(
    RiscvEmulatorState_t *state __attribute__((unused))) {
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "ntl.p1";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif
}

/**
 * Non-temporal locality hint: the following instruction accesses memory with inner-shareable locality.
 * This does nothing in this emulator because there are no caches.
 */
static inline void RiscvEmulatorNTLS1(
    RiscvEmulatorState_t *state __attribute__((unused))) {
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "ntl.s1";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif
}

/**
 * Non-temporal locality hint: the following instruction accesses memory with outer-shareable locality.
 * This does nothing in this emulator because there are no caches.
 */
static inline void RiscvEmulatorNTLPALL(
    RiscvEmulatorState_t *state __attribute__((unused))) {
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "ntl.pall";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif
}

/**
 * Non-temporal locality hint: the following instruction accesses memory with full-shareable locality.
 * This does nothing in this emulator because there are no caches.
 */
static inline void RiscvEmulatorNTLALL(
    RiscvEmulatorState_t *state __attribute__((unused))) {
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "ntl.all";
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
