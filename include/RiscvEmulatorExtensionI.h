/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

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
    uint32_t originalprogramcounternext = state->programcounternext;

    uint8_t rdnum = state->instruction.itype.rd;
    void *rd = &state->registers.array.location[rdnum];
    uint8_t rs1num = state->instruction.itype.rs1;
    void *rs1 = &state->registers.array.location[rs1num];

    int16_t imm = state->instruction.itype.imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorJALRHookBegin(state, rdnum, rd, rs1num, rs1, imm);
#endif

    // Execute jump.
    state->programcounternext = *(uint32_t *)rs1 + imm;
    state->programcounternext = state->programcounternext & (UINT32_MAX - 1);

    // Set destination register to the original next instruction.
    if (rdnum != 0) {
        *(uint32_t *)rd = originalprogramcounternext;
    }

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorJALRHookEnd(state, rdnum, rd, rs1num, rs1, imm);
#endif
}

/**
 * Process JALR opcode.
 */
static inline void RiscvEmulatorOpcodeJumpAndLinkRegister(RiscvEmulatorState_t *state) {
    if (state->instruction.itype.funct3 == FUNCT3_JUMPANDLINKREGISTER_JALR) {
        RiscvEmulatorJALR(state);
    } else {
        state->trapflags.bits.illegalinstruction = 1;
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
    RiscvEmulatorIntRegRegHookBegin("add", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 + *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegRegHookEnd("add", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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
    RiscvEmulatorIntRegImmHookBegin("addi", state, rdnum, rd, rs1num, rs1, imm);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 + imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegImmHookEnd("addi", state, rdnum, rd, rs1num, rs1, imm);
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
    RiscvEmulatorIntRegRegHookBegin("sub", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 - *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegRegHookEnd("sub", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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
    RiscvEmulatorIntRegRegHookBegin("sll", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 << (*(uint32_t *)rs2 & 0b11111);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegRegHookEnd("sll", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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
    RiscvEmulatorIntRegImmHookBegin("slli", state, rdnum, rd, rs1num, rs1, shamt);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 << (shamt & 0b11111);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegImmHookEnd("slli", state, rdnum, rd, rs1num, rs1, shamt);
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
    RiscvEmulatorIntRegRegHookBegin("slt", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = (*(int32_t *)rs1 < *(int32_t *)rs2);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegRegHookEnd("slt", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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
    RiscvEmulatorIntRegImmHookBegin("slti", state, rdnum, rd, rs1num, rs1, imm);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = (*(int32_t *)rs1 < imm);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegImmHookEnd("slti", state, rdnum, rd, rs1num, rs1, imm);
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
    RiscvEmulatorIntRegRegHookBegin("sltu", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = (*(uint32_t *)rs1 < *(uint32_t *)rs2);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegRegHookEnd("sltu", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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
    RiscvEmulatorIntRegImmHookBegin("sltiu", state, rdnum, rd, rs1num, rs1, imm);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = (*(uint32_t *)rs1 < imm);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegImmHookEnd("sltiu", state, rdnum, rd, rs1num, rs1, imm);
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
    RiscvEmulatorIntRegRegHookBegin("xor", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 ^ *(uint32_t *)rs2;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegRegHookEnd("xor", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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
    RiscvEmulatorIntRegImmHookBegin("xori", state, rdnum, rd, rs1num, rs1, imm);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 ^ imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegImmHookEnd("xori", state, rdnum, rd, rs1num, rs1, imm);
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
    RiscvEmulatorIntRegRegHookBegin("srl", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 >> (*(uint32_t *)rs2 & 0b11111);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegRegHookEnd("srl", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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
    RiscvEmulatorIntRegImmHookBegin("srli", state, rdnum, rd, rs1num, rs1, shamt);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint32_t *)rs1 >> (shamt & 0b11111);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegImmHookEnd("srli", state, rdnum, rd, rs1num, rs1, shamt);
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
    RiscvEmulatorIntRegRegHookBegin("sra", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 >> (*(uint32_t *)rs2 & 0b11111);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegRegHookEnd("sra", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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
    RiscvEmulatorIntRegImmHookBegin("srai", state, rdnum, rd, rs1num, rs1, shamt);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 >> (shamt & 0b11111);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegImmHookEnd("srai", state, rdnum, rd, rs1num, rs1, shamt);
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
    RiscvEmulatorIntRegRegHookBegin("or", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 | *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegRegHookEnd("or", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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

    if (rdnum == 0) {
        return;
    }

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegImmHookBegin("ori", state, rdnum, rd, rs1num, rs1, imm);
#endif

    *(int32_t *)rd = *(int32_t *)rs1 | imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegImmHookEnd("ori", state, rdnum, rd, rs1num, rs1, imm);
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
    RiscvEmulatorIntRegRegHookBegin("and", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 & *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegRegHookEnd("and", state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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
    RiscvEmulatorIntRegImmHookBegin("andi", state, rdnum, rd, rs1num, rs1, imm);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rs1 & imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorIntRegImmHookEnd("andi", state, rdnum, rd, rs1num, rs1, imm);
#endif
}

/**
 * Process operation opcodes.
 */
static inline void RiscvEmulatorOpcodeOperation(RiscvEmulatorState_t *state) {
    int8_t detectedUnknownInstruction = 1;

    uint8_t rdnum = state->instruction.rtype.rd;
    void *rd = &state->registers.array.location[rdnum];
    uint8_t rs1num = state->instruction.rtype.rs1;
    void *rs1 = &state->registers.array.location[rs1num];
    uint8_t rs2num = state->instruction.rtype.rs2;
    void *rs2 = &state->registers.array.location[rs2num];

    if (detectedUnknownInstruction == 1) {
        RiscvInstructionTypeRDecoderFunct7Funct3_u instruction_decoderhelper_rtype;
        instruction_decoderhelper_rtype.input.funct3 = state->instruction.rtype.funct3;
        instruction_decoderhelper_rtype.input.funct7 = state->instruction.rtype.funct7;

        detectedUnknownInstruction = -1;
        switch (instruction_decoderhelper_rtype.output.funct7_3) {
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
                RiscvEmulatorCMUL(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_CMULH:
                RiscvEmulatorCMULH(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_CMULR:
                RiscvEmulatorCMULR(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
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
        RiscvInstructionTypeRDecoderFunct3Rs2Funct7_u instruction_decoderhelper_rtype_Funct3Rs2Funct7;
        instruction_decoderhelper_rtype_Funct3Rs2Funct7.input.funct3 = state->instruction.rtype.funct3;
        instruction_decoderhelper_rtype_Funct3Rs2Funct7.input.rs2 = state->instruction.rtype.rs2;
        instruction_decoderhelper_rtype_Funct3Rs2Funct7.input.funct7 = state->instruction.rtype.funct7;

        detectedUnknownInstruction = -1;
        switch (instruction_decoderhelper_rtype_Funct3Rs2Funct7.output.funct3_rs2_funct7) {
            case FUNCT7_RS2_FUNCT3_OPERATION_ZEXTH:
                RiscvEmulatorZEXTH(state, rdnum, rd, rs1num, rs1);
                break;

            default:
                detectedUnknownInstruction = 1;
                break;
        }
    }
#endif

    if (detectedUnknownInstruction == 1) {
        state->trapflags.bits.illegalinstruction = 1;
    }
}

/**
 * Process immediate opcodes.
 */
static inline void RiscvEmulatorOpcodeImmediate(RiscvEmulatorState_t *state) {
    int8_t detectedUnknownInstruction = 1;

    uint8_t rdnum = state->instruction.itype.rd;
    void *rd = &state->registers.array.location[rdnum];
    uint8_t rs1num = state->instruction.itype.rs1;
    void *rs1 = &state->registers.array.location[rs1num];

#if (RVE_E_ZBB == 1)
    if (detectedUnknownInstruction == 1) {
        // If funct3 == 0b001 or 0b101 then a whole set of functions are encoded in imm.
        if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_FUNCTIONS_1 ||
            state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_FUNCTIONS_5) {
            RiscvInstructionTypeIDecoderImmFunct3ImmFunct3_u instruction_decoderhelper_itype_functiongroup;
            instruction_decoderhelper_itype_functiongroup.input.funct3 = state->instruction.itype.funct3;
            instruction_decoderhelper_itype_functiongroup.input.imm = state->instruction.itype.imm;

            detectedUnknownInstruction = -1;
            switch (instruction_decoderhelper_itype_functiongroup.output.immfunct3) {
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
                    RiscvEmulatorSEXTB(state, rdnum, rd, rs1num, rs1);
                    break;
                case IMM11_0_FUNCT3_IMMEDIATE_SEXTH:
                    RiscvEmulatorSEXTH(state, rdnum, rd, rs1num, rs1);
                    break;
                case IMM11_0_FUNCT3_IMMEDIATE_ORCB:
                    RiscvEmulatorORCB(state, rdnum, rd, rs1num, rs1);
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

            RiscvInstructionTypeIDecoderImm11_7Funct3Imm11_7Funct3_u instruction_decoderhelper_itype_functions_shamt;

            instruction_decoderhelper_itype_functions_shamt.input.funct3 = state->instruction.itype.funct3;
            instruction_decoderhelper_itype_functions_shamt.input.imm11_5 = state->instruction.itypeshiftbyconstant.imm11_5;

            detectedUnknownInstruction = -1;
            switch (instruction_decoderhelper_itype_functions_shamt.output.imm11_5funct3) {
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
        state->trapflags.bits.illegalinstruction = 1;
    }
}

/**
 * Process load opcodes.
 */
static inline void RiscvEmulatorOpcodeLoad(RiscvEmulatorState_t *state) {
    uint8_t rdnum = state->instruction.itype.rd;
    void *rd = &state->registers.array.location[rdnum];
    uint8_t rs1num = state->instruction.stype.rs1;
    void *rs1 = &state->registers.array.location[rs1num];

    int16_t imm = state->instruction.itype.imm;
    uint32_t memorylocation = imm + *(uint32_t *)rs1;

#if (RVE_E_HOOK == 1)
    char *instruction = "unknown";
#endif

    uint8_t length = 0;
    switch (state->instruction.itype.funct3) {
        case FUNCT3_LOAD_LB:
#if (RVE_E_HOOK == 1)
            instruction = "lb";
#endif
            length = sizeof(uint8_t);
            break;
        case FUNCT3_LOAD_LBU:
#if (RVE_E_HOOK == 1)
            instruction = "lbu";
#endif
            length = sizeof(uint8_t);
            break;
        case FUNCT3_LOAD_LH:
#if (RVE_E_HOOK == 1)
            instruction = "lh";
#endif
            length = sizeof(uint16_t);
            break;
        case FUNCT3_LOAD_LHU:
#if (RVE_E_HOOK == 1)
            instruction = "lhu";
#endif
            length = sizeof(uint16_t);
            break;
        case FUNCT3_LOAD_LW:
#if (RVE_E_HOOK == 1)
            instruction = "lw";
#endif
            length = sizeof(uint32_t);
            break;
        default:
            state->trapflags.bits.illegalinstruction = 1;
            return;
    }

    if (rdnum == 0) {
        return;
    }

#if (RVE_E_ZICSR == 1)
    // Check if the load is aligned.
    if (length > 1) {
        // Only the last few bits need to be checked.
        uint8_t memorylocation8 = memorylocation & 0xFF;
        if ((memorylocation8 % length) != 0) {
            state->trapflags.bits.loadaddressmisaligned = 1;
            state->csr.mtval = memorylocation;
        }
    }
#endif

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorLoadHookBegin(instruction, state, rdnum, rd, rs1num, rs1, imm, memorylocation, length);
#endif

#if (RVE_E_ZICSR == 1)
    if (state->trapflags.bits.loadaddressmisaligned == 1) {
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
    state->hookexists = 1;
    RiscvEmulatorLoadHookEnd(instruction, state, rdnum, rd, rs1num, rs1, imm, memorylocation, length);
#endif
}

/**
 * Process store opcodes.
 */
static inline void RiscvEmulatorOpcodeStore(RiscvEmulatorState_t *state) {
    // Untangle the immediate bits.
    RiscvInstructionTypeSDecoderImm_u helper;
    helper.input.imm4_0 = state->instruction.stype.imm4_0;
    helper.input.imm11_5 = state->instruction.stype.imm11_5;
    int16_t imm = helper.output.imm;

    uint8_t rs1num = state->instruction.stype.rs1;
    void *rs1 = &state->registers.array.location[rs1num];
    uint8_t rs2num = state->instruction.stype.rs2;
    void *rs2 = &state->registers.array.location[rs2num];

    uint32_t memorylocation = imm + *(uint32_t *)rs1;

#if (RVE_E_HOOK == 1)
    char *instruction = "unkown";
#endif

    uint8_t length = 0;
    switch (state->instruction.stype.funct3) {
        case FUNCT3_STORE_SW:
#if (RVE_E_HOOK == 1)
            instruction = "sw";
#endif
            length = sizeof(uint32_t);
            break;
        case FUNCT3_STORE_SH:
#if (RVE_E_HOOK == 1)
            instruction = "sh";
#endif
            length = sizeof(uint16_t);
            break;
        case FUNCT3_STORE_SB:
#if (RVE_E_HOOK == 1)
            instruction = "sb";
#endif
            length = sizeof(uint8_t);
            break;
        default:
            state->trapflags.bits.illegalinstruction = 1;
            return;
    }

#if (RVE_E_ZICSR == 1)
    // Check if the store is aligned.
    if (length > 1) {
        // Only the last few bits need to be checked.
        uint8_t memorylocation8 = memorylocation & 0xFF;
        if ((memorylocation8 % length) != 0) {
            state->trapflags.bits.storeaddressmisaligned = 1;
            state->csr.mtval = memorylocation;
        }
    }
#endif

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorStoreHookBegin(instruction, state, rs1num, rs1, rs2num, rs2, imm, memorylocation, length);
#endif

#if (RVE_E_ZICSR == 1)
    if (state->trapflags.bits.storeaddressmisaligned == 1) {
        return;
    }
#endif

    RiscvEmulatorStore(memorylocation, rs2, length);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorStoreHookEnd(instruction, state, rs1num, rs1, rs2num, rs2, imm, memorylocation, length);
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
    const int16_t imm,
    uint8_t *executebranch) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorBranchHookBegin("beq", state, rs1num, rs1, rs2num, rs2, imm);
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
    const int16_t imm,
    uint8_t *executebranch) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorBranchHookBegin("bne", state, rs1num, rs1, rs2num, rs2, imm);
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
    const int16_t imm,
    uint8_t *executebranch) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorBranchHookBegin("bge", state, rs1num, rs1, rs2num, rs2, imm);
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
    const int16_t imm,
    uint8_t *executebranch) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorBranchHookBegin("bgeu", state, rs1num, rs1, rs2num, rs2, imm);
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
    const int16_t imm,
    uint8_t *executebranch) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorBranchHookBegin("blt", state, rs1num, rs1, rs2num, rs2, imm);
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
    const int16_t imm,
    uint8_t *executebranch) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorBranchHookBegin("bltu", state, rs1num, rs1, rs2num, rs2, imm);
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
    void *rs1 = &state->registers.array.location[rs1num];
    uint8_t rs2num = state->instruction.btype.rs2;
    void *rs2 = &state->registers.array.location[rs2num];

    // Untangle the immediate bits.
    RiscvInstructionTypeBDecoderImm_u helper;
    helper.input.imm0 = 0;
    helper.input.imm4_1 = state->instruction.btype.imm4_1;
    helper.input.imm10_5 = state->instruction.btype.imm10_5;
    helper.input.imm11 = state->instruction.btype.imm11;
    helper.input.imm12 = state->instruction.btype.imm12;
    int16_t imm = helper.output.imm;

    switch (state->instruction.btype.funct3) {
        case FUNCT3_BRANCH_BEQ:
            RiscvEmulatorBEQ(state, rs1num, rs1, rs2num, rs2, imm, &executebranch);
            break;
        case FUNCT3_BRANCH_BNE:
            RiscvEmulatorBNE(state, rs1num, rs1, rs2num, rs2, imm, &executebranch);
            break;
        case FUNCT3_BRANCH_BGE:
            RiscvEmulatorBGE(state, rs1num, rs1, rs2num, rs2, imm, &executebranch);
            break;
        case FUNCT3_BRANCH_BGEU:
            RiscvEmulatorBGEU(state, rs1num, rs1, rs2num, rs2, imm, &executebranch);
            break;
        case FUNCT3_BRANCH_BLT:
            RiscvEmulatorBLT(state, rs1num, rs1, rs2num, rs2, imm, &executebranch);
            break;
        case FUNCT3_BRANCH_BLTU:
            RiscvEmulatorBLTU(state, rs1num, rs1, rs2num, rs2, imm, &executebranch);
            break;
        default:
            state->trapflags.bits.illegalinstruction = 1;
            return;
    }

    if (executebranch == BRANCH_YES) {
        state->programcounternext = state->programcounter + imm;

#if (RVE_E_HOOK == 1)
        state->hookexists = 1;
        RiscvEmulatorBranchHookEnd(state);
#endif
    }
}

/**
 * Add upper immediate to program counter.
 */
static inline void RiscvEmulatorAUIPC(RiscvEmulatorState_t *state) {
    uint32_t upperimmediate = state->instruction.utype.imm31_12;

    RiscvInstructionTypeUDecoderImm_u helper;
    helper.input.imm11_0 = 0;
    helper.input.imm31_12 = upperimmediate;

    uint8_t rdnum = state->instruction.utype.rd;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    void *rd = &state->registers.array.location[rdnum];
    RiscvEmulatorAUIPCHookBegin(state, rdnum, rd, upperimmediate, helper.output.imm);
#endif

    if (rdnum != 0) {
        state->registers.array.location[rdnum] = state->programcounter + helper.output.imm;
    }

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorAUIPCHookEnd(state, rdnum, rd, upperimmediate, helper.output.imm);
#endif
}

/**
 * Load upper with immediate.
 */
static inline void RiscvEmulatorLUI(RiscvEmulatorState_t *state) {
    RiscvInstructionTypeUDecoderImm_u helper;
    helper.input.imm11_0 = 0;
    helper.input.imm31_12 = state->instruction.utype.imm31_12;

    uint8_t rdnum = state->instruction.utype.rd;
    void *rd = &state->registers.array.location[rdnum];

    uint32_t imm = helper.output.imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorLUIHookBegin(state, rdnum, rd, state->instruction.utype.imm31_12);
#endif

    if (rdnum != 0) {
        *(uint32_t *)rd = imm;
    }

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorLUIHookEnd(state, rdnum, rd, state->instruction.utype.imm31_12);
#endif
}

/**
 * Jump and link.
 */
static inline void RiscvEmulatorJAL(RiscvEmulatorState_t *state) {
    uint8_t rdnum = state->instruction.jtype.rd;
    void *rd = &state->registers.array.location[rdnum];

    // Untangle the immediate bits.
    RiscvInstructionTypeJDecoderImm_u helper = {0};
    helper.input.imm10_1 = state->instruction.jtype.imm10_1;
    helper.input.imm11 = state->instruction.jtype.imm11;
    helper.input.imm19_12 = state->instruction.jtype.imm19_12;
    helper.input.imm20 = state->instruction.jtype.imm20;

    uint32_t jumptoprogramcounter = state->programcounter + helper.output.imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorJALHookBegin(state, rdnum, rd, helper.output.imm);
#endif

#if (RVE_E_ZICSR == 1)
    // Check if jumptoprogramcounter is aligned.
    uint8_t programcounter8 = jumptoprogramcounter & 0xFF;
    if ((programcounter8 % (IALIGN / 8)) != 0) {
        state->trapflags.bits.instructionaddressmisaligned = 1;
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
    state->hookexists = 1;
    RiscvEmulatorJALHookEnd(state, rdnum, rd, helper.output.imm);
#endif
}

/**
 * Make a service request to the execution environment.
 */
static inline void RiscvEmulatorECALL(RiscvEmulatorState_t *state) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorEcallHookBegin(state);
#endif

#if (RVE_E_ZICSR == 1)
    state->trapflags.bits.environmentcallfrommmode = 1;
#endif

    RiscvEmulatorHandleECALL(state);
}

/**
 * Cause control to be transferred back to a debugging environment.
 */
static inline void RiscvEmulatorEBREAK(RiscvEmulatorState_t *state) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorEbreakHookBegin(state);
#endif

#if (RVE_E_ZICSR == 1)
    state->trapflags.bits.breakpoint = 1;

    /**
     * https://github.com/riscv/riscv-isa-manual/releases/tag/riscv-isa-release-52a5742-2024-08-30 says:
     * EBREAK raises a breakpoint exception and performs no other operation.
     *
     * But it also says:
     * If mtval is written with a nonzero value when a breakpoint, address-misaligned, access-fault, or page-fault exception occurs on an instruction fetch, load, or store, then mtval will contain the faulting virtual address.
     *
     * See https://github.com/riscv-non-isa/riscv-arch-test/issues/200
     *
     * Doing what sail does to pass validation.
     */
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
        void *rd = &state->registers.array.location[rdnum];

        uint8_t rs1num = state->instruction.itypecsr.rs1;
        void *rs1 = &state->registers.array.location[rs1num];

        uint8_t imm = state->instruction.itypecsrimm.imm;

        uint16_t csrnum = state->instruction.itypecsr.csr;
        void *csr = RiscvEmulatorGetCSRAddress(state, csrnum);

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
        state->trapflags.bits.illegalinstruction = 1;
    }
}

/**
 * Excutes the fence instuction.
 *
 * This does nothing in this emulator because all memory access is always completely processed.
 */
static inline void RiscvEmulatorFence() {
}

#if (RVE_E_ZIFENCEI == 1)
/**
 * Excutes the fencei instuction.
 *
 * This does nothing in this emulator because all memory access is always completely processed.
 */
static inline void RiscvEmulatorFencei() {
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
            RiscvEmulatorFence();
        }
    }

#if (RVE_E_ZIFENCEI == 1)
    if (detectedUnknownInstruction) {
        if (state->instruction.itypemiscmem.rd == 0 &&
            state->instruction.itypemiscmem.funct3 == FUNCT3_FENCEI &&
            state->instruction.itypemiscmem.rs1 == 0) {
            detectedUnknownInstruction = -1;
            RiscvEmulatorFencei();
        }
    }
#endif

    if (detectedUnknownInstruction == 1) {
        state->trapflags.bits.illegalinstruction = 1;
    }
}

#endif