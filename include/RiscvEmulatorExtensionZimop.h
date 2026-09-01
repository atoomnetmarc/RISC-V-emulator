/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZimop_H_
#define RiscvEmulatorExtensionZimop_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZIMOP == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * May-Be-Operation read: a hint instruction that reads no registers and writes zero to rd.
 * rd = 0
 */
static inline void RiscvEmulatorMOPR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "mop.r";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = 0;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif
}

/**
 * May-Be-Operation read-register: a hint instruction that reads no registers and writes zero to rd.
 * rd = 0
 */
static inline void RiscvEmulatorMOPRR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "mop.rr";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = 0;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif
}

#endif

#endif
