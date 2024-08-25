/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorHook_H_
#define RiscvEmulatorHook_H_

#include <stdint.h>

#include "RiscvEmulatorDefine.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__attribute__((weak)) void RiscvEmulatorCSRRWHookBegin(
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num,
    const void *rs1,
    const uint16_t csrnum,
    const void *csr) {
}

__attribute__((weak)) void RiscvEmulatorCSRRWHookEnd(
    const RiscvEmulatorState_t *state,
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num,
    const void *rs1,
    const uint16_t csrnum,
    const void *csr) {
}

#pragma GCC diagnostic pop

#endif