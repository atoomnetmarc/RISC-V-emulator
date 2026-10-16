/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZcb_H_
#define RiscvEmulatorExtensionZcb_H_

#include "RiscvEmulatorConfig.h"

#if ((RVE_E_ZCB == 1) && (RVE_E_C == 1))

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorHook.h"
#include "RiscvEmulatorType.h"

/**
 * Compressed load operation funnel: c.lbu, c.lhu and c.lh.
 * rd = zeroext(memory[rs1 + offset]) or rd = signext(memory[rs1 + offset])
 *
 * The variant is the decoded imm5_3 field; imm2 selects between c.lhu and
 * c.lh for the halfword variants and is part of the offset for c.lbu.
 */
static inline void RiscvEmulatorC_LoadOperation(
    RiscvEmulatorState_t *state,
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const uint8_t imm5_3,
    const uint8_t imm2,
    const uint8_t imm6) {

    uint8_t offset = 0;
    uint8_t length = 0;

    switch (imm5_3) {
        case FUNCT3_ZCB_LBU:
            // c.lbu. The offset is formed as uimm[1] = encoding[5]
            // and uimm[0] = encoding[6].
            offset = (uint8_t)(imm2 | (imm6 << 1));
            length = sizeof(uint8_t);
            break;
        case FUNCT3_ZCB_LH:
            // c.lhu and c.lh. The offset is formed as uimm[1] = encoding[5].
            offset = (uint8_t)(imm6 << 1);
            length = sizeof(uint16_t);
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }

    uint32_t memorylocation = *(const uint32_t *)rs1 + offset;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    switch (imm5_3) {
        case FUNCT3_ZCB_LBU:
            hc.instruction = "c.lbu";
            break;
        case FUNCT3_ZCB_LH:
            if (imm2 == 0) {
                hc.instruction = "c.lhu";
            } else {
                hc.instruction = "c.lh";
            }
            break;
    }
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.imm = offset;
    hc.immlength = sizeof(offset);
    hc.immname = "offset";
    hc.memorylocation = memorylocation;
    hc.length = length;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintLoad(state, &hc);
#endif
#endif

    // Load into a zeroed local first so the upper bytes of the 32-bit
    // register are cleared.
    uint32_t loadedvalue = 0;
    RiscvEmulatorLoad(memorylocation, &loadedvalue, length);

    // Sign extend the loaded halfword for c.lh.
    if (imm5_3 == FUNCT3_ZCB_LH && imm2 != 0) {
        if (loadedvalue & 0x8000) {
            loadedvalue |= 0xFFFF0000;
        }
    }

    *(uint32_t *)rd = loadedvalue;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintLoad(state, &hc);
#endif
#endif
}

/**
 * Compressed store operation funnel: c.sb and c.sh.
 * memory[rs1 + offset] = rs2[7:0] or memory[rs1 + offset] = rs2[15:0]
 *
 * The variant is the decoded imm5_3 field; imm2 is part of the offset for
 * c.sb and must be zero for c.sh.
 */
static inline void RiscvEmulatorC_StoreOperation(
    RiscvEmulatorState_t *state,
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const uint8_t imm5_3,
    const uint8_t imm2,
    const uint8_t imm6) {

    uint8_t offset = 0;
    uint8_t length = 0;

    switch (imm5_3) {
        case FUNCT3_ZCB_SB:
            // c.sb. The offset is formed as uimm[1] = encoding[5]
            // and uimm[0] = encoding[6].
            offset = (uint8_t)(imm2 | (imm6 << 1));
            length = sizeof(uint8_t);
            break;
        case FUNCT3_ZCB_SH:
            // c.sh. The offset is formed as uimm[1] = encoding[5].
            if (imm2 != 0) {
                state->trapflag.illegalinstruction = 1;
                return;
            }
            offset = (uint8_t)(imm6 << 1);
            length = sizeof(uint16_t);
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }

    uint32_t memorylocation = *(const uint32_t *)rs1 + offset;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    switch (imm5_3) {
        case FUNCT3_ZCB_SB:
            hc.instruction = "c.sb";
            break;
        case FUNCT3_ZCB_SH:
            hc.instruction = "c.sh";
            break;
    }
    hc.hook = HOOK_BEGIN;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2name = RiscvEmulatorGetRegisterSymbolicName(rs2num);
    hc.rs2 = rs2;
    hc.imm = offset;
    hc.immlength = sizeof(offset);
    hc.immname = "offset";
    hc.memorylocation = memorylocation;
    hc.length = length;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintStore(state, &hc);
#endif
#endif

    RiscvEmulatorStore(memorylocation, rs2, length);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintStore(state, &hc);
#endif
#endif
}

/**
 * Compressed rd operation funnel: c.zext.b, c.sext.b, c.zext.h, c.sext.h
 * and c.not. The variant is the decoded rs2p field.
 */
static inline void RiscvEmulatorC_RdOperation(
    RiscvEmulatorState_t *state,
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t variant) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    switch (variant) {
        case RS2P_ZCB_ZEXTB:
            hc.instruction = "c.zext.b";
            break;
#if (RVE_E_ZBB == 1)
        case RS2P_ZCB_SEXTB:
            hc.instruction = "c.sext.b";
            break;
        case RS2P_ZCB_ZEXTH:
            hc.instruction = "c.zext.h";
            break;
        case RS2P_ZCB_SEXTH:
            hc.instruction = "c.sext.h";
            break;
#endif
        case RS2P_ZCB_NOT:
            hc.instruction = "c.not";
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
#endif
#endif

    switch (variant) {
        case RS2P_ZCB_ZEXTB:
            *(uint32_t *)rd &= 0xFF;
            break;
#if (RVE_E_ZBB == 1)
        case RS2P_ZCB_SEXTB:
            if (*(uint32_t *)rd & 0x80) {
                *(uint32_t *)rd |= 0xFFFFFF00;
            } else {
                *(uint32_t *)rd &= 0xFF;
            }
            break;
        case RS2P_ZCB_ZEXTH:
            *(uint32_t *)rd &= 0xFFFF;
            break;
        case RS2P_ZCB_SEXTH:
            if (*(uint32_t *)rd & 0x8000) {
                *(uint32_t *)rd |= 0xFFFF0000;
            } else {
                *(uint32_t *)rd &= 0xFFFF;
            }
            break;
#endif
        case RS2P_ZCB_NOT:
            *(uint32_t *)rd = ~*(uint32_t *)rd;
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
#endif
#endif
}

/**
 * Multiply: multiply rd and rs2, keeping the lower 32 bits of the result.
 * rd = (rd * rs2)[31:0]
 */
static inline void RiscvEmulatorC_MUL(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.mul";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs2num = rs2num;
    hc.rs2name = RiscvEmulatorGetRegisterSymbolicName(rs2num);
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintRType(state, &hc);
#endif
#endif

    *(uint32_t *)rd = *(uint32_t *)rd * *(const uint32_t *)rs2;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
#endif
#endif
}

#endif

#endif
