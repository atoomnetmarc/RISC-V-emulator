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
 * Load byte: load a byte from rs1 plus the offset, zero-extended.
 * rd = zeroext(memory[rs1 + offset])
 */
static inline void RiscvEmulatorC_LBU(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const uint8_t offset) {

    uint8_t length = sizeof(uint8_t);

    uint32_t memorylocation = *(const uint32_t *)rs1 + offset;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.lbu";
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
 * Load halfword: load a halfword from rs1 plus the offset, zero-extended.
 * rd = zeroext(memory[rs1 + offset])
 */
static inline void RiscvEmulatorC_LHU(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const uint8_t offset) {

    uint8_t length = sizeof(uint16_t);

    uint32_t memorylocation = *(const uint32_t *)rs1 + offset;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.lhu";
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
 * Load halfword: load a halfword from rs1 plus the offset, sign-extended.
 * rd = signext(memory[rs1 + offset])
 */
static inline void RiscvEmulatorC_LH(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const uint8_t offset) {

    uint8_t length = sizeof(uint16_t);

    uint32_t memorylocation = *(const uint32_t *)rs1 + offset;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.lh";
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

    // Sign extend the loaded halfword.
    if (loadedvalue & 0x8000) {
        loadedvalue |= 0xFFFF0000;
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
 * Store byte: store the least significant byte of rs2 to rs1 plus the offset.
 * memory[rs1 + offset] = rs2[7:0]
 */
static inline void RiscvEmulatorC_SB(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const uint8_t offset) {

    uint8_t length = sizeof(uint8_t);

    uint32_t memorylocation = *(const uint32_t *)rs1 + offset;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.sb";
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
 * Store halfword: store the least significant halfword of rs2 to rs1 plus the offset.
 * memory[rs1 + offset] = rs2[15:0]
 */
static inline void RiscvEmulatorC_SH(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rs1num __attribute__((unused)),
    void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const uint8_t offset) {

    uint8_t length = sizeof(uint16_t);

    uint32_t memorylocation = *(const uint32_t *)rs1 + offset;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.sh";
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
 * Zero-extend byte: zero-extend the least significant byte of rd to XLEN bits.
 * rd = zeroext(rd[7:0])
 */
static inline void RiscvEmulatorC_ZEXTB(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.zext.b";
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

    *(uint32_t *)rd &= 0xFF;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Sign-extend byte: sign-extend the least significant byte of rd to XLEN bits.
 * rd = signext(rd[7:0])
 */
static inline void RiscvEmulatorC_SEXTB(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.sext.b";
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

    if (*(uint32_t *)rd & 0x80) {
        *(uint32_t *)rd |= 0xFFFFFF00;
    } else {
        *(uint32_t *)rd &= 0xFF;
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
 * Zero-extend halfword: zero-extend the least significant halfword of rd to XLEN bits.
 * rd = zeroext(rd[15:0])
 */
static inline void RiscvEmulatorC_ZEXTH(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.zext.h";
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

    *(uint32_t *)rd &= 0xFFFF;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Sign-extend halfword: sign-extend the least significant halfword of rd to XLEN bits.
 * rd = signext(rd[15:0])
 */
static inline void RiscvEmulatorC_SEXTH(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.sext.h";
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

    if (*(uint32_t *)rd & 0x8000) {
        *(uint32_t *)rd |= 0xFFFF0000;
    } else {
        *(uint32_t *)rd &= 0xFFFF;
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
 * Not: invert all bits of rd.
 * rd = ~rd
 */
static inline void RiscvEmulatorC_NOT(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum __attribute__((unused)),
    void *rd) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.not";
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

    *(uint32_t *)rd = ~*(uint32_t *)rd;

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
