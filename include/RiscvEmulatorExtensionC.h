/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionC_H_
#define RiscvEmulatorExtensionC_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_C == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorHook.h"
#include "RiscvEmulatorType.h"

/**
 * rd = (*sp + nzuimm)
 */
static inline void RiscvEmulatorC_ADDI4SPN(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    void *sp,
    const uint16_t nzuimm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.addi4spn";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = nzuimm;
    hc.immlength = sizeof(nzuimm);
    hc.immname = "nzuimm";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)sp + nzuimm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Load word from memory to rd.
 */
static inline void RiscvEmulatorC_LW(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const uint8_t offset) {

    uint8_t length = sizeof(uint32_t);

    uint32_t memorylocation = *(int32_t *)rs1 + offset;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.lw";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = offset;
    hc.immlength = sizeof(offset);
    hc.immname = "offset";
    hc.memorylocation = memorylocation;
    hc.length = length;
    RiscvEmulatorHook(state, &hc);
#endif

#if (RVE_E_ZICSR == 1)
    if (state->trapflag.storeaddressmisaligned == 1) {
        return;
    }
#endif

    RiscvEmulatorLoad(memorylocation, rd, length);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Store word in rs2 to memory.
 */
static inline void RiscvEmulatorC_SW(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    void *rs2,
    const uint8_t offset) {

    uint8_t length = sizeof(uint32_t);

    uint32_t memorylocation = *(int32_t *)rs1 + offset;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.sw";
    hc.hook = HOOK_BEGIN;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    hc.imm = offset;
    hc.immlength = sizeof(offset);
    hc.immname = "offset";
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
 * rd = rd + nzimm
 */
static inline void RiscvEmulatorC_ADDI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const int8_t nzimm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.addi";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = nzimm;
    hc.immlength = sizeof(nzimm);
    hc.immissigned = 1;
    hc.immname = "nzimm";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rd + nzimm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * pc += offset
 */
static inline void RiscvEmulatorC_JAL(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *ra,
    const int16_t offset) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.jal";
    hc.hook = HOOK_BEGIN;
    hc.imm = offset;
    hc.immlength = sizeof(offset);
    hc.immissigned = 1;
    hc.immname = "offset";
    RiscvEmulatorHook(state, &hc);
#endif

    *(uint32_t *)ra = state->programcounter + 2;
    state->programcounternext = state->programcounter + offset;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * pc += rs1
 */
static inline void RiscvEmulatorC_JALR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    void *ra) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.instruction = "c.jalr";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#endif

    uint32_t originalvaluers1 = *(int32_t *)rs1;

    *(uint32_t *)ra = state->programcounter + 2;
    state->programcounternext = (originalvaluers1 & (UINT32_MAX - 1));

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * pc += offset
 */
static inline void RiscvEmulatorC_J(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const int16_t offset) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.j";
    hc.hook = HOOK_BEGIN;
    hc.imm = offset;
    hc.immlength = sizeof(offset);
    hc.immissigned = 1;
    hc.immname = "offset";
    RiscvEmulatorHook(state, &hc);
#endif

    state->programcounternext = state->programcounter + offset;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * pc += rs1
 */
static inline void RiscvEmulatorC_JR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    void *rs1) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.jr";
    hc.hook = HOOK_BEGIN;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
#endif

    state->programcounternext = *(int32_t *)rs1 & (UINT32_MAX - 1);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Branch if rs1 == 0
 */
