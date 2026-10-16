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
 * Single-bit operation funnel: bclr, bext, binv and bset.
 * The variant is the decoded combined funct7_3 constant.
 */
static inline void RiscvEmulatorBitOperation(
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
        case FUNCT7_FUNCT3_OPERATION_BCLR:
            hc.instruction = "bclr";
            break;
        case FUNCT7_FUNCT3_OPERATION_BEXT:
            hc.instruction = "bext";
            break;
        case FUNCT7_FUNCT3_OPERATION_BINV:
            hc.instruction = "binv";
            break;
        case FUNCT7_FUNCT3_OPERATION_BSET:
            hc.instruction = "bset";
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
    uint8_t index = *(const uint32_t *)rs2 & 31;
    uint32_t result = value;

    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_BCLR:
            result = value & ~((uint32_t)1 << index);
            break;
        case FUNCT7_FUNCT3_OPERATION_BEXT:
            result = (value >> index) & 1;
            break;
        case FUNCT7_FUNCT3_OPERATION_BINV:
            result = value ^ ((uint32_t)1 << index);
            break;
        case FUNCT7_FUNCT3_OPERATION_BSET:
            result = value | ((uint32_t)1 << index);
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
 * Single-bit immediate operation funnel: bclri, bexti, binvi and bseti.
 * The variant is the decoded combined imm11_5funct3 constant.
 */
static inline void RiscvEmulatorBitOperationImmediate(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt,
    const uint16_t variant) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    switch (variant) {
        case IMM11_5_FUNCT3_IMMEDIATE_BCLRI:
            hc.instruction = "bclri";
            break;
        case IMM11_5_FUNCT3_IMMEDIATE_BEXTI:
            hc.instruction = "bexti";
            break;
        case IMM11_5_FUNCT3_IMMEDIATE_BINVI:
            hc.instruction = "binvi";
            break;
        case IMM11_5_FUNCT3_IMMEDIATE_BSETI:
            hc.instruction = "bseti";
            break;
    }
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

    uint32_t value = *(const uint32_t *)rs1;
    uint8_t index = shamt & 31;
    uint32_t result = value;

    switch (variant) {
        case IMM11_5_FUNCT3_IMMEDIATE_BCLRI:
            result = value & ~((uint32_t)1 << index);
            break;
        case IMM11_5_FUNCT3_IMMEDIATE_BEXTI:
            result = (value >> index) & 1;
            break;
        case IMM11_5_FUNCT3_IMMEDIATE_BINVI:
            result = value ^ ((uint32_t)1 << index);
            break;
        case IMM11_5_FUNCT3_IMMEDIATE_BSETI:
            result = value | ((uint32_t)1 << index);
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

#endif

#endif
