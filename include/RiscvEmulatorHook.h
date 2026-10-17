/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorHook_H_
#define RiscvEmulatorHook_H_

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorTypeHook.h"

#if defined(__GNUC__) && !defined(__TINYC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

/**
 * Generic hook function.
 */
void RiscvEmulatorHook(
    const RiscvEmulatorState_t *state,
    const RiscvEmulatorHookContext_t *context);

__attribute__((weak)) void RiscvEmulatorHook(
    const RiscvEmulatorState_t *state,
    const RiscvEmulatorHookContext_t *context) {
}

#if defined(__GNUC__) && !defined(__TINYC__)
#pragma GCC diagnostic pop
#endif

#endif
