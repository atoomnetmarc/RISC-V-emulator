/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulator_H
#define RiscvEmulator_H

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorExtension.h"
#include "RiscvEmulatorType.h"

/**
 * Initialize the emulator.
 *
 * @param ram_length The size in bytes of the RAM available.
 */
static inline void RiscvEmulatorInit(RiscvEmulatorState_t *state, uint32_t ram_length) {
    // Initialize stack pointer.
    state->registers.symbolic.sp = RAM_ORIGIN + ram_length;

    // Initialize program counter.
    state->programcounter = ROM_ORIGIN;
    state->programcounternext = ROM_ORIGIN;

    // Initialize X0.
    state->registers.symbolic.Zero = 0;

#if (RVE_E_ZICSR == 1)
    // Initialize CSR
    memset(&state->csr, 0, sizeof(state->csr));
#endif
}

/**
 * Call this function repeatedly to execute the emulator one instruction at a time.
 */
static inline void RiscvEmulatorLoop(RiscvEmulatorState_t *state) {
    state->programcounter = state->programcounternext;
    state->programcounternext = state->programcounter + 4;

    state->instruction.value = RiscvEmulatorLoadInstruction(state->programcounter);

    switch (state->instruction.opcode.opcode) {
        case OPCODE_JUMPANDLINKREGISTER:
            RiscvEmulatorOpcodeJumpAndLinkRegister(state);
            break;
        case OPCODE_OPERATION:
            RiscvEmulatorOpcodeOperation(state);
            break;
        case OPCODE_IMMEDIATE:
            RiscvEmulatorOpcodeImmediate(state);
            break;
        case OPCODE_LOAD:
            RiscvEmulatorOpcodeLoad(state);
            break;
        case OPCODE_STORE:
            RiscvEmulatorOpcodeStore(state);
            break;
        case OPCODE_BRANCH:
            RiscvEmulatorOpcodeBranch(state);
            break;
        case OPCODE_ADDUPPERIMMEDIATE2PC:
            RiscvEmulatorAUIPC(state);
            break;
        case OPCODE_LOADUPPERIMMEDIATE:
            RiscvEmulatorLUI(state);
            break;
        case OPCODE_JUMPANDLINK:
            RiscvEmulatorJAL(state);
            break;
        case OPCODE_SYSTEM:
            RiscvEmulatorOpcodeSystem(state);
            break;
        case OPCODE_MISCMEM:
            RiscvEmulatorOpcodeMiscMem(state);
            break;
#if (RVE_E_A == 1)
        case OPCODE_ATOMICMEMORYOPERATION:
            RiscvEmulatorOpcodeAtomicMemoryOperation(state);
            break;
#endif
        default:
            RiscvEmulatorUnknownInstruction(state);
            break;
    }
}

#endif