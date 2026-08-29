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
 * May-Be-Operation read instruction.
 *
 * Writes zero to rd.
 */
static inline void RiscvEmulatorMOPR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "mop.r";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = 0;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * May-Be-Operation read-register instruction.
 *
 * Writes zero to rd.
 */
static inline void RiscvEmulatorMOPRR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "mop.rr";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = 0;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

#endif

#endif
