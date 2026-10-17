/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTrapFlag_H_
#define RiscvEmulatorTrapFlag_H_

#include "RiscvEmulatorType.h"

/**
 * Flags the executed instruction as illegal. Cold path: kept out-of-line so
 * the trap-flag read-modify-write is not inlined at every decode site.
 */
static void __attribute__((noinline)) RiscvEmulatorTrapFlagIllegalInstruction(RiscvEmulatorState_t *state) {
    state->trapflag.illegalinstruction = 1;
}

#endif
