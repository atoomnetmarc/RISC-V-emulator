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
 * R-type operation funnel for Zbb: andn, orn, xnor, max, maxu, min, minu,
 * rol and ror. The variant is the decoded combined funct7_3 constant.
 */
static inline void RiscvEmulatorRTypeOperation(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const uint16_t variant) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_ANDN:
            hc.instruction = "andn";
            break;
        case FUNCT7_FUNCT3_OPERATION_ORN:
            hc.instruction = "orn";
            break;
        case FUNCT7_FUNCT3_OPERATION_XNOR:
            hc.instruction = "xnor";
            break;
        case FUNCT7_FUNCT3_OPERATION_MAX:
            hc.instruction = "max";
            break;
        case FUNCT7_FUNCT3_OPERATION_MAXU:
            hc.instruction = "maxu";
            break;
        case FUNCT7_FUNCT3_OPERATION_MIN:
            hc.instruction = "min";
            break;
        case FUNCT7_FUNCT3_OPERATION_MINU:
            hc.instruction = "minu";
            break;
        case FUNCT7_FUNCT3_OPERATION_ROL:
            hc.instruction = "rol";
            break;
        case FUNCT7_FUNCT3_OPERATION_ROR:
            hc.instruction = "ror";
            break;
    }
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

    uint32_t value = *(const uint32_t *)rs1;
    uint32_t rs2value = *(const uint32_t *)rs2;
    uint32_t shamt = rs2value & 31;
    uint32_t result = value;

    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_ANDN:
            result = value & ~rs2value;
            break;
        case FUNCT7_FUNCT3_OPERATION_ORN:
            result = value | ~rs2value;
            break;
        case FUNCT7_FUNCT3_OPERATION_XNOR:
            result = ~(value ^ rs2value);
            break;
        case FUNCT7_FUNCT3_OPERATION_MAX:
            result = ((int32_t)value > (int32_t)rs2value) ? value : rs2value;
            break;
        case FUNCT7_FUNCT3_OPERATION_MAXU:
            result = (value > rs2value) ? value : rs2value;
            break;
        case FUNCT7_FUNCT3_OPERATION_MIN:
            result = ((int32_t)value < (int32_t)rs2value) ? value : rs2value;
            break;
        case FUNCT7_FUNCT3_OPERATION_MINU:
            result = (value < rs2value) ? value : rs2value;
            break;
        case FUNCT7_FUNCT3_OPERATION_ROL:
            result = value << shamt | value >> ((32 - shamt) & 31);
            break;
        case FUNCT7_FUNCT3_OPERATION_ROR:
            result = value >> shamt | value << ((32 - shamt) & 31);
            break;
    }

    *(uint32_t *)rd = result;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
#endif
#endif
}

/**
 * Unary operation funnel for Zbb: clz, ctz, cpop, sext.b and sext.h.
 * The variant is the decoded combined immfunct3 constant.
 */
static inline void RiscvEmulatorUnaryOperation(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint16_t variant) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    switch (variant) {
        case IMM11_0_FUNCT3_IMMEDIATE_CLZ:
            hc.instruction = "clz";
            break;
        case IMM11_0_FUNCT3_IMMEDIATE_CTZ:
            hc.instruction = "ctz";
            break;
        case IMM11_0_FUNCT3_IMMEDIATE_CPOP:
            hc.instruction = "cpop";
            break;
        case IMM11_0_FUNCT3_IMMEDIATE_SEXTB:
            hc.instruction = "sext.b";
            break;
        case IMM11_0_FUNCT3_IMMEDIATE_SEXTH:
            hc.instruction = "sext.h";
            break;
    }
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

    uint32_t value = *(const uint32_t *)rs1;
    uint32_t result = value;

    switch (variant) {
        case IMM11_0_FUNCT3_IMMEDIATE_CLZ: {
            uint8_t leadingzeros = 32;
            for (int8_t i = 31; i >= 0; i--) {
                if (value & ((uint32_t)1 << i)) {
                    leadingzeros = (uint8_t)(31 - i);
                    break;
                }
            }
            result = leadingzeros;
            break;
        }
        case IMM11_0_FUNCT3_IMMEDIATE_CTZ: {
            uint8_t trailingzeros = 32;
            for (int8_t i = 0; i <= 31; i++) {
                if (value & ((uint32_t)1 << i)) {
                    trailingzeros = (uint8_t)i;
                    break;
                }
            }
            result = trailingzeros;
            break;
        }
        case IMM11_0_FUNCT3_IMMEDIATE_CPOP: {
            uint8_t setbits = 0;
            for (int8_t i = 0; i <= 31; i++) {
                if (value & ((uint32_t)1 << i)) {
                    setbits++;
                }
            }
            result = setbits;
            break;
        }
        case IMM11_0_FUNCT3_IMMEDIATE_SEXTB:
            result = (uint32_t)(int32_t)(int8_t)value;
            break;
        case IMM11_0_FUNCT3_IMMEDIATE_SEXTH:
            result = (uint32_t)(int32_t)(int16_t)value;
            break;
    }

    *(uint32_t *)rd = result;

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