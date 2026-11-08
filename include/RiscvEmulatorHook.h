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
 *
 * A consumer that provides its own override in the same translation unit
 * must define RVE_HOOK_OVERRIDE before including the library, so the weak
 * definition below is skipped (a weak definition cannot be overridden
 * within the same TU).
 */
void RiscvEmulatorHook(
    const RiscvEmulatorState_t *state,
    const RiscvEmulatorHookContext_t *context);

#ifndef RVE_HOOK_OVERRIDE
__attribute__((weak)) void RiscvEmulatorHook(
    const RiscvEmulatorState_t *state,
    const RiscvEmulatorHookContext_t *context) {
}
#endif

#if defined(__GNUC__) && !defined(__TINYC__)
#pragma GCC diagnostic pop
#endif

#endif
