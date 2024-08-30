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
    state->registers.symbolic.sp = RAM_ORIGIN + ram_length;

    // Initialize program counter.
    state->programcounter = ROM_ORIGIN;
    state->programcounternext = ROM_ORIGIN;

    // Initialize X0.
    state->registers.symbolic.Zero = 0;

#if (RVE_E_ZICSR == 1)
    // Initialize CSR.
    memset(&state->csr, 0, sizeof(state->csr));
#endif

    // Initialize trap flags.
    state->trapflags.value = 0;
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
            state->trapflags.bits.illegalinstruction = 1;
            break;
    }

#if (RVE_E_ZICSR == 1)
    // Check if programcounternext is aligned.
    // Must be changed from %4 to %2 when compressed instructions are added to the emulator.
    uint8_t programcounternext8 = state->programcounternext & 0xFF;
    if ((programcounternext8 % 4) != 0) {
        state->trapflags.bits.instructionaddressmisaligned = 1;
        state->csr.mtval = state->programcounternext;
    }
#endif

    if (state->trapflags.value > 0) {
        RiscvEmulatorTrap(state);
    }
}

#endif