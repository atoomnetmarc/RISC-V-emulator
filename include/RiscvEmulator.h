/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulator_H_
#define RiscvEmulator_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorExtension.h"
#include "RiscvEmulatorTrap.h"
#include "RiscvEmulatorTrapFlag.h"
#include "RiscvEmulatorType.h"

/**
 * Initialize the emulator.
 *
 * @param ram_length The size in bytes of the RAM available.
 */
static inline void RiscvEmulatorInit(RiscvEmulatorState_t *state, uint32_t ram_length) {
    // Initialize stack pointer.
    state->reg.sp = RAM_ORIGIN + ram_length;

    // Initialize program counter.
    state->programcounter = ROM_ORIGIN;
    state->programcounternext = ROM_ORIGIN;

    // Initialize X0.
    state->reg.Zero = 0;

#if (RVE_E_ZICSR == 1)
    // The hart starts running, not waiting.
    state->waiting = 0;

    // The hart starts in M-mode.
    state->privilege = 3;

    // Initialize CSR.
    memset(&state->csr, 0, sizeof(state->csr));

    // misa reports the maximum set of implemented extensions at reset.
    state->csr.misa.extensions = RVE_CSR_MISA_VALUE;
    state->csr.misa.mxl = 1; // RV32
#endif

    // Initialize the pending trap.
    state->trap.pending = 0;
    state->trap.tval = 0;
}

/**
 * Returns non-zero when the hart is parked in WFI.
 */
static inline uint8_t RiscvEmulatorIsWaiting(const RiscvEmulatorState_t *state) {
#if (RVE_E_ZICSR == 1)
    return state->waiting;
#else
    (void)state;
    return 0;
#endif
}

/**
 * Call this function repeatedly to execute the emulator one instruction at a time.
 */
static inline void RiscvEmulatorLoop(RiscvEmulatorState_t *state) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    // Detect if the executed instruction was handled by a hook or disassembly.
    state->instructionhandled = 0;
#endif

    state->programcounter = state->programcounternext;

#if (RVE_E_ZICSR == 1)
    // WFI wake-up. The spec requires resuming on a locally enabled pending
    // interrupt regardless of mstatus.mie (machine.adoc:2718).
    if (state->waiting != 0) {
        if ((state->csr.mip.mip & state->csr.mie.mie) == 0) {
            return; // Hart is idle; no instruction retires.
        }
        state->waiting = 0;
    }

    // Take the highest-priority pending interrupt before fetching the next
    // instruction. M-mode priority: MEI, MSI, MTI. M-mode interrupts are
    // always globally enabled in a less-privileged mode.
    if ((state->privilege < 3) || (state->csr.mstatus.mie != 0)) {
        uint32_t pending = state->csr.mip.mip & state->csr.mie.mie;
        uint32_t cause = 0;
        if ((pending & (1u << 11)) != 0) {
            cause = 11; // Machine external interrupt.
        } else if ((pending & (1u << 3)) != 0) {
            cause = 3; // Machine software interrupt.
        } else if ((pending & (1u << 7)) != 0) {
            cause = 7; // Machine timer interrupt.
        }
        if (cause != 0) {
            RiscvEmulatorTrapEnter(state, cause, 1);
#if (RVE_E_ZICNTR == 1)
            state->csr.cycle++;
#if defined(RVE_TIME_SOURCE)
            state->csr.time = RVE_TIME_SOURCE(state);
#else
            state->csr.time++;
#endif
#endif
            return;
        }
    }

    // The fetch validates the target address. This catches MRET and any other
    // PC source that does not check alignment itself (machine.adoc:1870).
    if ((state->programcounter & RVE_IALIGN_MASK) != 0) {
        RiscvEmulatorRaiseException(state,
                                    MCAUSE_EXCEPTION_CODE_INSTRUCTION_ADDRESS_MISALIGNED, state->programcounter);
        RiscvEmulatorTrap(state);
        return;
    }
#endif

    uint8_t instructionlength = 32;

#if (RVE_E_C == 1)
    // Read 16 bits.
    state->instruction.H = 0;
    if (!RiscvEmulatorLoadVirtual(
            state,
            state->programcounter,
            &state->instruction.L,
            sizeof(state->instruction.L),
            RVE_SV32_ACCESS_FETCH)) {
        RiscvEmulatorRaiseException(state,
                                    MCAUSE_EXCEPTION_CODE_INSTRUCTION_ACCESS_FAULT, state->programcounter);
        RiscvEmulatorTrap(state);
        return;
    }

    state->programcounternext += sizeof(state->instruction.L);

    // Read another 16 bits when this is a 32-bit instruction.
    if (state->instruction.copcode.op == OPCODE16_QUADRANT_INVALID) {
        if (!RiscvEmulatorLoadVirtual(
                state,
                state->programcounternext,
                &state->instruction.H,
                sizeof(state->instruction.L),
                RVE_SV32_ACCESS_FETCH)) {
            RiscvEmulatorRaiseException(state,
                                        MCAUSE_EXCEPTION_CODE_INSTRUCTION_ACCESS_FAULT, state->programcounternext);
            RiscvEmulatorTrap(state);
            return;
        }

        state->programcounternext += sizeof(state->instruction.L);
    } else {
        instructionlength = 16;
    }
