/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZba_H_
#define RiscvEmulatorExtensionZba_H_

#if (RVE_E_ZBA == 1)

#include <stdint.h>

#include "RiscvEmulatorConfig.h"
#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Shift and add: shift rs1 left by the amount in funct3 (1, 2 or 3) and add rs2.
 * rd = rs2 + (rs1 << x)
 */
static inline void RiscvEmulatorSHADD(
    RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "shadd";
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

    *(uint32_t *)rd = *(const uint32_t *)rs2 + (*(const uint32_t *)rs1 << state->instruction.rtypeshift.funct3_shifts);

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