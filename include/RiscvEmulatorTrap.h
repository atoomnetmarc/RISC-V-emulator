/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTrap_H_
#define RiscvEmulatorTrap_H_

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDebug.h"
#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorDisasm.h"
#include "RiscvEmulatorHook.h"

/**
 * Enter a trap: set mcause, mepc, mstatus and the trap vector.
 *
 * @param cause The exception code or interrupt cause number.
 * @param isinterrupt Nonzero for an asynchronous interrupt.
 *
 * The caller sets state->programcounter to the address that mepc must hold:
 * the faulting instruction for an exception, or the next instruction for an
 * interrupt.
 */
static inline void RiscvEmulatorTrapEnter(RiscvEmulatorState_t *state __attribute__((unused)), uint32_t cause __attribute__((unused)), uint8_t isinterrupt __attribute__((unused))) {
#if (RVE_E_ZICSR == 1)
    state->csr.mcause.interrupt = (uint8_t)(isinterrupt & 1u);
    state->csr.mcause.exceptioncode = (uint32_t)(cause & MCAUSE_EXCEPTION_CODE_MASK);
    state->csr.mepc = state->programcounter;

    state->csr.mstatus.mpp = 3; // Previous privilege mode: M
    state->csr.mstatus.mpie = state->csr.mstatus.mie;
    state->csr.mstatus.mie = 0;

    // Jump to the trap handler. Vectored mode offsets interrupts only.
    uint32_t base = (uint32_t)state->csr.mtvec.base << 2;
    if ((state->csr.mtvec.mode == 1) && (isinterrupt != 0)) {
        state->programcounternext = base + (4u * cause);
    } else {
        state->programcounternext = base;
    }
#endif

#if ((RVE_E_HOOK == 1 || RVE_E_DISASM == 1) && RVE_E_ZICSR == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "_trap";
    RiscvEmulatorHook(state, &hc);
#endif
#if (RVE_E_DISASM == 1 && RVE_E_ZICSR == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintTrap(state);
#endif
}

/**
 * Handle a synchronous trap.
 */
static inline void RiscvEmulatorTrap(RiscvEmulatorState_t *state) {
#if (RVE_E_ZICSR == 1)
    uint32_t cause = 0;

    // Instruction address misaligned
    if (state->trapflag.instructionaddressmisaligned == 1) {
        cause = MCAUSE_EXCEPTION_CODE_INSTRUCTION_ADDRESS_MISALIGNED;
    }

    // Breakpoint
    if (state->trapflag.breakpoint == 1) {
        cause = MCAUSE_EXCEPTION_CODE_BREAKPOINT;
    }

    // Load address misaligned
    if (state->trapflag.loadaddressmisaligned == 1) {
        cause = MCAUSE_EXCEPTION_CODE_LOAD_ADDRESS_MISALIGNED;
    }

    // Store/AMO address misaligned
    if (state->trapflag.storeaddressmisaligned == 1) {
        cause = MCAUSE_EXCEPTION_CODE_STORE_ADDRESS_MISALIGNED;
    }

    //  Environment call from M-mode
    if (state->trapflag.environmentcallfrommmode == 1) {
        cause = MCAUSE_EXCEPTION_CODE_ENVIRONMENT_CALL_FROM_MMODE;
    }

    // Illegal instruction
    if (state->trapflag.illegalinstruction == 1) {
        cause = MCAUSE_EXCEPTION_CODE_ILLEGAL_INSTRUCTION;
        state->csr.mtval = state->instruction.value;
    }

    RiscvEmulatorTrapEnter(state, cause, 0);
#endif

    if (state->trapflag.illegalinstruction == 1) {
        RiscvEmulatorIllegalInstruction(state);
    }

    state->trapflag.value = 0;
}

#endif