#else
    // Read 32 bits.
    if (!RiscvEmulatorLoadVirtual(state, state->programcounter, &state->instruction.value, sizeof(state->instruction.value), RVE_SV32_ACCESS_FETCH)) {
        RiscvEmulatorRaiseException(state,
                                    MCAUSE_EXCEPTION_CODE_INSTRUCTION_ACCESS_FAULT, state->programcounter);
        RiscvEmulatorTrap(state);
        return;
    }
    state->programcounternext += sizeof(state->instruction.value);
#endif

#if (RVE_E_C == 1)
    if (instructionlength == 16) {
        RiscvEmulatorOpcodeCompressed(state, state->instruction.copcode.op, state->instruction.copcode.funct3);
    }
#endif

    if (instructionlength == 32) {
        uint8_t rdnum = state->instruction.rtype.rd;
        uint8_t rs1num = state->instruction.rtype.rs1;
        uint8_t rs2num = state->instruction.rtype.rs2;
        uint8_t funct3 = state->instruction.rtype.funct3;
        uint8_t funct7 = state->instruction.rtype.funct7;
        int16_t imm = state->instruction.itype.imm;
        void *rd = &state->reg.x[rdnum];
        void *rs1 = &state->reg.x[rs1num];
        void *rs2 = &state->reg.x[rs2num];

        switch (state->instruction.opcode) {
            case OPCODE32_JUMPANDLINKREGISTER:
                RiscvEmulatorOpcodeJumpAndLinkRegister(state, rdnum, rd, rs1num, rs1, funct3, imm);
                break;
            case OPCODE32_OPERATION:
                RiscvEmulatorOpcodeOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, funct3, funct7);
                break;
            case OPCODE32_IMMEDIATE:
                RiscvEmulatorOpcodeImmediate(state, rdnum, rd, rs1num, rs1, funct3, imm);
                break;
            case OPCODE32_LOAD:
                RiscvEmulatorOpcodeLoad(state, rdnum, rd, rs1num, rs1, imm, funct3);
                break;
            case OPCODE32_STORE:
                RiscvEmulatorOpcodeStore(state, rs1num, rs1, rs2num, rs2, funct3);
                break;
            case OPCODE32_BRANCH:
                RiscvEmulatorOpcodeBranch(state, rs1num, rs1, rs2num, rs2, funct3);
                break;
            case OPCODE32_ADDUPPERIMMEDIATE2PC:
                RiscvEmulatorAUIPC(state, rdnum, rd);
                break;
            case OPCODE32_LOADUPPERIMMEDIATE:
                RiscvEmulatorLUI(state, rdnum, rd);
                break;
            case OPCODE32_JUMPANDLINK:
                RiscvEmulatorJAL(state, rdnum, rd);
                break;
            case OPCODE32_SYSTEM:
                RiscvEmulatorOpcodeSystem(state, rdnum, rd, rs1num, rs1, funct3);
                break;
            case OPCODE32_MISCMEM:
                RiscvEmulatorOpcodeMiscMem(state, funct3);
                break;
#if ((RVE_E_ZAAMO == 1) || (RVE_E_ZALRSC == 1))
            case OPCODE32_ATOMICMEMORYOPERATION:
                RiscvEmulatorOpcodeAtomicMemoryOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, funct3);
                break;
#endif
            default:
                RiscvEmulatorTrapFlagIllegalInstruction(state);
                break;
        }
    }

#if (RVE_E_ZICNTR == 1)
    // Update the counters. Zicntr implies Zicsr, so the CSR struct always
    // exists when this extension is enabled.
    state->csr.cycle++;
#if defined(RVE_TIME_SOURCE)
    // time is a shadow of the platform clock (or CLINT mtime), not an
    // instruction counter (zicntr.adoc:116).
    state->csr.time = RVE_TIME_SOURCE(state);
#else
    state->csr.time++;
#endif
    state->csr.instret++;
#endif

    if (state->trap.pending != 0) {
        RiscvEmulatorTrap(state);
    }
}

#endif