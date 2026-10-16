/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZbc_H_
#define RiscvEmulatorExtensionZbc_H_

#include "RiscvEmulatorConfig.h"

#if ((RVE_E_ZBC == 1) || (RVE_E_ZBKC == 1))

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Carry-less multiply funnel for the register variants clmul, clmulh and
 * clmulr. variant is the combined funct7/funct3 decoder value
 * (FUNCT7_FUNCT3_OPERATION_CMUL/CMULH/CMULR), decoded once by the caller
 * and shared by a single call site.
 */
static inline void RiscvEmulatorCLMULOperation(
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
        case FUNCT7_FUNCT3_OPERATION_CMUL:
            hc.instruction = "clmul";
            break;
        case FUNCT7_FUNCT3_OPERATION_CMULH:
            hc.instruction = "clmulh";
            break;
        default: // FUNCT7_FUNCT3_OPERATION_CMULR
            hc.instruction = "clmulr";
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

    uint32_t result = 0;

    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_CMUL: {
            for (uint8_t i = 0; i < 32; i++) {
                if ((rs2value >> i) & 1) {
                    result ^= rs1value << i;
                }
            }
            break;
        }
        case FUNCT7_FUNCT3_OPERATION_CMULH: {
            for (uint8_t i = 1; i < 32; i++) {
                if ((rs2value >> i) & 1) {
                    result ^= rs1value >> (32 - i);
                }
            }
            break;
        }
        default: { // FUNCT7_FUNCT3_OPERATION_CMULR
            for (uint8_t i = 0; i < 32; i++) {
                if ((rs2value >> i) & 1) {
                    result ^= rs1value >> (32 - i - 1);
                }
            }
            break;
        }
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
