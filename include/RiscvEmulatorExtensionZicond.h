/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZicond_H_
#define RiscvEmulatorExtensionZicond_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZICOND == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Conditional zero funnel for the register variants czero.eqz and czero.nez.
 * variant is the combined funct7/funct3 decoder value
 * (FUNCT7_FUNCT3_OPERATION_CZEROEQZ/CZERONEZ), decoded once by the caller
 * and shared by a single call site.
 */
static inline void RiscvEmulatorCZEROOperation(
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
        case FUNCT7_FUNCT3_OPERATION_CZEROEQZ:
            hc.instruction = "czero.eqz";
            break;
        default: // FUNCT7_FUNCT3_OPERATION_CZERONEZ
            hc.instruction = "czero.nez";
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

    const uint32_t rs1value = *(const uint32_t *)rs1;
    const uint32_t rs2value = *(const uint32_t *)rs2;

    uint32_t result = rs1value;

    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_CZEROEQZ:
            // Conditional zero if equal zero: write zero to rd if rs2 equals
            // zero, otherwise copy rs1. rd = (rs2 == 0) ? 0 : rs1
            if (rs2value == 0) {
                result = 0;
            }
            break;
        default: // FUNCT7_FUNCT3_OPERATION_CZERONEZ
            // Conditional zero if not equal zero: write zero to rd if rs2
            // does not equal zero, otherwise copy rs1.
            // rd = (rs2 != 0) ? 0 : rs1
            if (rs2value != 0) {
                result = 0;
            }
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

#endif

#endif
