/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

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
 * Store word in rs2 to memory.
 */
static inline void RiscvEmulatorC_SW(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    void *rs2,
    const int32_t imm) {

    uint8_t length = sizeof(uint32_t);

    uint32_t memorylocation = *(int32_t *)rs1 + imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorStoreHookBegin("c.sw", state, rs1num, rs1, rs2num, rs2, imm, memorylocation, length);
#endif

#if (RVE_E_ZICSR == 1)
    if (state->trapflags.bits.storeaddressmisaligned == 1) {
        return;
    }
#endif

    RiscvEmulatorStore(memorylocation, rs2, length);

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorStoreHookEnd("c.sw", state, rs1num, rs1, rs2num, rs2, imm, memorylocation, length);
#endif
}

/**
 * rd = rd + imm
 */
static inline void RiscvEmulatorC_ADDI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const int32_t imm) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorRegImmHookBegin("c.addi", state, rdnum, rd, imm);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rd + imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorRegImmHookEnd("c.addi", state, rdnum, rd, imm);
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
    RiscvEmulatorRegImmHookBegin("c.li", state, rdnum, rd, imm);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorRegImmHookEnd("c.li", state, rdnum, rd, imm);
#endif
}

/**
 * Load upper with immediate.
 */
static inline void RiscvEmulatorC_LUI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd) {

    RiscvInstructionTypeCILuiDecoderImm_u decoderimm = {0};
    decoderimm.input.imm16_12 = state->instruction.cilui.imm16_12;
    decoderimm.input.imm17 = state->instruction.cilui.imm17;

    // Sign extend.
    if (decoderimm.input.imm17 == 1) {
        decoderimm.input.imm31_18 = 0x3FFF;
    }

    int32_t imm = decoderimm.output.imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorRegImmHookBegin("c.lui", state, rdnum, rd, imm);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorRegImmHookEnd("c.lui", state, rdnum, rd, imm);
#endif
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
    RiscvEmulatorRegRegHookBegin("c.add", state, rdnum, rd, rs2num, rs2);
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int32_t *)rd + *(int32_t *)rs2;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorRegRegHookEnd("c.add", state, rdnum, rd, rs2num, rs2);
#endif
}

/**
 * Process compressed opcodes.
 */
static inline void RiscvEmulatorOpcodeCompressed(RiscvEmulatorState_t *state) {
    RiscvInstructionTypeCDecoderOpcode_u decoderOpcode16;
    decoderOpcode16.input.funct3 = state->instruction.copcode.funct3;
    decoderOpcode16.input.op = state->instruction.copcode.op;
    uint8_t opfunct3 = decoderOpcode16.output.opfunct3;

    int8_t rdnum = -1;
    int8_t rs1num = -1;
    int8_t rs2num = -1;
    uint32_t imm = 0;
    void *rd = 0;
    void *rs1 = 0;
    void *rs2 = 0;

    // Whenever possible, combine decoding instruction bits for multiple instructions.
    switch (opfunct3) {
        case OPCODE16_ADDI:
        case OPCODE16_LI: {
            RiscvInstructionTypeCIDecoderImm_u RiscvInstructionTypeCIDecoderImm = {0};
            RiscvInstructionTypeCIDecoderImm.input.imm4_0 = state->instruction.citype.imm4_0;
            RiscvInstructionTypeCIDecoderImm.input.imm5 = state->instruction.citype.imm5;

            // Sign extend 6 bits to 32.
            if (RiscvInstructionTypeCIDecoderImm.input.imm5 == 1) {
                RiscvInstructionTypeCIDecoderImm.input.imm31_6 = 0x3FFFFFF;
            }

            imm = RiscvInstructionTypeCIDecoderImm.output.imm;

            rdnum = state->instruction.citype.rd;
            break;
        }
        case OPCODE16_SW:
        case OPCODE16_FSW: {

            RiscvInstructionTypeCSDecoderImm_u RiscvInstructionTypeCSDecoderImm = {0};
            RiscvInstructionTypeCSDecoderImm.input.uimm2 = state->instruction.cstype.uimm2;
            RiscvInstructionTypeCSDecoderImm.input.uimm5_3 = state->instruction.cstype.uimm5_3;
            RiscvInstructionTypeCSDecoderImm.input.uimm6 = state->instruction.cstype.uimm6;
            imm = RiscvInstructionTypeCSDecoderImm.output.uimm;

            rs1num = state->instruction.cstype.rs1p + 8;
            rs2num = state->instruction.cstype.rs2p + 8;
            break;
        }
    }

    if (rdnum >= 0) {
        rd = &state->registers.array.location[rdnum];
    }
    if (rs1num >= 0) {
        rs1 = &state->registers.array.location[rs1num];
    }
    if (rs2num >= 0) {
        rs2 = &state->registers.array.location[rs2num];
    }

    switch (opfunct3) {
        case OPCODE16_ADDI4SPN:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_FLD:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_LW:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_FLW:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_FSD:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_SW:
            RiscvEmulatorC_SW(state, rs1num, rs1, rs2num, rs2, imm);
            break;
        case OPCODE16_FSW:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_ADDI:
            RiscvEmulatorC_ADDI(state, rdnum, rd, imm);
            break;
        case OPCODE16_JAL:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_LI:
            RiscvEmulatorC_LI(state, rdnum, rd, imm);
            break;
        case OPCODE16_LUI_ADDI16SP: {
            rdnum = state->instruction.cilui.rd;
            rd = &state->registers.array.location[rdnum];

            if (rdnum == 2) {
                // ADDI16SP
                state->trapflags.bits.illegalinstruction = 1;
            } else {
                RiscvEmulatorC_LUI(state, rdnum, rd);
            }
            break;
        }
        case OPCODE16_MISCALU:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_J:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_BEQZ:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_BNEZ:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_SLLI:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_FLDSP:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_LWSP:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_FLWSP:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_JALR_MV_ADD:
            rdnum = state->instruction.crtype.rd;
            rd = &state->registers.array.location[rdnum];
            rs2num = state->instruction.crtype.rs2;
            rs2 = &state->registers.array.location[rs2num];

            if (state->instruction.crtype.funct4 == FUNCT4_MV) {
                // jr, mv
                state->trapflags.bits.illegalinstruction = 1;
            } else /* FUNCT4_ADD */
            {
                if (rdnum == 0 &&
                    rs2num == 0) {
                    // ebreak
                    state->trapflags.bits.illegalinstruction = 1;
                } else if (rs2num == 0) {
                    // jalr
                    state->trapflags.bits.illegalinstruction = 1;
                } else {
                    RiscvEmulatorC_ADD(state, rdnum, rd, rs2num, rs2);
                }
            }
            break;
        case OPCODE16_FSDSP:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        default:
            state->trapflags.bits.illegalinstruction = 1;
            break;
    }
}

#endif

#endif