static inline void RiscvEmulatorC_BEQZ(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const int16_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.beqz";
    hc.hook = HOOK_BEGIN;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = imm;
    hc.immlength = sizeof(imm);
    hc.immissigned = 1;
    hc.immname = "offset";
    RiscvEmulatorHook(state, &hc);
#endif

    if (*(int32_t *)rs1 == 0) {
        state->programcounternext = state->programcounter + imm;
    };

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Branch if rs1 != 0
 */
static inline void RiscvEmulatorC_BNEZ(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const int16_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.bnez";
    hc.hook = HOOK_BEGIN;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = imm;
    hc.immlength = sizeof(imm);
    hc.immissigned = 1;
    hc.immname = "offset";
    RiscvEmulatorHook(state, &hc);
#endif

    if (*(int32_t *)rs1 != 0) {
        state->programcounternext = state->programcounter + imm;
    };

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Logical shift left: rd = rs1 << shamt
 */
static inline void RiscvEmulatorC_SLLI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t shamt) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.slli";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rd << shamt;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * rd = imm
 */
static inline void RiscvEmulatorC_LI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const int8_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.li";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = imm;
    hc.immlength = sizeof(imm);
    hc.immissigned = 1;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = imm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * sp += imm*16
 */
static inline void RiscvEmulatorC_ADDI16SP(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd) {

    RiscvInstructionTypeCIAddi16spDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm4 = state->instruction.ciaddi16sp.imm4;
    immdecoder.bit.imm5 = state->instruction.ciaddi16sp.imm5;
    immdecoder.bit.imm6 = state->instruction.ciaddi16sp.imm6;
    immdecoder.bit.imm8_7 = state->instruction.ciaddi16sp.imm8_7;
    immdecoder.bit.imm9 = state->instruction.ciaddi16sp.imm9;

    int16_t nzimm = immdecoder.imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.addi16sp";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = nzimm;
    hc.immissigned = 1;
    hc.immlength = sizeof(nzimm);
    hc.immname = "nzimm";
    RiscvEmulatorHook(state, &hc);
#endif

    if (nzimm == 0) {
        return;
    }

    *(int32_t *)rd += nzimm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Load upper with immediate.
 */
static inline void RiscvEmulatorC_LUI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd) {

    RiscvInstructionTypeCILuiDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm16_12 = state->instruction.cilui.imm16_12;
    immdecoder.bit.imm17 = state->instruction.cilui.imm17;
    int32_t nzimm = immdecoder.imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.lui";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = nzimm;
    hc.immissigned = 1;
    hc.immname = "nzimm";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = nzimm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Logical shift right: rd = rd >> shamt
 */
static inline void RiscvEmulatorC_SRLI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    uint8_t shamt) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.srli";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rd >> shamt;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Arithmetic shift right: rd = rs1 >> shamt
 */
static inline void RiscvEmulatorC_SRAI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    uint8_t shamt) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.srai";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rd >> shamt;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * rd = rd & imm.
 */
static inline void RiscvEmulatorC_ANDI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    int8_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.andi";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = imm;
    hc.immlength = sizeof(imm);
    hc.immissigned = 1;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rd & imm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * rd = rd - rs2.
 */
