/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTrap_H_
#define RiscvEmulatorTrap_H_

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorHook.h"

/**
 * Handle a trap.
 */
static inline void RiscvEmulatorTrap(RiscvEmulatorState_t *state) {
#if (RVE_E_ZICSR == 1)

    // Instruction address misaligned
    if (state->trapflag.instructionaddressmisaligned == 1) {
        state->csr.mcause.exceptioncode = MCAUSE_EXCEPTION_CODE_INSTRUCTION_ADDRESS_MISALIGNED;
    }

    // Breakpoint
    if (state->trapflag.breakpoint == 1) {
        state->csr.mcause.exceptioncode = MCAUSE_EXCEPTION_CODE_BREAKPOINT;
    }

    // Load address misaligned
    if (state->trapflag.loadaddressmisaligned == 1) {
        state->csr.mcause.exceptioncode = MCAUSE_EXCEPTION_CODE_LOAD_ADDRESS_MISALIGNED;
    }

    // Store/AMO address misaligned
    if (state->trapflag.storeaddressmisaligned == 1) {
        state->csr.mcause.exceptioncode = MCAUSE_EXCEPTION_CODE_STORE_ADDRESS_MISALIGNED;
    }

    //  Environment call from M-mode
    if (state->trapflag.environmentcallfrommmode == 1) {
        state->csr.mcause.exceptioncode = MCAUSE_EXCEPTION_CODE_ENVIRONMENT_CALL_FROM_MMODE;
    }

    // Illegal instruction
    if (state->trapflag.illegalinstruction == 1) {
        state->csr.mcause.exceptioncode = MCAUSE_EXCEPTION_CODE_ILLEGAL_INSTRUCTION;
        state->csr.mtval = state->instruction.value;
    }

    state->csr.mstatus.mpp = 3; // Previous privilege mode: M
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

#if (RVE_E_HOOK == 1 && RVE_E_ZICSR == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "_trap";
    RiscvEmulatorHook(state, &hc);
#endif

    if (state->trapflag.illegalinstruction == 1) {
        RiscvEmulatorIllegalInstruction(state);
    }

    state->trapflag.value = 0;
}

#endif