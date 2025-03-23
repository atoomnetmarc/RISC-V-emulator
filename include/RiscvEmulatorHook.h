/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorHook_H_
#define RiscvEmulatorHook_H_

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorTypeHook.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * Generic hook function.
 */
__attribute__((weak)) void RiscvEmulatorHook(
    const RiscvEmulatorState_t *state,
    const RiscvEmulatorHookContext_t *context) {
}

#pragma GCC diagnostic pop

#endif
