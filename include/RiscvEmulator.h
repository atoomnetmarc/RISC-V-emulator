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
    RiscvEmulatorLoad(
        state->programcounter,
        &state->instruction.L,
        sizeof(state->instruction.L));

    state->programcounternext += sizeof(state->instruction.L);

    // Read another 16 bits when this is a 32-bit instruction.
    if (state->instruction.copcode.op == OPCODE16_QUADRANT_INVALID) {
        RiscvEmulatorLoad(
            state->programcounternext,
            &state->instruction.H,
            sizeof(state->instruction.L));

        state->programcounternext += sizeof(state->instruction.L);
    } else {
        instructionlength = 16;
    }
#else
    // Read 32 bits.
    RiscvEmulatorLoad(state->programcounter, &state->instruction.value, sizeof(state->instruction.value));
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
        void *rd = &state->reg.x[rdnum];
        void *rs1 = &state->reg.x[rs1num];
        void *rs2 = &state->reg.x[rs2num];

        switch (state->instruction.opcode) {
            case OPCODE32_JUMPANDLINKREGISTER:
                RiscvEmulatorOpcodeJumpAndLinkRegister(state, rdnum, rd, rs1num, rs1, funct3);
                break;
            case OPCODE32_OPERATION:
                RiscvEmulatorOpcodeOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, funct3, funct7);
                break;
            case OPCODE32_IMMEDIATE:
                RiscvEmulatorOpcodeImmediate(state, rdnum, rd, rs1num, rs1);
                break;
            case OPCODE32_LOAD:
                RiscvEmulatorOpcodeLoad(state, rdnum, rd, rs1num, rs1, (int16_t)state->instruction.itype.imm, funct3);
                break;
            case OPCODE32_STORE:
                RiscvEmulatorOpcodeStore(state, rs1num, rs1, rs2num, rs2);
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
                state->trapflag.illegalinstruction = 1;
                break;
        }
    }

#if (RVE_E_ZICNTR == 1)
    // Update the counters. Zicntr implies Zicsr, so the CSR struct always
    // exists when this extension is enabled.
    state->csr.cycle++;
    state->csr.time++;
    state->csr.instret++;
#endif

    if (state->trapflag.value > 0) {
        RiscvEmulatorTrap(state);
    }
}

#endif