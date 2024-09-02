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

    RiscvInstruction16TypeCQ1luiDecoderImm_u decoderimm = {0};
    decoderimm.input.imm16_12 = state->instruction.cq1lui.imm16_12;
    decoderimm.input.imm17 = state->instruction.cq1lui.imm17;

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
    RiscvInstruction16TypeCDecoderOpcode_u decoderOpcode16;
    decoderOpcode16.input.inst15_13 = state->instruction.copcode.inst15_13;
    decoderOpcode16.input.inst1_0 = state->instruction.copcode.inst1_0;
    uint8_t opcode = decoderOpcode16.output.opcode;

    uint8_t rdnum = 0;
    uint8_t rs2num = 0;
    uint32_t imm = 0;
    void *rd = 0;
    void *rs2 = 0;

    // Whenever possible, decode instruction bits only once.
    switch (opcode) {
        case OPCODE16_ADDI:
        case OPCODE16_LI: {
            RiscvInstruction16TypeCQ1v1DecoderImm_u RiscvInstruction16TypeCQ1v1DecoderImm;
            RiscvInstruction16TypeCQ1v1DecoderImm.input.imm4_0 = state->instruction.cq1v1type.imm4_0;
            RiscvInstruction16TypeCQ1v1DecoderImm.input.imm5 = state->instruction.cq1v1type.imm5;
            imm = RiscvInstruction16TypeCQ1v1DecoderImm.output.imm;
            // Sign extend 6 bits to 32.
            if (imm & 0b100000) {
                imm |= ~0b111111;
            }

            rdnum = state->instruction.cq1v1type.rd;
            rd = &state->registers.array.location[rdnum];
            break;
        }
    }

    switch (opcode) {
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
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_FSW:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_ADDI:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_JAL:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        case OPCODE16_LI:
            RiscvEmulatorC_LI(state, rdnum, rd, imm);
            break;
        case OPCODE16_LUI_ADDI16SP: {
            rdnum = state->instruction.cq1lui.rd;
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
            rdnum = state->instruction.cq2v1type.rd;
            rd = &state->registers.array.location[rdnum];
            rs2num = state->instruction.cq2v1type.rs2;
            rs2 = &state->registers.array.location[rs2num];

            if (state->instruction.cq2v1type.inst12 == 1) {
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