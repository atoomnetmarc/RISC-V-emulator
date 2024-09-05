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
    if (state->trapflags.bits.storeaddressmisaligned == 1) {
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
 * Load upper with immediate.
 */
static inline void RiscvEmulatorC_LUI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd) {

    RiscvInstructionTypeCILuiDecoderImm_u decoderimm = {0};
    decoderimm.input.imm16_12 = state->instruction.cilui.imm16_12;
    decoderimm.input.imm17 = state->instruction.cilui.imm17;
    int32_t imm = decoderimm.output.imm;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.lui";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = imm;
    hc.immissigned = 1;
    hc.immname = "nzimm";
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
    void *sp = &state->registers.symbolic.sp;

    // Whenever possible, combine decoding instruction bits for multiple instructions.
    switch (opfunct3) {
        case OPCODE16_ADDI4SPN: {
            RiscvInstructionTypeCIWDecoderImm_u RiscvInstructionTypeCIWDecoderImm = {0};
            RiscvInstructionTypeCIWDecoderImm.input.imm2 = state->instruction.ciwtype.imm2;
            RiscvInstructionTypeCIWDecoderImm.input.imm3 = state->instruction.ciwtype.imm3;
            RiscvInstructionTypeCIWDecoderImm.input.imm5_4 = state->instruction.ciwtype.imm5_4;
            RiscvInstructionTypeCIWDecoderImm.input.imm9_6 = state->instruction.ciwtype.imm9_6;
            imm = RiscvInstructionTypeCIWDecoderImm.output.imm;

            rdnum = state->instruction.ciwtype.rdp + 8;
            break;
        }
        case OPCODE16_ADDI:
        case OPCODE16_LI: {
            RiscvInstructionTypeCIDecoderImm_u RiscvInstructionTypeCIDecoderImm = {0};
            RiscvInstructionTypeCIDecoderImm.input.imm4_0 = state->instruction.citype.imm4_0;
            RiscvInstructionTypeCIDecoderImm.input.imm5 = state->instruction.citype.imm5;

            imm = RiscvInstructionTypeCIDecoderImm.output.imm;

            rdnum = state->instruction.citype.rd;
            break;
        }
        case OPCODE16_SW:
        case OPCODE16_FSW: {
            RiscvInstructionTypeCSDecoderImm_u RiscvInstructionTypeCSDecoderImm = {0};
            RiscvInstructionTypeCSDecoderImm.input.imm2 = state->instruction.cstype.imm2;
            RiscvInstructionTypeCSDecoderImm.input.imm5_3 = state->instruction.cstype.imm5_3;
            RiscvInstructionTypeCSDecoderImm.input.imm6 = state->instruction.cstype.imm6;
            imm = RiscvInstructionTypeCSDecoderImm.output.imm;

            rs1num = state->instruction.cstype.rs1p + 8;
            rs2num = state->instruction.cstype.rs2p + 8;
            break;
        }
        case OPCODE16_LWSP:
        case OPCODE16_FLWSP: {
            RiscvInstructionTypeCILwspDecoderImm_u RiscvInstructionTypeCILwspDecoderImm = {0};
            RiscvInstructionTypeCILwspDecoderImm.input.imm4_2 = state->instruction.cilwsp.imm4_2;
            RiscvInstructionTypeCILwspDecoderImm.input.imm5 = state->instruction.cilwsp.imm5;
            RiscvInstructionTypeCILwspDecoderImm.input.imm7_6 = state->instruction.cilwsp.imm7_6;
            imm = RiscvInstructionTypeCILwspDecoderImm.output.imm;
            rdnum = state->instruction.cilwsp.rd;
            break;
        }
        case OPCODE16_SWSP:
        case OPCODE16_FSWSP: {
            RiscvInstructionTypeCSSDecoderImm_u RiscvInstructionTypeCSSDecoderImm = {0};
            RiscvInstructionTypeCSSDecoderImm.input.imm5_2 = state->instruction.csstype.imm5_2;
            RiscvInstructionTypeCSSDecoderImm.input.imm7_6 = state->instruction.csstype.imm7_6;
            imm = RiscvInstructionTypeCSSDecoderImm.output.imm;

            rs2num = state->instruction.csstype.rs2;
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
            if (imm != 0) {
                RiscvEmulatorC_ADDI4SPN(state, rdnum, rd, sp, imm);
            } else {
                state->trapflags.bits.illegalinstruction = 1;
            }
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
            RiscvEmulatorC_LWSP(state, rdnum, rd, sp, imm);
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
                if (rs2num == 0) {
                    // jr
                    state->trapflags.bits.illegalinstruction = 1;
                } else {
                    RiscvEmulatorC_MV(state, rdnum, rd, rs2num, rs2);
                }
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
        case OPCODE16_SWSP:
            RiscvEmulatorC_SWSP(state, rs2num, rs2, sp, imm);
            break;
        case OPCODE16_FSWSP:
            state->trapflags.bits.illegalinstruction = 1;
            break;
        default:
            state->trapflags.bits.illegalinstruction = 1;
            break;
    }
}

#endif

#endif