/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZbkx_H_
#define RiscvEmulatorExtensionZbkx_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZBKX == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Crossbar permutation funnel for the register variants xperm4 and xperm8.
 * variant is the combined funct7/funct3 decoder value
 * (FUNCT7_FUNCT3_OPERATION_XPERM4/XPERM8), decoded once by the caller and
 * shared by a single call site.
 */
static inline void RiscvEmulatorXPERMOperation(
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
        case FUNCT7_FUNCT3_OPERATION_XPERM4:
            hc.instruction = "xperm4";
            break;
        default: // FUNCT7_FUNCT3_OPERATION_XPERM8
            hc.instruction = "xperm8";
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

    const uint32_t input = *(const uint32_t *)rs2;
    const uint32_t table = *(const uint32_t *)rs1;

    uint32_t result = 0;

    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_XPERM4: {
            // Crossbar permutation of nibbles: each nibble of rs2 selects a
            // nibble of rs1, nibbles with an index of 8 or higher become zero.
            // rd[i] = (rs2[i] < 8) ? rs1[rs2[i]] : 0
            for (uint8_t i = 0; i < 8; i++) {
                uint8_t index = (input >> (i * 4)) & 0xF;
                if (index < 8) {
                    result |= ((table >> (index * 4)) & 0xF) << (i * 4);
                }
            }
            break;
        }
        default: { // FUNCT7_FUNCT3_OPERATION_XPERM8
            // Crossbar permutation of bytes: each byte of rs2 selects a byte
            // of rs1, bytes with an index of 4 or higher become zero.
            // rd[i] = (rs2[i] < 4) ? rs1[rs2[i]] : 0
            for (uint8_t i = 0; i < 4; i++) {
                uint8_t index = (input >> (i * 8)) & 0xFF;
                if (index < 4) {
                    result |= ((table >> (index * 8)) & 0xFF) << (i * 8);
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
