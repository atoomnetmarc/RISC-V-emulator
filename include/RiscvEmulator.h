/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulator_H_
#define RiscvEmulator_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorExtension.h"
#include "RiscvEmulatorTrap.h"
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
    // Initialize CSR.
    memset(&state->csr, 0, sizeof(state->csr));
#endif

    // Initialize trap flags.
    state->trapflag.value = 0;
}

/**
 * Call this function repeatedly to execute the emulator one instruction at a time.
 */
static inline void RiscvEmulatorLoop(RiscvEmulatorState_t *state) {

#if (RVE_E_HOOK == 1)
    // Detect if hook exists for the instruction executed. Will be set to 1 when executing a hook.
    state->hookexists = 0;
#endif

    state->programcounter = state->programcounternext;

    uint8_t instructionlength = 32;

#if (RVE_E_C == 1)
    // Read 16 bits.
    state->instruction.H = 0;
    RiscvEmulatorLoadInstruction(
        state->programcounter,
        &state->instruction.L,
        sizeof(state->instruction.L));

    state->programcounternext += sizeof(state->instruction.L);

    // Read another 16 bits when this is a 32-bit instruction.
    if (state->instruction.copcode.op == OPCODE16_QUADRANT_INVALID) {
        RiscvEmulatorLoadInstruction(
            state->programcounternext,
            &state->instruction.H,
            sizeof(state->instruction.L));

        state->programcounternext += sizeof(state->instruction.L);
    } else {
        instructionlength = 16;
    }
#else
    // Read 32 bits.
    RiscvEmulatorLoadInstruction(state->programcounter, &state->instruction.value, sizeof(state->instruction.value));
    state->programcounternext += sizeof(state->instruction.value);
#endif

#if (RVE_E_C == 1)
    if (instructionlength == 16) {
        RiscvEmulatorOpcodeCompressed(state);
    }
#endif

    if (instructionlength == 32) {
        switch (state->instruction.opcode) {
            case OPCODE32_JUMPANDLINKREGISTER:
                RiscvEmulatorOpcodeJumpAndLinkRegister(state);
                break;
            case OPCODE32_OPERATION:
                RiscvEmulatorOpcodeOperation(state);
                break;
            case OPCODE32_IMMEDIATE:
                RiscvEmulatorOpcodeImmediate(state);
                break;
            case OPCODE32_LOAD:
                RiscvEmulatorOpcodeLoad(state);
                break;
            case OPCODE32_STORE:
                RiscvEmulatorOpcodeStore(state);
                break;
            case OPCODE32_BRANCH:
                RiscvEmulatorOpcodeBranch(state);
                break;
            case OPCODE32_ADDUPPERIMMEDIATE2PC:
                RiscvEmulatorAUIPC(state);
                break;
            case OPCODE32_LOADUPPERIMMEDIATE:
                RiscvEmulatorLUI(state);
                break;
            case OPCODE32_JUMPANDLINK:
                RiscvEmulatorJAL(state);
                break;
            case OPCODE32_SYSTEM:
                RiscvEmulatorOpcodeSystem(state);
                break;
            case OPCODE32_MISCMEM:
                RiscvEmulatorOpcodeMiscMem(state);
                break;
#if (RVE_E_A == 1)
            case OPCODE32_ATOMICMEMORYOPERATION:
                RiscvEmulatorOpcodeAtomicMemoryOperation(state);
                break;
#endif
            default:
                state->trapflag.illegalinstruction = 1;
                break;
        }
    }

    if (state->trapflag.value > 0) {
        RiscvEmulatorTrap(state);
    }
}

#endif