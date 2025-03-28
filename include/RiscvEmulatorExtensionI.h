/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionI_H_
#define RiscvEmulatorExtensionI_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorHook.h"
#include "RiscvEmulatorType.h"

#include "RiscvEmulatorExtensionM.h"
#include "RiscvEmulatorExtensionZba.h"
#include "RiscvEmulatorExtensionZbb.h"
#include "RiscvEmulatorExtensionZbc.h"
#include "RiscvEmulatorExtensionZbs.h"
#include "RiscvEmulatorExtensionZicsr.h"

/**
 * Jump and link register.
 */
static inline void RiscvEmulatorJALR(RiscvEmulatorState_t *state) {
    uint8_t rdnum = state->instruction.itype.rd;
    void *rd = &state->reg.x[rdnum];
    uint8_t rs1num = state->instruction.itype.rs1;
    void *rs1 = &state->reg.x[rs1num];

    int16_t imm = state->instruction.itype.imm;

    uint32_t jumptoprogramcounter = (*(uint32_t *)rs1 + imm) & (UINT32_MAX - 1);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "jalr";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = imm;
    hc.immissigned = 1;
    hc.immname = "offset";
    RiscvEmulatorHook(state, &hc);
#endif

#if (RVE_E_ZICSR == 1) && (RVE_E_C != 1)
    // Check if jumptoprogramcounter is aligned.
    if ((jumptoprogramcounter & 0b11) != 0) {
        state->trapflag.instructionaddressmisaligned = 1;
        state->csr.mtval = jumptoprogramcounter;
        return;
    }
#endif

    // Set destination register to current next instruction acting as a return address.
    if (rdnum != 0) {
        *(uint32_t *)rd = state->programcounternext;
    }

    // Execute jump.
    state->programcounternext = jumptoprogramcounter;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Process JALR opcode.
 */
static inline void RiscvEmulatorOpcodeJumpAndLinkRegister(RiscvEmulatorState_t *state) {
    if (state->instruction.itype.funct3 == FUNCT3_JUMPANDLINKREGISTER_JALR) {
        RiscvEmulatorJALR(state);
    } else {
        state->trapflag.illegalinstruction = 1;
    }
}

/**
 * Add: rd = rs1 + rs2
 */
static inline void RiscvEmulatorADD(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "add";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 + *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Add: rd = rs1 + imm
 */
static inline void RiscvEmulatorADDI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const int16_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "addi";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = imm;
    hc.immissigned = 1;
    hc.immlength = sizeof(imm);
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 + imm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Subtract: rd = rs1 - rs2
 */
static inline void RiscvEmulatorSUB(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "sub";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 - *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Logical shift left: rd = rs1 << (rs2 & 0b11111)
 */
static inline void RiscvEmulatorSLL(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "sll";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 << (*(uint32_t *)rs2 & 0b11111);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Logical shift left: rd = rs1 << (shamt & 0b11111)
 */
static inline void RiscvEmulatorSLLI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "slli";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 << (shamt & 0b11111);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Signed compare: rd = (rs1 < rs2)
 */
static inline void RiscvEmulatorSLT(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "slt";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = (*(int32_t *)rs1 < *(int32_t *)rs2);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Signed compare: rd = (rs1 < imm)
 */
static inline void RiscvEmulatorSLTI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const int16_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "slti";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = imm;
    hc.immlength = (sizeof(imm));
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = (*(int32_t *)rs1 < imm);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Unsigned compare: rd = (rs1 < rs2)
 */
static inline void RiscvEmulatorSLTU(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "sltu";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = (*(uint32_t *)rs1 < *(uint32_t *)rs2);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Unsigned compare: rd = (rs1 < imm)
 */
static inline void RiscvEmulatorSLTIU(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint32_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "sltiu";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = imm;
    hc.immlength = (sizeof(imm));
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = (*(uint32_t *)rs1 < imm);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Exclusive or: rd = rs1 ^ rs2
 */
static inline void RiscvEmulatorXOR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "xor";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 ^ *(uint32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Exclusive or: rd = rs1 ^ imm
 */
static inline void RiscvEmulatorXORI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint32_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "xori";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = imm;
    hc.immlength = (sizeof(imm));
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 ^ imm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Logical shift right: rd = rs1 >> (rs2 & 0b11111)
 */
static inline void RiscvEmulatorSRL(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "srl";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 >> (*(uint32_t *)rs2 & 0b11111);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Logical shift right: rd = rs1 >> (shamt & 0b11111)
 */
static inline void RiscvEmulatorSRLI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "srli";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 >> (shamt & 0b11111);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Arithmetic shift right: rd = rs1 >> (rs2 & 0b11111)
 */
static inline void RiscvEmulatorSRA(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "sra";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 >> (*(uint32_t *)rs2 & 0b11111);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Arithmetic shift right: rd = rs1 >> (shamt & 0b11111)
 */
static inline void RiscvEmulatorSRAI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "srai";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 >> (shamt & 0b11111);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Boolean or: rd = rs1 | rs2
 */
static inline void RiscvEmulatorOR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "or";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 | *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Boolean or: rd = rs1 | imm
 */
static inline void RiscvEmulatorORI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const int16_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "ori";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = imm;
    hc.immlength = (sizeof(imm));
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 | imm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Boolean and: rd = rs1 & rs2
 */
static inline void RiscvEmulatorAND(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "and";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 & *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Boolean and: rd = rs1 & imm
 */
static inline void RiscvEmulatorANDI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const int16_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "andi";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.imm = imm;
    hc.immlength = (sizeof(imm));
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 & imm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Process operation opcodes.
 */
static inline void RiscvEmulatorOpcodeOperation(RiscvEmulatorState_t *state) {
    int8_t detectedUnknownInstruction = 1;

    uint8_t rdnum = state->instruction.rtype.rd;
    void *rd = &state->reg.x[rdnum];
    uint8_t rs1num = state->instruction.rtype.rs1;
    void *rs1 = &state->reg.x[rs1num];
    uint8_t rs2num = state->instruction.rtype.rs2;
    void *rs2 = &state->reg.x[rs2num];

    if (detectedUnknownInstruction == 1) {
        RiscvInstructionTypeRDecoderFunct7Funct3_u instruction_decoderhelper_rtype = {0};
        instruction_decoderhelper_rtype.funct3 = state->instruction.rtype.funct3;
        instruction_decoderhelper_rtype.funct7 = state->instruction.rtype.funct7;

        detectedUnknownInstruction = -1;
        switch (instruction_decoderhelper_rtype.funct7_3) {
            case FUNCT7_FUNCT3_OPERATION_ADD:
                RiscvEmulatorADD(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_SUB:
                RiscvEmulatorSUB(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_SLL:
                RiscvEmulatorSLL(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_SLT:
                RiscvEmulatorSLT(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_SLTU:
                RiscvEmulatorSLTU(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_XOR:
                RiscvEmulatorXOR(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_SRL:
                RiscvEmulatorSRL(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_SRA:
                RiscvEmulatorSRA(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_OR:
                RiscvEmulatorOR(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_AND:
                RiscvEmulatorAND(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
#if (RVE_E_M == 1)
            case FUNCT7_FUNCT3_OPERATION_MUL:
                RiscvEmulatorMUL(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_MULH:
                RiscvEmulatorMULH(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_MULHSU:
                RiscvEmulatorMULHSU(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_MULHU:
                RiscvEmulatorMULHU(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_DIV:
                RiscvEmulatorDIV(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_DIVU:
                RiscvEmulatorDIVU(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_REM:
                RiscvEmulatorREM(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_REMU:
                RiscvEmulatorREMU(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
#endif
#if (RVE_E_ZBA == 1)
            case FUNCT7_FUNCT3_OPERATION_SH1ADD:
            case FUNCT7_FUNCT3_OPERATION_SH2ADD:
            case FUNCT7_FUNCT3_OPERATION_SH3ADD:
                RiscvEmulatorSHADD(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
#endif
#if (RVE_E_ZBB == 1)
            case FUNCT7_FUNCT3_OPERATION_ANDN:
                RiscvEmulatorANDN(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_ORN:
                RiscvEmulatorORN(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_XNOR:
                RiscvEmulatorXNOR(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_MAX:
                RiscvEmulatorMAX(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_MAXU:
                RiscvEmulatorMAXU(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_MIN:
                RiscvEmulatorMIN(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_MINU:
                RiscvEmulatorMINU(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_ROL:
                RiscvEmulatorROL(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_ROR:
                RiscvEmulatorROR(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
#endif
#if (RVE_E_ZBC == 1)
            case FUNCT7_FUNCT3_OPERATION_CMUL:
                RiscvEmulatorCLMUL(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_CMULH:
                RiscvEmulatorCLMULH(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_CMULR:
                RiscvEmulatorCLMULR(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
#endif
#if (RVE_E_ZBS == 1)
            case FUNCT7_FUNCT3_OPERATION_BCLR:
                RiscvEmulatorBCLR(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_BEXT:
                RiscvEmulatorBEXT(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_BINV:
                RiscvEmulatorBINV(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_BSET:
                RiscvEmulatorBSET(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
#endif
            default:
                detectedUnknownInstruction = 1;
                break;
        }
    }

#if (RVE_E_ZBB == 1)
    if (detectedUnknownInstruction == 1) {
        RiscvInstructionTypeRDecoderFunct3Rs2Funct7_u instruction_decoderhelper_rtype_Funct3Rs2Funct7 = {0};
        instruction_decoderhelper_rtype_Funct3Rs2Funct7.funct3 = state->instruction.rtype.funct3;
        instruction_decoderhelper_rtype_Funct3Rs2Funct7.rs2 = state->instruction.rtype.rs2;
        instruction_decoderhelper_rtype_Funct3Rs2Funct7.funct7 = state->instruction.rtype.funct7;

        detectedUnknownInstruction = -1;
        switch (instruction_decoderhelper_rtype_Funct3Rs2Funct7.funct3_rs2_funct7) {
            case FUNCT7_RS2_FUNCT3_OPERATION_ZEXTH:
                RiscvEmulatorZEXT_H(state, rdnum, rd, rs1num, rs1);
                break;

            default:
                detectedUnknownInstruction = 1;
                break;
        }
    }
#endif

    if (detectedUnknownInstruction == 1) {
        state->trapflag.illegalinstruction = 1;
    }
}

/**
 * Process immediate opcodes.
 */
static inline void RiscvEmulatorOpcodeImmediate(RiscvEmulatorState_t *state) {
    int8_t detectedUnknownInstruction = 1;

    uint8_t rdnum = state->instruction.itype.rd;
    void *rd = &state->reg.x[rdnum];
    uint8_t rs1num = state->instruction.itype.rs1;
    void *rs1 = &state->reg.x[rs1num];

#if (RVE_E_ZBB == 1)
    if (detectedUnknownInstruction == 1) {
        // If funct3 == 0b001 or 0b101 then a whole set of functions are encoded in imm.
        if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_FUNCTIONS_1 ||
            state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_FUNCTIONS_5) {
            RiscvInstructionTypeIDecoderImmFunct3ImmFunct3_u instruction_decoderhelper_itype_functiongroup = {0};
            instruction_decoderhelper_itype_functiongroup.funct3 = state->instruction.itype.funct3;
            instruction_decoderhelper_itype_functiongroup.imm = state->instruction.itype.imm;

            detectedUnknownInstruction = -1;
            switch (instruction_decoderhelper_itype_functiongroup.immfunct3) {
                case IMM11_0_FUNCT3_IMMEDIATE_CLZ:
                    RiscvEmulatorCLZ(state, rdnum, rd, rs1num, rs1);
                    break;
                case IMM11_0_FUNCT3_IMMEDIATE_CTZ:
                    RiscvEmulatorCTZ(state, rdnum, rd, rs1num, rs1);
                    break;
                case IMM11_0_FUNCT3_IMMEDIATE_CPOP:
                    RiscvEmulatorCPOP(state, rdnum, rd, rs1num, rs1);
                    break;
                case IMM11_0_FUNCT3_IMMEDIATE_SEXTB:
                    RiscvEmulatorSEXT_B(state, rdnum, rd, rs1num, rs1);
                    break;
                case IMM11_0_FUNCT3_IMMEDIATE_SEXTH:
                    RiscvEmulatorSEXT_H(state, rdnum, rd, rs1num, rs1);
                    break;
                case IMM11_0_FUNCT3_IMMEDIATE_ORCB:
                    RiscvEmulatorORC_B(state, rdnum, rd, rs1num, rs1);
                    break;
                case IMM11_0_FUNCT3_IMMEDIATE_REV8:
                    RiscvEmulatorREV8(state, rdnum, rd, rs1num, rs1);
                    break;
                default:
                    detectedUnknownInstruction = 1;
                    break;
            }
        }
    }
#endif

    if (detectedUnknownInstruction == 1) {
        // If funct3 == 0b001 or 0b101 then a whole set of functions are encoded in parts of imm.
        if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_FUNCTIONS_1 ||
            state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_FUNCTIONS_5) {

            uint8_t shamt = state->instruction.itypeshiftbyconstant.shamt;

            RiscvInstructionTypeIDecoderImm11_7Funct3Imm11_7Funct3_u instruction_decoderhelper_itype_functions_shamt = {0};

            instruction_decoderhelper_itype_functions_shamt.funct3 = state->instruction.itype.funct3;
            instruction_decoderhelper_itype_functions_shamt.imm11_5 = state->instruction.itypeshiftbyconstant.imm11_5;

            detectedUnknownInstruction = -1;
            switch (instruction_decoderhelper_itype_functions_shamt.imm11_5funct3) {
                case IMM11_5_FUNCT3_IMMEDIATE_SLLI:
                    RiscvEmulatorSLLI(state, rdnum, rd, rs1num, rs1, shamt);
                    break;
                case IMM11_5_FUNCT3_IMMEDIATE_SRLI:
                    RiscvEmulatorSRLI(state, rdnum, rd, rs1num, rs1, shamt);
                    break;
                case IMM11_5_FUNCT3_IMMEDIATE_SRAI:
                    RiscvEmulatorSRAI(state, rdnum, rd, rs1num, rs1, shamt);
                    break;
#if (RVE_E_ZBB == 1)
                case IMM11_5_FUNCT3_IMMEDIATE_RORI:
                    RiscvEmulatorRORI(state, rdnum, rd, rs1num, rs1, shamt);
                    break;
#endif
#if (RVE_E_ZBS == 1)
                case IMM11_5_FUNCT3_IMMEDIATE_BCLRI:
                    RiscvEmulatorBCLRI(state, rdnum, rd, rs1num, rs1, shamt);
                    break;
                case IMM11_5_FUNCT3_IMMEDIATE_BEXTI:
                    RiscvEmulatorBEXTI(state, rdnum, rd, rs1num, rs1, shamt);
                    break;
                case IMM11_5_FUNCT3_IMMEDIATE_BINVI:
                    RiscvEmulatorBINVI(state, rdnum, rd, rs1num, rs1, shamt);
                    break;
                case IMM11_5_FUNCT3_IMMEDIATE_BSETI:
                    RiscvEmulatorBSETI(state, rdnum, rd, rs1num, rs1, shamt);
                    break;
#endif
                default:
                    detectedUnknownInstruction = 1;
                    break;
            }
        }
    }

    if (detectedUnknownInstruction == 1) {
        detectedUnknownInstruction = -1;
        int16_t imm = state->instruction.itype.imm;
        switch (state->instruction.itype.funct3) {
            case FUNCT3_IMMEDIATE_ADDI:
                RiscvEmulatorADDI(state, rdnum, rd, rs1num, rs1, imm);
                break;
            case FUNCT3_IMMEDIATE_SLTI:
                RiscvEmulatorSLTI(state, rdnum, rd, rs1num, rs1, imm);
                break;
            case FUNCT3_IMMEDIATE_SLTIU:
                RiscvEmulatorSLTIU(state, rdnum, rd, rs1num, rs1, imm);
                break;
            case FUNCT3_IMMEDIATE_XORI:
                RiscvEmulatorXORI(state, rdnum, rd, rs1num, rs1, imm);
                break;
            case FUNCT3_IMMEDIATE_ORI:
                RiscvEmulatorORI(state, rdnum, rd, rs1num, rs1, imm);
                break;
            case FUNCT3_IMMEDIATE_ANDI:
                RiscvEmulatorANDI(state, rdnum, rd, rs1num, rs1, imm);
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

/**
 * Process load opcodes.
 */
static inline void RiscvEmulatorOpcodeLoad(RiscvEmulatorState_t *state) {
    uint8_t rdnum = state->instruction.itype.rd;
    void *rd = &state->reg.x[rdnum];
    uint8_t rs1num = state->instruction.stype.rs1;
    void *rs1 = &state->reg.x[rs1num];

    int16_t imm = state->instruction.itype.imm;
    uint32_t memorylocation = imm + *(uint32_t *)rs1;

#if (RVE_E_HOOK == 1)
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "unknown";
#endif

    uint8_t length = 0;
    switch (state->instruction.itype.funct3) {
        case FUNCT3_LOAD_LB:
#if (RVE_E_HOOK == 1)
            hc.instruction = "lb";
#endif
            length = sizeof(uint8_t);
            break;
        case FUNCT3_LOAD_LBU:
#if (RVE_E_HOOK == 1)
            hc.instruction = "lbu";
#endif
            length = sizeof(uint8_t);
            break;
        case FUNCT3_LOAD_LH:
#if (RVE_E_HOOK == 1)
            hc.instruction = "lh";
#endif
            length = sizeof(uint16_t);
            break;
        case FUNCT3_LOAD_LHU:
#if (RVE_E_HOOK == 1)
            hc.instruction = "lhu";
#endif
            length = sizeof(uint16_t);
            break;
        case FUNCT3_LOAD_LW:
#if (RVE_E_HOOK == 1)
            hc.instruction = "lw";
#endif
            length = sizeof(uint32_t);
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }

#if (RVE_E_ZICSR == 1)
    // Check if the load is aligned.
    if (length > 1) {
        // Only the last few bits need to be checked.
        uint8_t memorylocation8 = memorylocation & 0xFF;
        if ((memorylocation8 % length) != 0) {
            state->trapflag.loadaddressmisaligned = 1;
            state->csr.mtval = memorylocation;
        }
    }
#endif

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
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

    if (rdnum == 0) {
        return;
    }

#if (RVE_E_ZICSR == 1)
    if (state->trapflag.loadaddressmisaligned == 1) {
        return;
    }
#endif

    uint32_t value = 0;
    RiscvEmulatorLoad(memorylocation, &value, length);

    switch (state->instruction.itype.funct3) {
        case FUNCT3_LOAD_LB:
            *(int32_t *)rd = (int8_t)value;
            break;
        case FUNCT3_LOAD_LBU:
            *(uint32_t *)rd = (uint8_t)value;
            break;
        case FUNCT3_LOAD_LH:
            *(int32_t *)rd = (int16_t)value;
            break;
        case FUNCT3_LOAD_LHU:
            *(uint32_t *)rd = (uint16_t)value;
            break;
        case FUNCT3_LOAD_LW:
            *(uint32_t *)rd = (uint32_t)value;
            break;
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Process store opcodes.
 */
static inline void RiscvEmulatorOpcodeStore(RiscvEmulatorState_t *state) {
    // Untangle the immediate bits.
    RiscvInstructionTypeSDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm4_0 = state->instruction.stype.imm4_0;
    immdecoder.bit.imm11_5 = state->instruction.stype.imm11_5;
    int16_t offset = immdecoder.imm;

    uint8_t rs1num = state->instruction.stype.rs1;
    void *rs1 = &state->reg.x[rs1num];
    uint8_t rs2num = state->instruction.stype.rs2;
    void *rs2 = &state->reg.x[rs2num];

    uint32_t memorylocation = offset + *(uint32_t *)rs1;

#if (RVE_E_HOOK == 1)
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "unkown";
#endif

    uint8_t length = 0;
    switch (state->instruction.stype.funct3) {
        case FUNCT3_STORE_SW:
#if (RVE_E_HOOK == 1)
            hc.instruction = "sw";
#endif
            length = sizeof(uint32_t);
            break;
        case FUNCT3_STORE_SH:
#if (RVE_E_HOOK == 1)
            hc.instruction = "sh";
#endif
            length = sizeof(uint16_t);
            break;
        case FUNCT3_STORE_SB:
#if (RVE_E_HOOK == 1)
            hc.instruction = "sb";
#endif
            length = sizeof(uint8_t);
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }

#if (RVE_E_ZICSR == 1)
    // Check if the store is aligned.
    if (length > 1) {
        // Only the last few bits need to be checked.
        uint8_t memorylocation8 = memorylocation & 0xFF;
        if ((memorylocation8 % length) != 0) {
            state->trapflag.storeaddressmisaligned = 1;
            state->csr.mtval = memorylocation;
        }
    }
#endif

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
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
 * Branch if equal.
 */
static inline void RiscvEmulatorBEQ(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const int16_t imm __attribute__((unused)),
    uint8_t *executebranch,
    RiscvEmulatorHookContext_t *hc) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    hc->instruction = "beq";
    hc->hook = HOOK_BEGIN;
    hc->rs1num = rs1num;
    hc->rs1 = rs1;
    hc->rs2num = rs2num;
    hc->rs2 = rs2;
    hc->imm = imm;
    hc->immissigned = 1;
    hc->immlength = sizeof(imm);
    hc->immname = "offset";
    RiscvEmulatorHook(state, hc);
#endif

    if (*(int32_t *)rs1 == *(int32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Branch if not equal.
 */
static inline void RiscvEmulatorBNE(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const int16_t imm __attribute__((unused)),
    uint8_t *executebranch,
    RiscvEmulatorHookContext_t *hc) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    hc->instruction = "bne";
    hc->hook = HOOK_BEGIN;
    hc->rs1num = rs1num;
    hc->rs1 = rs1;
    hc->rs2num = rs2num;
    hc->rs2 = rs2;
    hc->imm = imm;
    hc->immissigned = 1;
    hc->immlength = sizeof(imm);
    hc->immname = "offset";
    RiscvEmulatorHook(state, hc);
#endif

    if (*(int32_t *)rs1 != *(int32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Branch if greater than or equal.
 */
static inline void RiscvEmulatorBGE(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const int16_t imm __attribute__((unused)),
    uint8_t *executebranch,
    RiscvEmulatorHookContext_t *hc) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    hc->instruction = "bge";
    hc->hook = HOOK_BEGIN;
    hc->rs1num = rs1num;
    hc->rs1 = rs1;
    hc->rs2num = rs2num;
    hc->rs2 = rs2;
    hc->imm = imm;
    hc->immissigned = 1;
    hc->immlength = sizeof(imm);
    hc->immname = "offset";
    RiscvEmulatorHook(state, hc);
#endif

    if (*(int32_t *)rs1 >= *(int32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Branch if greater than or equal unsigned.
 */
static inline void RiscvEmulatorBGEU(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const int16_t imm __attribute__((unused)),
    uint8_t *executebranch,
    RiscvEmulatorHookContext_t *hc) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    hc->instruction = "bgeu";
    hc->hook = HOOK_BEGIN;
    hc->rs1num = rs1num;
    hc->rs1 = rs1;
    hc->rs2num = rs2num;
    hc->rs2 = rs2;
    hc->imm = imm;
    hc->immissigned = 1;
    hc->immlength = sizeof(imm);
    hc->immname = "offset";
    RiscvEmulatorHook(state, hc);
#endif

    if (*(uint32_t *)rs1 >= *(uint32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Branch if less than.
 */
static inline void RiscvEmulatorBLT(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const int16_t imm __attribute__((unused)),
    uint8_t *executebranch,
    RiscvEmulatorHookContext_t *hc) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    hc->instruction = "blt";
    hc->hook = HOOK_BEGIN;
    hc->rs1num = rs1num;
    hc->rs1 = rs1;
    hc->rs2num = rs2num;
    hc->rs2 = rs2;
    hc->imm = imm;
    hc->immissigned = 1;
    hc->immlength = sizeof(imm);
    hc->immname = "offset";
    RiscvEmulatorHook(state, hc);
#endif

    if (*(int32_t *)rs1 < *(int32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Branch if less than unsigned.
 */
static inline void RiscvEmulatorBLTU(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const int16_t imm __attribute__((unused)),
    uint8_t *executebranch,
    RiscvEmulatorHookContext_t *hc) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    hc->instruction = "bltu";
    hc->hook = HOOK_BEGIN;
    hc->rs1num = rs1num;
    hc->rs1 = rs1;
    hc->rs2num = rs2num;
    hc->rs2 = rs2;
    hc->imm = imm;
    hc->immissigned = 1;
    hc->immlength = sizeof(imm);
    hc->immname = "offset";
    RiscvEmulatorHook(state, hc);
#endif

    if (*(uint32_t *)rs1 < *(uint32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Process branch opcodes.
 */
static inline void RiscvEmulatorOpcodeBranch(RiscvEmulatorState_t *state) {
    uint8_t executebranch = BRANCH_NO;
    uint8_t rs1num = state->instruction.btype.rs1;
    void *rs1 = &state->reg.x[rs1num];
    uint8_t rs2num = state->instruction.btype.rs2;
    void *rs2 = &state->reg.x[rs2num];

    // Untangle the immediate bits.
    RiscvInstructionTypeBDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm4_1 = state->instruction.btype.imm4_1;
    immdecoder.bit.imm10_5 = state->instruction.btype.imm10_5;
    immdecoder.bit.imm11 = state->instruction.btype.imm11;
    immdecoder.bit.imm12 = state->instruction.btype.imm12;
    int16_t imm = immdecoder.imm;

#if (RVE_E_HOOK == 1)
    RiscvEmulatorHookContext_t hc = {0};
#else
    RiscvEmulatorHookContext_t hc;
#endif

    switch (state->instruction.btype.funct3) {
        case FUNCT3_BRANCH_BEQ:
            RiscvEmulatorBEQ(state, rs1num, rs1, rs2num, rs2, imm, &executebranch, &hc);
            break;
        case FUNCT3_BRANCH_BNE:
            RiscvEmulatorBNE(state, rs1num, rs1, rs2num, rs2, imm, &executebranch, &hc);
            break;
        case FUNCT3_BRANCH_BGE:
            RiscvEmulatorBGE(state, rs1num, rs1, rs2num, rs2, imm, &executebranch, &hc);
            break;
        case FUNCT3_BRANCH_BGEU:
            RiscvEmulatorBGEU(state, rs1num, rs1, rs2num, rs2, imm, &executebranch, &hc);
            break;
        case FUNCT3_BRANCH_BLT:
            RiscvEmulatorBLT(state, rs1num, rs1, rs2num, rs2, imm, &executebranch, &hc);
            break;
        case FUNCT3_BRANCH_BLTU:
            RiscvEmulatorBLTU(state, rs1num, rs1, rs2num, rs2, imm, &executebranch, &hc);
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }

    if (executebranch == BRANCH_YES) {
        state->programcounternext = state->programcounter + imm;

#if (RVE_E_ZICSR == 1) && (RVE_E_C != 1)
        // Check if programcounternext is aligned.
        if ((state->programcounternext & 0b11) != 0) {
            state->trapflag.instructionaddressmisaligned = 1;
            state->csr.mtval = state->programcounternext;
        }
#endif

#if (RVE_E_HOOK == 1)
        hc.hook = HOOK_END;
        RiscvEmulatorHook(state, &hc);
#endif
    }
}

/**
 * Add upper immediate to program counter.
 */
static inline void RiscvEmulatorAUIPC(RiscvEmulatorState_t *state) {
    uint32_t upperimmediate = state->instruction.utype.imm31_12;

    RiscvInstructionTypeUDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm31_12 = upperimmediate;
    int32_t imm = immdecoder.imm;

    uint8_t rdnum = state->instruction.utype.rd;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    void *rd = &state->reg.x[rdnum];
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "auipc";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.upperimmediate = upperimmediate;
    hc.imm = imm;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum != 0) {
        state->reg.x[rdnum] = state->programcounter + imm;
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Load upper with immediate.
 */
static inline void RiscvEmulatorLUI(RiscvEmulatorState_t *state) {
    RiscvInstructionTypeUDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm11_0 = 0;
    immdecoder.bit.imm31_12 = state->instruction.utype.imm31_12;

    uint8_t rdnum = state->instruction.utype.rd;
    void *rd = &state->reg.x[rdnum];

    uint32_t imm = immdecoder.imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "lui";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = imm;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum != 0) {
        *(uint32_t *)rd = imm;
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Jump and link.
 */
static inline void RiscvEmulatorJAL(RiscvEmulatorState_t *state) {
    uint8_t rdnum = state->instruction.jtype.rd;
    void *rd = &state->reg.x[rdnum];

    // Untangle the immediate bits.
    RiscvInstructionTypeJDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm10_1 = state->instruction.jtype.imm10_1;
    immdecoder.bit.imm11 = state->instruction.jtype.imm11;
    immdecoder.bit.imm19_12 = state->instruction.jtype.imm19_12;
    immdecoder.bit.imm20 = state->instruction.jtype.imm20;

    uint32_t jumptoprogramcounter = state->programcounter + immdecoder.imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "jal";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = immdecoder.imm;
    hc.immissigned = 1;
    hc.immname = "offset";
    RiscvEmulatorHook(state, &hc);
#endif
#if (RVE_E_ZICSR == 1) && (RVE_E_C != 1)
    // Check if jumptoprogramcounter is aligned.
    if ((jumptoprogramcounter & 0b11) != 0) {
        state->trapflag.instructionaddressmisaligned = 1;
        state->csr.mtval = jumptoprogramcounter;
        return;
    }
#endif

    // Set destination register to current next instruction acting as a return address.
    if (rdnum != 0) {
        *(uint32_t *)rd = state->programcounternext;
    }

    // Execute jump.
    state->programcounternext = jumptoprogramcounter;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Make a service request to the execution environment.
 */
static inline void RiscvEmulatorECALL(RiscvEmulatorState_t *state) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "ecall";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#endif

#if (RVE_E_ZICSR == 1)
    state->trapflag.environmentcallfrommmode = 1;
#endif

    RiscvEmulatorHandleECALL(state);
}

/**
 * Cause control to be transferred back to a debugging environment.
 */
static inline void RiscvEmulatorEBREAK(RiscvEmulatorState_t *state) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "ebreak";
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
 * Process system opcodes.
 */
static inline void RiscvEmulatorOpcodeSystem(RiscvEmulatorState_t *state) {
    int8_t detectedUnknownInstruction = 1;

    if (detectedUnknownInstruction == 1) {
        if (state->instruction.itypesystem.rd == 0 &&
            state->instruction.itypesystem.funct3 == 0 &&
            state->instruction.itypesystem.rs1 == 0) {
            detectedUnknownInstruction = -1;
            switch (state->instruction.itypesystem.funct12) {
#if (RVE_E_ZICSR == 1)
                case FUNCT12_MRET:
                    RiscvEmulatorMRET(state);
                    break;
#endif
                case FUNCT12_ECALL:
                    RiscvEmulatorECALL(state);
                    break;
                case FUNCT12_EBREAK:
                    RiscvEmulatorEBREAK(state);
                    break;
                default:
                    detectedUnknownInstruction = 1;
                    break;
            }
        }
    }

#if (RVE_E_ZICSR == 1)
    if (detectedUnknownInstruction == 1) {
        detectedUnknownInstruction = -1;

        uint8_t rdnum = state->instruction.itypecsr.rd;
        void *rd = &state->reg.x[rdnum];

        uint8_t rs1num = state->instruction.itypecsr.rs1;
        void *rs1 = &state->reg.x[rs1num];

        uint8_t imm = state->instruction.itypecsrimm.imm;

        uint16_t csrnum = state->instruction.itypecsr.csr;
        void *csr = RiscvEmulatorGetCSRAddress(state, csrnum);

        if (state->trapflag.value > 0) {
            return;
        }

        switch (state->instruction.itypecsr.funct3) {
            case FUNCT3_CSR_CSRRW:
                RiscvEmulatorCSRRW(state, rdnum, rd, rs1num, rs1, csrnum, csr);
                break;
            case FUNCT3_CSR_CSRRWI:
                RiscvEmulatorCSRRWI(state, rdnum, rd, imm, csrnum, csr);
                break;
            case FUNCT3_CSR_CSRRS:
                RiscvEmulatorCSRRS(state, rdnum, rd, rs1num, rs1, csrnum, csr);
                break;
            case FUNCT3_CSR_CSRRSI:
                RiscvEmulatorCSRRSI(state, rdnum, rd, imm, csrnum, csr);
                break;
            case FUNCT3_CSR_CSRRC:
                RiscvEmulatorCSRRC(state, rdnum, rd, rs1num, rs1, csrnum, csr);
                break;
            case FUNCT3_CSR_CSRRCI:
                RiscvEmulatorCSRRCI(state, rdnum, rd, imm, csrnum, csr);
                break;
            default:
                detectedUnknownInstruction = 1;
                break;
        }
    }

#endif

    if (detectedUnknownInstruction == 1) {
        state->trapflag.illegalinstruction = 1;
    }
}

/**
 * Excutes the fence instuction.
 *
 * This does nothing in this emulator because all memory access is always completely processed.
 */
static inline void RiscvEmulatorFence(
    RiscvEmulatorState_t *state __attribute__((unused))) {
#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "fence";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#endif
}

#if (RVE_E_ZIFENCEI == 1)
/**
 * Excutes the fencei instuction.
 *
 * This does nothing in this emulator because all memory access is always completely processed.
 */
static inline void RiscvEmulatorFencei(
    RiscvEmulatorState_t *state __attribute__((unused))) {
#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "fencei";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#endif
}
#endif

/**
 * Process miscellaneous memory opcodes.
 */
static inline void RiscvEmulatorOpcodeMiscMem(RiscvEmulatorState_t *state) {
    uint8_t detectedUnknownInstruction = 1;

    if (detectedUnknownInstruction) {
        if (state->instruction.itypemiscmem.rd == 0 &&
            state->instruction.itypemiscmem.funct3 == FUNCT3_FENCE &&
            state->instruction.itypemiscmem.rs1 == 0) {
            detectedUnknownInstruction = -1;
            RiscvEmulatorFence(state);
        }
    }

#if (RVE_E_ZIFENCEI == 1)
    if (detectedUnknownInstruction) {
        if (state->instruction.itypemiscmem.rd == 0 &&
            state->instruction.itypemiscmem.funct3 == FUNCT3_FENCEI &&
            state->instruction.itypemiscmem.rs1 == 0) {
            detectedUnknownInstruction = -1;
            RiscvEmulatorFencei(state);
        }
    }
#endif

    if (detectedUnknownInstruction == 1) {
        state->trapflag.illegalinstruction = 1;
    }
}

#endif