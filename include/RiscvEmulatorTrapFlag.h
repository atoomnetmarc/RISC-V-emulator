/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTrapFlag_H_
#define RiscvEmulatorTrapFlag_H_

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Raise a synchronous exception.
 *
 * Only one exception can be raised per instruction. The first raise wins;
 * later raises for the same instruction are ignored.
 */
static inline void RiscvEmulatorRaiseException(RiscvEmulatorState_t *state, uint8_t cause, uint32_t tval) {
    if (state->trap.pending == 0) {
        state->trap.pending = 1;
        state->trap.cause = cause;
        state->trap.tval = tval;
    }
}

/**
 * Flags the executed instruction as illegal. Cold path: kept out-of-line so
 * the trap-flag read-modify-write is not inlined at every decode site.
 */
static void __attribute__((noinline)) RiscvEmulatorTrapFlagIllegalInstruction(RiscvEmulatorState_t *state) {
    RiscvEmulatorRaiseException(state, MCAUSE_EXCEPTION_CODE_ILLEGAL_INSTRUCTION, state->instruction.value);
}

#endif
