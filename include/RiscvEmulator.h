/*

Copyright 2023 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulator_H
#define RiscvEmulator_H

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

#include "RiscvEmulatorRv32I.h"

/**
 * @param ram_length The size in bytes of the RAM available.
 */
inline void RiscvEmulatorInit(RiscvEmulatorState_t *state, uint32_t ram_length)
{
    // Initialize stack pointer.
    state->registers.symbolic.sp = RAM_ORIGIN + ram_length;

    // Initialize program counter.
    state->programcounter = ROM_ORIGIN;

    // Initialize X0.
    state->registers.symbolic.Zero = 0;
}

/**
 * Calling this function frequently executes the emulator.
 */
inline void RiscvEmulatorLoop(RiscvEmulatorState_t *state)
{
    uint32_t programcounternext = state->programcounter + 4;

    state->instruction.value = RiscvEmulatorLoadInstruction(state->programcounter);

    switch (state->instruction.opcode.opcode)
    {
        case OPCODE_JUMPANDLINKREGISTER:
            RiscvEmulatorOpcodeJumpAndLinkRegister(state, &programcounternext);
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
            RiscvEmulatorOpcodeBranch(state, &programcounternext);
            break;
        case OPCODE_ADDUPPERIMMEDIATE2PC:
            RiscvEmulatorAUIPC(state);
            break;
        case OPCODE_LOADUPPERIMMEDIATE:
            RiscvEmulatorLUI(state);
            break;
        case OPCODE_JUMPANDLINK:
            RiscvEmulatorJAL(state, &programcounternext);
            break;
        default:
            RiscvEmulatorUnknownInstruction(state);
            break;
    }

    state->programcounter = programcounternext;
}

#endif