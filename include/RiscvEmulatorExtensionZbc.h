/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorExtensionZbc_H_
#define RiscvEmulatorExtensionZbc_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZBC == 1)

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Carry-less multiply low-part.
 */
static inline void RiscvEmulatorCMUL(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

    if (rdnum == 0) {
        return;
    }

    uint32_t output = 0;

    for (uint8_t i = 0; i < 32; i++) {
        if ((*(uint32_t *)rs2 >> i) & 1) {
            output ^= *(uint32_t *)rs1 << i;
        }
    }

    *(uint32_t *)rd = output;
}

/**
 * Carry-less multiply high-part.
 */
static inline void RiscvEmulatorCMULH(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

    if (rdnum == 0) {
        return;
    }

    uint32_t output = 0;

    for (uint8_t i = 1; i < 32; i++) {
        if ((*(uint32_t *)rs2 >> i) & 1) {
            output ^= *(uint32_t *)rs1 >> (32 - i);
        }
    }

    *(uint32_t *)rd = output;
}

/**
 * Carry-less multiply reversed.
 */
static inline void RiscvEmulatorCMULR(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

    if (rdnum == 0) {
        return;
    }

    uint32_t output = 0;

    for (uint8_t i = 0; i < 32; i++) {
        if ((*(uint32_t *)rs2 >> i) & 1) {
            output ^= *(uint32_t *)rs1 >> (32 - i - 1);
        }
    }

    *(uint32_t *)rd = output;
}

#endif

#endif