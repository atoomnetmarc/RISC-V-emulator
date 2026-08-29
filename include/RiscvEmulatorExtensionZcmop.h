/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZcmop_H_
#define RiscvEmulatorExtensionZcmop_H_

#include "RiscvEmulatorConfig.h"

#if ((RVE_E_ZCMOP == 1) && (RVE_E_C == 1))

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorHook.h"
#include "RiscvEmulatorType.h"

/**
 * Compressed May-Be-Operation: a hint instruction that performs no operation.
 * This does nothing in this emulator.
 */
static inline void RiscvEmulatorCMOP(
    RiscvEmulatorState_t *state __attribute__((unused))) {
#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "c.mop";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#endif
}

#endif

#endif
