/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZbs_H_
#define RiscvEmulatorExtensionZbs_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZBS == 1)

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Single-bit clear: clear the bit of rs1 at the index in the low 5 bits of rs2.
 * rd = rs1 & ~(1 << (rs2 & 31))
 */
static inline void RiscvEmulatorBCLR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "bclr";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2name = RiscvEmulatorGetRegisterSymbolicName(rs2num);
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t index = *(const uint32_t *)rs2 & 31;
    *(uint32_t *)rd = *(const uint32_t *)rs1 & ~((uint32_t)1 << index);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Single-bit clear immediate: clear the bit of rs1 at index shamt.
 * rd = rs1 & ~(1 << (shamt & 31))
 */
static inline void RiscvEmulatorBCLRI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "bclri";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t index = shamt & 31;
    *(uint32_t *)rd = *(const uint32_t *)rs1 & ~((uint32_t)1 << index);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Single-bit extract: extract the bit of rs1 at the index in the low 5 bits of rs2.
 * rd = (rs1 >> (rs2 & 31)) & 1
 */
static inline void RiscvEmulatorBEXT(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "bext";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2name = RiscvEmulatorGetRegisterSymbolicName(rs2num);
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t index = *(const uint32_t *)rs2 & 31;
    *(uint32_t *)rd = (*(const uint32_t *)rs1 >> index) & 1;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Single-bit extract immediate: extract the bit of rs1 at index shamt.
 * rd = (rs1 >> (shamt & 31)) & 1
 */
static inline void RiscvEmulatorBEXTI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "bexti";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t index = shamt & 31;
    *(uint32_t *)rd = (*(const uint32_t *)rs1 >> index) & 1;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Single-bit invert: invert the bit of rs1 at the index in the low 5 bits of rs2.
 * rd = rs1 ^ (1 << (rs2 & 31))
 */
static inline void RiscvEmulatorBINV(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "binv";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2name = RiscvEmulatorGetRegisterSymbolicName(rs2num);
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t index = *(const uint32_t *)rs2 & 31;
    *(uint32_t *)rd = *(const uint32_t *)rs1 ^ ((uint32_t)1 << index);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Single-bit invert immediate: invert the bit of rs1 at index shamt.
 * rd = rs1 ^ (1 << (shamt & 31))
 */
static inline void RiscvEmulatorBINVI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "binvi";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t index = shamt & 31;
    *(uint32_t *)rd = *(const uint32_t *)rs1 ^ ((uint32_t)1 << index);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Single-bit set: set the bit of rs1 at the index in the low 5 bits of rs2.
 * rd = rs1 | (1 << (rs2 & 31))
 */
static inline void RiscvEmulatorBSET(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "bset";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2name = RiscvEmulatorGetRegisterSymbolicName(rs2num);
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t index = *(const uint32_t *)rs2 & 31;
    *(uint32_t *)rd = *(const uint32_t *)rs1 | ((uint32_t)1 << index);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Single-bit set immediate: set the bit of rs1 at index shamt.
 * rd = rs1 | (1 << (shamt & 31))
 */
static inline void RiscvEmulatorBSETI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "bseti";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t index = shamt & 31;
    *(uint32_t *)rd = *(const uint32_t *)rs1 | ((uint32_t)1 << index);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

#endif

#endif