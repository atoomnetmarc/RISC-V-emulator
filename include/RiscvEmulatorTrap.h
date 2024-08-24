/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTrap_H_
#define RiscvEmulatorTrap_H_

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"

/**
 * Handle a trap.
 */
static inline void RiscvEmulatorTrap(RiscvEmulatorState_t *state) {
#if (RVE_E_ZICSR == 1)
    // Load address misaligned
    if (state->trapflags.bits.loadaddressmisaligned == 1) {
        state->csr.mcause.exceptioncode = MCAUSE_EXCEPTION_CODE_LOAD_ADDRESS_MISALIGNED;
    }

    // Store/AMO address misaligned
    if (state->trapflags.bits.storeaddressmisaligned == 1) {
        state->csr.mcause.exceptioncode = MCAUSE_EXCEPTION_CODE_STORE_ADDRESS_MISALIGNED;
    }
#endif

    // Illegal instruction
    if (state->trapflags.bits.illegalinstruction == 1) {
        RiscvEmulatorIllegalInstruction(state);

#if (RVE_E_ZICSR == 1)
        state->csr.mcause.exceptioncode = MCAUSE_EXCEPTION_CODE_ILLEGAL_INSTRUCTION;
#endif
    }

#if (RVE_E_ZICSR == 1)
    state->csr.mstatus.mpie = state->csr.mstatus.mie;
    state->csr.mstatus.mie = 0;
    state->csr.mepc = state->programcounter;

    // Jump to trap handler.
    state->programcounternext = state->csr.mtvec.base << 2;
    // For mode 1, add some offset based on exceptioncode.
    if (state->csr.mtvec.mode == 1) {
        state->programcounternext = 4 * state->csr.mcause.exceptioncode;
    }
#endif

    state->trapflags.value = 0;
}

#endif