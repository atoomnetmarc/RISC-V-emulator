/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZbb_H_
#define RiscvEmulatorExtensionZbb_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZBB == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * And with inverted operand: and rs1 with the bitwise inverse of rs2.
 * rd = rs1 & ~rs2
 */
static inline void RiscvEmulatorANDN(
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
    hc.instruction = "andn";
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

    *(uint32_t *)rd = *(const uint32_t *)rs1 & ~*(const uint32_t *)rs2;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Or with inverted operand: or rs1 with the bitwise inverse of rs2.
 * rd = rs1 | ~rs2
 */
static inline void RiscvEmulatorORN(
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
    hc.instruction = "orn";
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

    *(uint32_t *)rd = *(const uint32_t *)rs1 | ~*(const uint32_t *)rs2;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Exclusive nor: xor rs1 and rs2, then invert the result.
 * rd = ~(rs1 ^ rs2)
 */
static inline void RiscvEmulatorXNOR(
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
    hc.instruction = "xnor";
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

    *(uint32_t *)rd = ~(*(const uint32_t *)rs1 ^ *(const uint32_t *)rs2);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Count leading zeros: count the number of leading zero bits in rs1.
 * rd = countleadingzeros(rs1)
 */
static inline void RiscvEmulatorCLZ(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "clz";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t leadingzeros = 32;

    for (int8_t i = 31; i >= 0; i--) {
        if (*(const uint32_t *)rs1 & ((uint32_t)1 << i)) {
            leadingzeros = (uint8_t)(31 - i);
            break;
        }
    }

    *(uint32_t *)rd = leadingzeros;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Count trailing zeros: count the number of trailing zero bits in rs1.
 * rd = counttrailingzeros(rs1)
 */
static inline void RiscvEmulatorCTZ(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "ctz";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t trailingzeros = 32;

    for (int8_t i = 0; i <= 31; i++) {
        if (*(const uint32_t *)rs1 & ((uint32_t)1 << i)) {
            trailingzeros = (uint8_t)i;
            break;
        }
    }

    *(uint32_t *)rd = trailingzeros;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Count population: count the number of set bits in rs1.
 * rd = populationcount(rs1)
 */
static inline void RiscvEmulatorCPOP(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "cpop";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint8_t setbits = 0;

    for (int8_t i = 0; i <= 31; i++) {
        if (*(const uint32_t *)rs1 & ((uint32_t)1 << i)) {
            setbits++;
        }
    }

    *(uint32_t *)rd = setbits;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Maximum: take the larger of rs1 and rs2 as signed integers.
 * rd = (rs1 > rs2) ? rs1 : rs2
 */
static inline void RiscvEmulatorMAX(
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
    hc.instruction = "max";
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

    if (*(const int32_t *)rs1 > *(const int32_t *)rs2) {
        *(int32_t *)rd = *(const int32_t *)rs1;
    } else {
        *(int32_t *)rd = *(const int32_t *)rs2;
    }

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Maximum unsigned: take the larger of rs1 and rs2 as unsigned integers.
 * rd = (rs1 > rs2) ? rs1 : rs2
 */
static inline void RiscvEmulatorMAXU(
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
    hc.instruction = "maxu";
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

    if (*(const uint32_t *)rs1 > *(const uint32_t *)rs2) {
        *(uint32_t *)rd = *(const uint32_t *)rs1;
    } else {
        *(uint32_t *)rd = *(const uint32_t *)rs2;
    }

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Minimum: take the smaller of rs1 and rs2 as signed integers.
 * rd = (rs1 < rs2) ? rs1 : rs2
 */
static inline void RiscvEmulatorMIN(
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
    hc.instruction = "min";
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

    if (*(const int32_t *)rs1 < *(const int32_t *)rs2) {
        *(int32_t *)rd = *(const int32_t *)rs1;
    } else {
        *(int32_t *)rd = *(const int32_t *)rs2;
    }

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Minimum unsigned: take the smaller of rs1 and rs2 as unsigned integers.
 * rd = (rs1 < rs2) ? rs1 : rs2
 */
static inline void RiscvEmulatorMINU(
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
    hc.instruction = "minu";
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

    if (*(const uint32_t *)rs1 < *(const uint32_t *)rs2) {
        *(uint32_t *)rd = *(const uint32_t *)rs1;
    } else {
        *(uint32_t *)rd = *(const uint32_t *)rs2;
    }

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Sign-extend byte: take the least significant byte of rs1 and sign-extend it to XLEN bits.
 * rd = signext(rs1[7:0])
 */
static inline void RiscvEmulatorSEXT_B(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "sext.b";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(const int8_t *)rs1;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Sign-extend halfword: take the least significant halfword of rs1 and sign-extend it to XLEN bits.
 * rd = signext(rs1[15:0])
 */
static inline void RiscvEmulatorSEXT_H(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "sext.h";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(const int16_t *)rs1;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Zero-extend halfword: take the least significant halfword of rs1 and zero-extend it to XLEN bits.
 * rd = zeroext(rs1[15:0])
 */
static inline void RiscvEmulatorZEXT_H(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "zext.h";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(const uint16_t *)rs1;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif
}

/**
 * Rotate left: rotate rs1 left by the amount in the low 5 bits of rs2.
 * rd = rotl(rs1, rs2)
 */
static inline void RiscvEmulatorROL(
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
    hc.instruction = "rol";
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

    uint32_t shamt = *(const uint32_t *)rs2 & 31;

    *(uint32_t *)rd =
        *(const uint32_t *)rs1 << shamt |
        *(const uint32_t *)rs1 >> ((32 - shamt) & 31);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Rotate right: rotate rs1 right by the amount in the low 5 bits of rs2.
 * rd = rotr(rs1, rs2)
 */
static inline void RiscvEmulatorROR(
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
    hc.instruction = "ror";
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

    uint32_t shamt = *(const uint32_t *)rs2 & 31;

    *(uint32_t *)rd =
        *(const uint32_t *)rs1 >> shamt |
        *(const uint32_t *)rs1 << ((32 - shamt) & 31);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
    #endif
#endif
}

/**
 * Rotate right immediate: rotate rs1 right by shamt.
 * rd = rotr(rs1, shamt)
 */
static inline void RiscvEmulatorRORI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "rori";
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

    *(uint32_t *)rd =
        *(const uint32_t *)rs1 >> shamt |
        *(const uint32_t *)rs1 << ((32 - shamt) & 31);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif
}

/**
 * Or combine byte: set each byte of rd to 0xff if the corresponding byte of rs1 is non-zero, or 0x00 otherwise.
 * rd[i] = (rs1[i] == 0) ? 0x00 : 0xff
 */
static inline void RiscvEmulatorORC_B(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "orc.b";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    for (uint8_t i = 0; i <= 3; i++) {
        if (((const uint8_t *)rs1)[i] == 0) {
            ((uint8_t *)rd)[i] = 0;
        } else {
            ((uint8_t *)rd)[i] = UINT8_MAX;
        }
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
 * Reverse byte: reverse the byte order of rs1.
 * rd = bytereversal(rs1)
 */
static inline void RiscvEmulatorREV8(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "rev8";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint32_t temp = 0;

    for (uint8_t i = 0; i <= 3; i++) {
        ((uint8_t *)&temp)[i] = ((const uint8_t *)rs1)[3 - i];
    }

    *(uint32_t *)rd = temp;

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