static inline void RiscvEmulatorC_SUB(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs2num __attribute__((unused)),
    void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.sub";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rd - *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Exclusive or: rd = rd ^ rs2
 */
static inline void RiscvEmulatorC_XOR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs2num __attribute__((unused)),
    void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.xor";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rd ^ *(uint32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Exclusive or: rd = rd | rs2
 */
static inline void RiscvEmulatorC_OR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs2num __attribute__((unused)),
    void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.or";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rd | *(uint32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Exclusive or: rd = rd & rs2
 */
static inline void RiscvEmulatorC_AND(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs2num __attribute__((unused)),
    void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.or";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rd & *(uint32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Load memorylocation (*sp + offset) into rd.
 */
static inline void RiscvEmulatorC_LWSP(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    void *sp,
    const uint8_t offset) {

    uint32_t memorylocation = *(int32_t *)sp + offset;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.lwsp";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = offset;
    hc.immlength = sizeof(offset);
    hc.immname = "offset";
    hc.memorylocation = memorylocation;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    RiscvEmulatorLoad(memorylocation, rd, sizeof(uint32_t));

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * rd = rs2
 */
static inline void RiscvEmulatorC_MV(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs2num __attribute__((unused)),
    void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.mv";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Cause control to be transferred back to a debugging environment.
 */
static inline void RiscvEmulatorC_EBREAK(RiscvEmulatorState_t *state) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.ebreak";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#endif

#if (RVE_E_ZICSR == 1)
    state->trapflag.breakpoint = 1;
    state->csr.mtval = state->programcounter;
#endif

    RiscvEmulatorHandleEBREAK(state);
}

/**
 * rd = rd + rs2
 */
static inline void RiscvEmulatorC_ADD(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs2num __attribute__((unused)),
    void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.add";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rd + *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Store rs2 to memorylocation (*sp + offset)
 */
static inline void RiscvEmulatorC_SWSP(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs2num __attribute__((unused)),
    void *rs2,
    void *sp,
    const uint8_t offset) {

    uint32_t memorylocation = *(int32_t *)sp + offset;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.swsp";
    hc.hook = HOOK_BEGIN;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    hc.imm = offset;
    hc.immlength = sizeof(offset);
    hc.immname = "offset";
    hc.memorylocation = memorylocation;
    RiscvEmulatorHook(state, &hc);
#endif

    RiscvEmulatorStore(memorylocation, rs2, sizeof(uint32_t));

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Process compressed opcodes.
 */
static inline void RiscvEmulatorOpcodeCompressed(RiscvEmulatorState_t *state) {
    RiscvInstructionTypeCDecoderOpcode_u decoderOpcode16 = {0};
    decoderOpcode16.funct3 = state->instruction.copcode.funct3;
    decoderOpcode16.op = state->instruction.copcode.op;
    uint8_t opfunct3 = decoderOpcode16.opfunct3;

    uint8_t funct3_funct2 = 0;
    uint8_t funct6_funct2 = 0;
    int8_t rdnum = -1;
    int8_t rs1num = -1;
    int8_t rs2num = -1;
    uint32_t imm = 0;
    void *rd = 0;
    void *rs1 = 0;
    void *rs2 = 0;
    void *sp = &state->reg.sp;
    void *ra = &state->reg.ra;

    // Whenever possible, combine decoding instruction bits for multiple instructions.
    switch (opfunct3) {
        case OPCODE16_ADDI4SPN: {
            RiscvInstructionTypeCIWDecoderImm_u RiscvInstructionTypeCIWDecoderImm = {0};
            RiscvInstructionTypeCIWDecoderImm.bit.imm2 = state->instruction.ciwtype.imm2;
            RiscvInstructionTypeCIWDecoderImm.bit.imm3 = state->instruction.ciwtype.imm3;
            RiscvInstructionTypeCIWDecoderImm.bit.imm5_4 = state->instruction.ciwtype.imm5_4;
            RiscvInstructionTypeCIWDecoderImm.bit.imm9_6 = state->instruction.ciwtype.imm9_6;
            imm = RiscvInstructionTypeCIWDecoderImm.imm;
            rdnum = state->instruction.ciwtype.rdp + 8;
            break;
        }
        case OPCODE16_MISCALU: {
            RiscvInstructionTypeCBDecoderFunct3Funct2_u RiscvInstructionTypeCBDecoderFunct3Funct2 = {0};
            RiscvInstructionTypeCBDecoderFunct3Funct2.funct3 = state->instruction.cbimm.funct3;
            RiscvInstructionTypeCBDecoderFunct3Funct2.funct2 = state->instruction.cbimm.funct2;
            funct3_funct2 = RiscvInstructionTypeCBDecoderFunct3Funct2.funct3_funct2;

            RiscvInstructionTypeCBImmDecoderImm_u RiscvInstructionTypeCBImmDecoderImm = {0};

            if (funct3_funct2 == FUNCT3_FUNCT2_SRLI ||
                funct3_funct2 == FUNCT3_FUNCT2_SRAI ||
                funct3_funct2 == FUNCT3_FUNCT2_ANDI) {
                RiscvInstructionTypeCBImmDecoderImm.bit.imm4_0 = state->instruction.cbimm.imm4_0;
                RiscvInstructionTypeCBImmDecoderImm.bit.imm5 = state->instruction.cbimm.imm5;
                imm = RiscvInstructionTypeCBImmDecoderImm.imm;

                rdnum = state->instruction.cbimm.rdp + 8;
                break;
            }

            RiscvInstructionTypeCADecoderFunct6Funct2_u RiscvInstructionTypeCADecoderFunct6Funct2 = {0};
            RiscvInstructionTypeCADecoderFunct6Funct2.funct6 = state->instruction.catype.funct6;
            RiscvInstructionTypeCADecoderFunct6Funct2.funct2 = state->instruction.catype.funct2;
            funct6_funct2 = RiscvInstructionTypeCADecoderFunct6Funct2.funct6_funct2;

            if (funct6_funct2 == FUNCT6_FUNCT2_SUB ||
                funct6_funct2 == FUNCT6_FUNCT2_XOR ||
                funct6_funct2 == FUNCT6_FUNCT2_OR ||
                funct6_funct2 == FUNCT6_FUNCT2_AND) {
                rdnum = state->instruction.catype.rdp + 8;
                rs2num = state->instruction.catype.rs2p + 8;
                break;
            }
            break;
        }
        case OPCODE16_BEQZ:
        case OPCODE16_BNEZ: {
            RiscvInstructionTypeCBDecoderImm_u RiscvInstructionTypeCBDecoderImm = {0};
            RiscvInstructionTypeCBDecoderImm.bit.imm2_1 = state->instruction.cbtype.imm2_1;
            RiscvInstructionTypeCBDecoderImm.bit.imm4_3 = state->instruction.cbtype.imm4_3;
            RiscvInstructionTypeCBDecoderImm.bit.imm5 = state->instruction.cbtype.imm5;
            RiscvInstructionTypeCBDecoderImm.bit.imm7_6 = state->instruction.cbtype.imm7_6;
            RiscvInstructionTypeCBDecoderImm.bit.imm8 = state->instruction.cbtype.imm8;
            imm = RiscvInstructionTypeCBDecoderImm.imm;
            rs1num = state->instruction.cbtype.rs1p + 8;
            break;
        }
        case OPCODE16_ADDI:
        case OPCODE16_LI:
        case OPCODE16_SLLI: {
            RiscvInstructionTypeCIDecoderImm_u RiscvInstructionTypeCIDecoderImm = {0};
            RiscvInstructionTypeCIDecoderImm.bit.imm4_0 = state->instruction.citype.imm4_0;
            RiscvInstructionTypeCIDecoderImm.bit.imm5 = state->instruction.citype.imm5;
            imm = RiscvInstructionTypeCIDecoderImm.imm;
            rdnum = state->instruction.citype.rd;
            break;
        }
        case OPCODE16_JAL:
        case OPCODE16_J: {
            RiscvInstructionTypeCJDecoderImm_u RiscvInstructionTypeCJDecoderImm = {0};
            RiscvInstructionTypeCJDecoderImm.bit.imm3_1 = state->instruction.cjtype.imm3_1;
            RiscvInstructionTypeCJDecoderImm.bit.imm4 = state->instruction.cjtype.imm4;
            RiscvInstructionTypeCJDecoderImm.bit.imm5 = state->instruction.cjtype.imm5;
            RiscvInstructionTypeCJDecoderImm.bit.imm6 = state->instruction.cjtype.imm6;
            RiscvInstructionTypeCJDecoderImm.bit.imm7 = state->instruction.cjtype.imm7;
            RiscvInstructionTypeCJDecoderImm.bit.imm9_8 = state->instruction.cjtype.imm9_8;
            RiscvInstructionTypeCJDecoderImm.bit.imm10 = state->instruction.cjtype.imm10;
            RiscvInstructionTypeCJDecoderImm.bit.imm11 = state->instruction.cjtype.imm11;
            imm = RiscvInstructionTypeCJDecoderImm.imm;
            break;
        }
#if (RVE_E_F == 1)
        case OPCODE16_FLW:
#endif
        case OPCODE16_LW: {
            RiscvInstructionTypeCLDecoderImm_u RiscvInstructionTypeCLDecoderImm = {0};
            RiscvInstructionTypeCLDecoderImm.bit.imm2 = state->instruction.cltype.imm2;
            RiscvInstructionTypeCLDecoderImm.bit.imm5_3 = state->instruction.cltype.imm5_3;
            RiscvInstructionTypeCLDecoderImm.bit.imm6 = state->instruction.cltype.imm6;
            imm = RiscvInstructionTypeCLDecoderImm.imm;
            rs1num = state->instruction.cltype.rs1p + 8;
            rdnum = state->instruction.cltype.rdp + 8;
            break;
        }
#if (RVE_E_F == 1)
        case OPCODE16_FSW:
#endif
        case OPCODE16_SW: {
            RiscvInstructionTypeCSDecoderImm_u RiscvInstructionTypeCSDecoderImm = {0};
            RiscvInstructionTypeCSDecoderImm.bit.imm2 = state->instruction.cstype.imm2;
            RiscvInstructionTypeCSDecoderImm.bit.imm5_3 = state->instruction.cstype.imm5_3;
            RiscvInstructionTypeCSDecoderImm.bit.imm6 = state->instruction.cstype.imm6;
            imm = RiscvInstructionTypeCSDecoderImm.imm;
            rs1num = state->instruction.cstype.rs1p + 8;
            rs2num = state->instruction.cstype.rs2p + 8;
            break;
        }
#if (RVE_E_F == 1)
        case OPCODE16_FLWSP:
#endif
        case OPCODE16_LWSP: {
            RiscvInstructionTypeCILwspDecoderImm_u RiscvInstructionTypeCILwspDecoderImm = {0};
            RiscvInstructionTypeCILwspDecoderImm.bit.imm4_2 = state->instruction.cilwsp.imm4_2;
            RiscvInstructionTypeCILwspDecoderImm.bit.imm5 = state->instruction.cilwsp.imm5;
            RiscvInstructionTypeCILwspDecoderImm.bit.imm7_6 = state->instruction.cilwsp.imm7_6;
            imm = RiscvInstructionTypeCILwspDecoderImm.imm;
            rdnum = state->instruction.cilwsp.rd;
            break;
        }
#if (RVE_E_F == 1)
        case OPCODE16_FSWSP:
#endif
        case OPCODE16_SWSP: {
            RiscvInstructionTypeCSSDecoderImm_u RiscvInstructionTypeCSSDecoderImm = {0};
            RiscvInstructionTypeCSSDecoderImm.bit.imm5_2 = state->instruction.csstype.imm5_2;
            RiscvInstructionTypeCSSDecoderImm.bit.imm7_6 = state->instruction.csstype.imm7_6;
            imm = RiscvInstructionTypeCSSDecoderImm.imm;
            rs2num = state->instruction.csstype.rs2;
            break;
        }
    }

    if (rdnum >= 0) {
        rd = &state->reg.x[rdnum];
    }
    if (rs1num >= 0) {
        rs1 = &state->reg.x[rs1num];
    }
    if (rs2num >= 0) {
        rs2 = &state->reg.x[rs2num];
    }

    switch (opfunct3) {
        case OPCODE16_ADDI4SPN:
            if (imm != 0) {
                RiscvEmulatorC_ADDI4SPN(state, rdnum, rd, sp, imm);
            } else {
                state->trapflag.illegalinstruction = 1;
            }
            break;
        case OPCODE16_LW:
            RiscvEmulatorC_LW(state, rdnum, rd, rs1num, rs1, imm);
            break;
        case OPCODE16_SW:
            RiscvEmulatorC_SW(state, rs1num, rs1, rs2num, rs2, imm);
            break;
        case OPCODE16_ADDI:
            RiscvEmulatorC_ADDI(state, rdnum, rd, imm);
            break;
        case OPCODE16_JAL:
            RiscvEmulatorC_JAL(state, ra, imm);
            break;
        case OPCODE16_LI:
            RiscvEmulatorC_LI(state, rdnum, rd, imm);
            break;
        case OPCODE16_LUI_ADDI16SP: {
            rdnum = state->instruction.cilui.rd;
            rd = &state->reg.x[rdnum];

            if (rdnum == 2) {
                RiscvEmulatorC_ADDI16SP(state, rdnum, rd);
            } else {
                RiscvEmulatorC_LUI(state, rdnum, rd);
            }
            break;
        }
        case OPCODE16_MISCALU:
            if (funct3_funct2 == FUNCT3_FUNCT2_SRLI) {
                RiscvEmulatorC_SRLI(state, rdnum, rd, imm);
                break;
            }

            if (funct3_funct2 == FUNCT3_FUNCT2_SRAI) {
                RiscvEmulatorC_SRAI(state, rdnum, rd, imm);
                break;
            }

            if (funct3_funct2 == FUNCT3_FUNCT2_ANDI) {
                RiscvEmulatorC_ANDI(state, rdnum, rd, imm);
                break;
            }

            if (funct6_funct2 == FUNCT6_FUNCT2_SUB) {
                RiscvEmulatorC_SUB(state, rdnum, rd, rs2num, rs2);
                break;
            }

            if (funct6_funct2 == FUNCT6_FUNCT2_XOR) {
                RiscvEmulatorC_XOR(state, rdnum, rd, rs2num, rs2);
                break;
            }

            if (funct6_funct2 == FUNCT6_FUNCT2_OR) {
                RiscvEmulatorC_OR(state, rdnum, rd, rs2num, rs2);
                break;
            }

            if (funct6_funct2 == FUNCT6_FUNCT2_AND) {
                RiscvEmulatorC_AND(state, rdnum, rd, rs2num, rs2);
                break;
            }

            state->trapflag.illegalinstruction = 1;
            break;
        case OPCODE16_J:
            RiscvEmulatorC_J(state, imm);
            break;
        case OPCODE16_BEQZ:
            RiscvEmulatorC_BEQZ(state, rs1num, rs1, imm);
            break;
        case OPCODE16_BNEZ:
            RiscvEmulatorC_BNEZ(state, rs1num, rs1, imm);
            break;
        case OPCODE16_SLLI:
            RiscvEmulatorC_SLLI(state, rdnum, rd, imm);
            break;
        case OPCODE16_LWSP:
            RiscvEmulatorC_LWSP(state, rdnum, rd, sp, imm);
            break;
        case OPCODE16_JALR_MV_ADD:
            rdnum = state->instruction.crtype.rd;
            rd = &state->reg.x[rdnum];
            rs2num = state->instruction.crtype.rs2;
            rs2 = &state->reg.x[rs2num];

            if (state->instruction.crtype.funct4 == FUNCT4_MV) {
                if (rs2num == 0) {
                    RiscvEmulatorC_JR(state, rdnum, rd);
                } else {
                    RiscvEmulatorC_MV(state, rdnum, rd, rs2num, rs2);
                }
            } else /* FUNCT4_ADD */
            {
                if (rdnum == 0 &&
                    rs2num == 0) {
                    RiscvEmulatorC_EBREAK(state);
                } else if (rs2num == 0) {
                    RiscvEmulatorC_JALR(state, rdnum, rd, ra);
                } else {
                    RiscvEmulatorC_ADD(state, rdnum, rd, rs2num, rs2);
                }
            }
            break;
        case OPCODE16_SWSP:
            RiscvEmulatorC_SWSP(state, rs2num, rs2, sp, imm);
            break;
#if (RVE_E_F == 1)
        case OPCODE16_FLW:
            state->trapflag.illegalinstruction = 1;
            break;
        case OPCODE16_FLWSP:
            state->trapflag.illegalinstruction = 1;
            break;
        case OPCODE16_FSW:
            state->trapflag.illegalinstruction = 1;
            break;
        case OPCODE16_FSWSP:
            state->trapflag.illegalinstruction = 1;
            break;
#endif
#if (RVE_E_D == 1)
        case OPCODE16_FLD:
            state->trapflag.illegalinstruction = 1;
            break;
        case OPCODE16_FLDSP:
            state->trapflag.illegalinstruction = 1;
            break;
        case OPCODE16_FSD:
            state->trapflag.illegalinstruction = 1;
            break;
        case OPCODE16_FSDSP:
            state->trapflag.illegalinstruction = 1;
            break;
#endif
        default:
            state->trapflag.illegalinstruction = 1;
            break;
    }
}

#endif

#endif