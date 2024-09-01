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
 * Process compressed opcodes.
 */
static inline void RiscvEmulatorOpcodeCompressed(RiscvEmulatorState_t *state) {
    RiscvInstruction16TypeCDecoderOpcode_u decoderOpcode16;
    decoderOpcode16.input.inst15_13 = state->instruction.copcode.inst15_13;
    decoderOpcode16.input.inst1_0 = state->instruction.copcode.inst1_0;

    uint8_t rdnum;
    uint32_t imm;
    void *rd;

    switch (decoderOpcode16.output.opcode) {
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
        default: {
            state->trapflags.bits.illegalinstruction = 1;
            return;
        }
    }

    switch (decoderOpcode16.output.opcode) {
        case OPCODE16_ADDI4SPN:
            // Voeg hier de logica voor ADDI4SPN toe
            break;
        case OPCODE16_FLD:
            // Voeg hier de logica voor FLD toe
            break;
        case OPCODE16_LW:
            // Voeg hier de logica voor LW toe
            break;
        case OPCODE16_FLW:
            // Voeg hier de logica voor FLW toe
            break;
        case OPCODE16_FSD:
            // Voeg hier de logica voor FSD toe
            break;
        case OPCODE16_SW:
            // Voeg hier de logica voor SW toe
            break;
        case OPCODE16_FSW:
            // Voeg hier de logica voor FSW toe
            break;
        case OPCODE16_ADDI:
            // Voeg hier de logica voor ADDI toe
            break;
        case OPCODE16_JAL:
            // Voeg hier de logica voor JAL toe
            break;
        case OPCODE16_LI:
            RiscvEmulatorC_LI(state, rdnum, rd, imm);
            break;
        case OPCODE16_LUI_ADDI16SP:
            // Voeg hier de logica voor LUI/ADDI16SP toe
            break;
        case OPCODE16_MISCALU:
            // Voeg hier de logica voor MISCALU toe
            break;
        case OPCODE16_J:
            // Voeg hier de logica voor J toe
            break;
        case OPCODE16_BEQZ:
            // Voeg hier de logica voor BEQZ toe
            break;
        case OPCODE16_BNEZ:
            // Voeg hier de logica voor BNEZ toe
            break;
        case OPCODE16_SLLI:
            // Voeg hier de logica voor SLLI toe
            break;
        case OPCODE16_FLDSP:
            // Voeg hier de logica voor FLDSP toe
            break;
        case OPCODE16_LWSP:
            // Voeg hier de logica voor LWSP toe
            break;
        case OPCODE16_FLWSP:
            // Voeg hier de logica voor FLWSP toe
            break;
        case OPCODE16_JALR_MV_ADD:
            // Voeg hier de logica voor JALR/MV/ADD toe
            break;
        case OPCODE16_FSDSP:
            // Voeg hier de logica voor FSDSP toe
            break;
        default:
            state->trapflags.bits.illegalinstruction = 1;
            break;
    }
}

#endif

#endif