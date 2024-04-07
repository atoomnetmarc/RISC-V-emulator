/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorExtensionZba_H_
#define RiscvEmulatorExtensionZba_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZBA == 1)

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Shift left by x and add.
 */
static inline void RiscvEmulatorSHADD(RiscvEmulatorState_t *state, void *rd, const void *rs1, const void *rs2) {
    *(uint32_t *)rd = *(uint32_t *)rs2 + (*(uint32_t *)rs1 << state->instruction.rtypeshift.funct3_shifts);
}

#endif

#endif