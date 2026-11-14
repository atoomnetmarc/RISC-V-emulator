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
static inline void RiscvEmulatorTrapEnterMachine(RiscvEmulatorState_t *state __attribute__((unused)), uint32_t cause __attribute__((unused)), uint8_t isinterrupt __attribute__((unused))) {
#if (RVE_E_ZICSR == 1)
    state->csr.mcause.interrupt = (uint8_t)(isinterrupt & 1u);
    state->csr.mcause.exceptioncode = (uint32_t)(cause & MCAUSE_EXCEPTION_CODE_MASK);
    state->csr.mepc = state->programcounter;

    // Previous privilege mode: U (0), S (1) or M (3).
    state->csr.mstatus.mpp = (state->privilege == 0) ? 0 : ((state->privilege == 1) ? 1 : 3);
    state->csr.mstatus.mpie = state->csr.mstatus.mie;
    state->csr.mstatus.mie = 0;
    state->privilege = 3; // Traps enter M-mode.

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

#if (RVE_E_SV32 == 1)
/**
 * Enter a trap in supervisor mode: set scause, sepc, sstatus and stvec.
 */
static inline void RiscvEmulatorTrapEnterSupervisor(RiscvEmulatorState_t *state, uint32_t cause, uint8_t isinterrupt) {
#if (RVE_E_ZICSR == 1)
    state->csr.scause.interrupt = (uint8_t)(isinterrupt & 1u);
    state->csr.scause.exceptioncode = (uint32_t)(cause & MCAUSE_EXCEPTION_CODE_MASK);
    state->csr.sepc = state->programcounter;

    state->csr.mstatus.spp = (state->privilege == 0) ? 0 : 1;
    state->csr.mstatus.spie = state->csr.mstatus.sie;
    state->csr.mstatus.sie = 0;
    state->privilege = 1;

    // Jump to the trap handler. Vectored mode offsets interrupts only.
    uint32_t base = (uint32_t)state->csr.stvec.base << 2;
    if ((state->csr.stvec.mode == 1) && (isinterrupt != 0)) {
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
 * Return non-zero when a trap is delegated to supervisor mode.
 */
static inline uint8_t RiscvEmulatorTrapDelegated(RiscvEmulatorState_t *state, uint32_t cause, uint8_t isinterrupt) {
    if (state->privilege == 3) {
        return 0;
    }
    if (isinterrupt != 0) {
        return (uint8_t)((state->csr.mideleg.interrupts >> cause) & 1u);
    }
    return (uint8_t)((state->csr.medeleg.synchronousexceptions >> cause) & 1u);
}
#endif

/**
 * Enter a trap in machine or supervisor mode, depending on the delegation.
 */
static inline void RiscvEmulatorTrapEnter(RiscvEmulatorState_t *state, uint32_t cause, uint8_t isinterrupt) {
#if (RVE_E_ZICSR == 1)
#if (RVE_E_SV32 == 1)
    if (RiscvEmulatorTrapDelegated(state, cause, isinterrupt) != 0) {
        state->csr.stval = (isinterrupt != 0) ? 0 : state->trap.tval;
        RiscvEmulatorTrapEnterSupervisor(state, cause, isinterrupt);
        return;
    }
#endif
    state->csr.mtval = (isinterrupt != 0) ? 0 : state->trap.tval;
#endif
    RiscvEmulatorTrapEnterMachine(state, cause, isinterrupt);
}

/**
 * Handle a synchronous trap.
 *
 * The exception cause and mtval were recorded by RiscvEmulatorRaiseException()
 * while the instruction was executing. Only one exception can be pending.
 */
static inline void RiscvEmulatorTrap(RiscvEmulatorState_t *state) {
#if (RVE_E_ZICSR == 1)
    if (state->trap.pending != 0) {
        RiscvEmulatorTrapEnter(state, state->trap.pending, 0);
    }

    if (state->trap.pending == MCAUSE_EXCEPTION_CODE_ILLEGAL_INSTRUCTION) {
        RiscvEmulatorIllegalInstruction(state);
    }
#else
    // Without Zicsr there is no trap handler, so any exception is fatal.
    if (state->trap.pending != 0) {
        RiscvEmulatorIllegalInstruction(state);
    }
#endif

    state->trap.pending = 0;
}

#endif