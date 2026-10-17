/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionM_H_
#define RiscvEmulatorExtensionM_H_

#if ((RVE_E_ZMMUL == 1) || (RVE_E_M == 1))

#include <stdint.h>

#include "RiscvEmulatorConfig.h"
#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Multiply and divide funnel for the register variants mul, mulh, mulhsu,
 * mulhu, div, divu, rem and remu. variant is the combined funct7/funct3
 * decoder value (FUNCT7_FUNCT3_OPERATION_MUL/MULH/MULHSU/MULHU/DIV/DIVU/
 * REM/REMU), decoded once by the caller and shared by a single call site.
 */
static inline void RiscvEmulatorMOperation(
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
        case FUNCT7_FUNCT3_OPERATION_MUL:
            hc.instruction = "mul";
            break;
        case FUNCT7_FUNCT3_OPERATION_MULH:
            hc.instruction = "mulh";
            break;
        case FUNCT7_FUNCT3_OPERATION_MULHSU:
            hc.instruction = "mulhsu";
            break;
        case FUNCT7_FUNCT3_OPERATION_MULHU:
            hc.instruction = "mulhu";
            break;
#if (RVE_E_M == 1)
        case FUNCT7_FUNCT3_OPERATION_DIV:
            hc.instruction = "div";
            break;
        case FUNCT7_FUNCT3_OPERATION_DIVU:
            hc.instruction = "divu";
            break;
        case FUNCT7_FUNCT3_OPERATION_REM:
            hc.instruction = "rem";
            break;
        default: // FUNCT7_FUNCT3_OPERATION_REMU
            hc.instruction = "remu";
            break;
#endif
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

    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_MUL:
            *(uint32_t *)rd = (*(const uint32_t *)rs1 * *(const uint32_t *)rs2);
            break;
        case FUNCT7_FUNCT3_OPERATION_MULH: {
            int64_t result = (int64_t)(*(const int32_t *)rs1 * (int64_t)*(const int32_t *)rs2);
            *(int32_t *)rd = (int32_t)(result >> 32);
            break;
        }
        case FUNCT7_FUNCT3_OPERATION_MULHSU: {
            int64_t result = (int64_t)*(const int32_t *)rs1 * (int64_t)(uint64_t)*(const uint32_t *)rs2;
            *(int32_t *)rd = (int32_t)(result >> 32);
            break;
        }
        case FUNCT7_FUNCT3_OPERATION_MULHU: {
            uint64_t result = (uint64_t)(*(const uint32_t *)rs1 * (uint64_t)*(const uint32_t *)rs2);
            *(uint32_t *)rd = (uint32_t)(result >> 32);
            break;
        }
#if (RVE_E_M == 1)
        case FUNCT7_FUNCT3_OPERATION_DIV:
            if (*(const int32_t *)rs2 == 0) {
                // Division by zero.
                *(int32_t *)rd = -1;
            } else if (*(const int32_t *)rs1 == INT32_MIN && *(const int32_t *)rs2 == -1) {
                // Overflow.
                *(int32_t *)rd = INT32_MIN;
            } else {
                *(int32_t *)rd = (*(const int32_t *)rs1 / *(const int32_t *)rs2);
            }
            break;
        case FUNCT7_FUNCT3_OPERATION_DIVU:
            if (*(const uint32_t *)rs2 == 0) {
                // Division by zero.
                *(uint32_t *)rd = UINT32_MAX;
            } else {
                *(uint32_t *)rd = (*(const uint32_t *)rs1 / *(const uint32_t *)rs2);
            }
            break;
        case FUNCT7_FUNCT3_OPERATION_REM:
            if (*(const int32_t *)rs2 == 0) {
                // Division by zero.
                *(int32_t *)rd = *(const int32_t *)rs1;
            } else if (*(const int32_t *)rs1 == INT32_MIN && *(const int32_t *)rs2 == -1) {
                // Overflow.
                *(int32_t *)rd = 0;
            } else {
                *(int32_t *)rd = (*(const int32_t *)rs1 % *(const int32_t *)rs2);
            }
            break;
        default: // FUNCT7_FUNCT3_OPERATION_REMU
            if (*(const uint32_t *)rs2 == 0) {
                // Division by zero.
                *(uint32_t *)rd = *(const uint32_t *)rs1;
            } else {
                *(uint32_t *)rd = (*(const uint32_t *)rs1 % *(const uint32_t *)rs2);
            }
            break;
#endif
    }

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
