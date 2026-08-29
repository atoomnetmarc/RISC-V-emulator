/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZihintpause_H_
#define RiscvEmulatorExtensionZihintpause_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZIHINTPAUSE == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Pause: a hint that the hart should pause until an interrupt or another event occurs.
 * This does nothing in this emulator because there is no pipeline to stall.
 */
static inline void RiscvEmulatorPAUSE(
    RiscvEmulatorState_t *state __attribute__((unused))) {
#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "pause";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#endif
}

#endif

#endif
