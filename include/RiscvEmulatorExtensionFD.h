/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionFD_H_
#define RiscvEmulatorExtensionFD_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_F == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorHook.h"
#include "RiscvEmulatorType.h"

/**
 * Single-precision floating-point load.
 */
static inline void RiscvEmulatorOpcodeLoadFloatingPoint(RiscvEmulatorState_t *state) {
    uint8_t rdnum = state->instruction.itype.rd;
    void *rd = &state->reg.f[rdnum];
    uint8_t rs1num = state->instruction.stype.rs1;
    void *rs1 = &state->reg.x[rs1num];

    int16_t imm = state->instruction.itype.imm;
    uint32_t memorylocation = imm + *(uint32_t *)rs1;

    // Check if the load is aligned.
    uint8_t length = sizeof(uint32_t);
    if (length > 1) {
        // Only the last few bits need to be checked.
        uint8_t memorylocation8 = memorylocation & 0xFF;
        if ((memorylocation8 % length) != 0) {
            state->trapflag.loadaddressmisaligned = 1;
            state->csr.mtval = memorylocation;
        }
    }

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "flw";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = imm;
    hc.immlength = (sizeof(imm));
    hc.immname = "offset";
    hc.memorylocation = memorylocation;
    hc.length = length;
    RiscvEmulatorHook(state, &hc);
#endif

    if (state->trapflag.loadaddressmisaligned == 1) {
        return;
    }

    uint32_t value = 0;
    RiscvEmulatorLoad(memorylocation, &value, length);

    *(uint32_t *)rd = (uint32_t)value;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Single-precision floating-point load.
 */
static inline void RiscvEmulatorOpcodeStoreFloatingPoint(RiscvEmulatorState_t *state) {
    // Untangle the immediate bits.
    RiscvInstructionTypeSDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm4_0 = state->instruction.stype.imm4_0;
    immdecoder.bit.imm11_5 = state->instruction.stype.imm11_5;
    int16_t offset = immdecoder.imm;

    uint8_t rs1num = state->instruction.stype.rs1;
    void *rs1 = &state->reg.x[rs1num];
    uint8_t rs2num = state->instruction.stype.rs2;
    void *rs2 = &state->reg.f[rs2num];

    uint32_t memorylocation = offset + *(uint32_t *)rs1;

#if (RVE_E_HOOK == 1)
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "unkown";
#endif

    uint8_t length = 0;
    length = sizeof(uint32_t);

    // Check if the store is aligned.
    // Only the last few bits need to be checked.
    uint8_t memorylocation8 = memorylocation & 0xFF;
    if ((memorylocation8 % length) != 0) {
        state->trapflag.storeaddressmisaligned = 1;
        state->csr.mtval = memorylocation;
    }

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    hc.instruction = "fsw";
    hc.hook = HOOK_BEGIN;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    hc.imm = offset;
    hc.immname = "offset";
    hc.immlength = sizeof(offset);
    hc.immissigned = 1;
    hc.memorylocation = memorylocation;
    hc.length = length;
    RiscvEmulatorHook(state, &hc);
#endif

#if (RVE_E_ZICSR == 1)
    if (state->trapflag.storeaddressmisaligned == 1) {
        return;
    }
#endif

    RiscvEmulatorStore(memorylocation, rs2, length);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Add: fd = fs1 + fs2
 */
static inline void RiscvEmulatorFADD_S(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t fdnum,
    void *fd,
    const uint8_t fs1num __attribute__((unused)),
    const void *fs1,
    const uint8_t fs2num __attribute__((unused)),
    const void *fs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "fadd.s";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = fdnum;
    hc.rd = fd;
    hc.rs1num = fs1num;
    hc.rs1 = fs1;
    hc.rs2num = fs2num;
    hc.rs2 = fs2;
    RiscvEmulatorHook(state, &hc);
#endif

    *(float *)fd = *(float *)fs1 + *(float *)fs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Process floating point operation opcodes.
 */
static inline void RiscvEmulatorOpcodeOperationFloatingPoint(RiscvEmulatorState_t *state) {
    int8_t detectedUnknownInstruction = 1;

    uint8_t fdnum = state->instruction.rtype.rd;
    void *fd = &state->reg.f[fdnum];
    uint8_t fs1num = state->instruction.rtype.rs1;
    void *fs1 = &state->reg.f[fs1num];
    uint8_t fs2num = state->instruction.rtype.rs2;
    void *fs2 = &state->reg.f[fs2num];

    if (detectedUnknownInstruction == 1) {
        detectedUnknownInstruction = -1;
        switch (state->instruction.rtype.funct7) {
            case FUNCT7_OPERATION_FADDS:
                RiscvEmulatorFADD_S(state, fdnum, fd, fs1num, fs1, fs2num, fs2);
                break;

            default:
                detectedUnknownInstruction = 1;
                break;
        }
    }

    if (detectedUnknownInstruction == 1) {
        state->trapflag.illegalinstruction = 1;
    }
}

#endif

#endif