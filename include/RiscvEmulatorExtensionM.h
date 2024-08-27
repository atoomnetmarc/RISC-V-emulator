/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorExtensionM_H_
#define RiscvEmulatorExtensionM_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_M == 1)

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Multiply signed or unsigned.
 */
static inline void RiscvEmulatorMUL(
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

    *(uint32_t *)rd = (*(uint32_t *)rs1 * *(uint32_t *)rs2);
}

/**
 * Multiply signed, return 32 bit MSB of resulting 64-bit value.
 */
static inline void RiscvEmulatorMULH(
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

    int64_t result = (int64_t)(*(int32_t *)rs1 * (int64_t) * (int32_t *)rs2);
    *(int32_t *)rd = (result >> 32);
}

/**
 * Multiply signed rs1 and unsigned rs2, return 32 bit MSB of resulting unsigned 64-bit value.
 */
static inline void RiscvEmulatorMULHSU(
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

    int64_t result = (int64_t)(*(int32_t *)rs1 * (uint64_t) * (uint32_t *)rs2);
    *(int32_t *)rd = (result >> 32);
}

/**
 * Multiply unsigned, return 32 bit MSB of resulting unsigned 64-bit value.
 */
static inline void RiscvEmulatorMULHU(
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

    uint64_t result = (uint64_t)(*(uint32_t *)rs1 * (uint64_t) * (uint32_t *)rs2);
    *(uint32_t *)rd = (result >> 32);
}

/**
 * Divide signed.
 */
static inline void RiscvEmulatorDIV(
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

    if (*(int32_t *)rs2 == 0) {
        // Division by zero.
        *(int32_t *)rd = -1;
    } else if (*(int32_t *)rs1 == INT32_MIN && *(int32_t *)rs2 == -1) {
        // Overflow.
        *(int32_t *)rd = INT32_MIN;
    } else {
        *(int32_t *)rd = (*(int32_t *)rs1 / *(int32_t *)rs2);
    }
}

/**
 * Divide unsigned.
 */
static inline void RiscvEmulatorDIVU(
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

    if (*(uint32_t *)rs2 == 0) {
        // Division by zero.
        *(uint32_t *)rd = UINT32_MAX;
    } else {
        *(uint32_t *)rd = (*(uint32_t *)rs1 / *(uint32_t *)rs2);
    }
}

/**
 * Remainder signed.
 */
static inline void RiscvEmulatorREM(
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

    if (*(int32_t *)rs2 == 0) {
        // Division by zero.
        *(int32_t *)rd = *(int32_t *)rs1;
    } else if (*(int32_t *)rs1 == INT32_MIN && *(int32_t *)rs2 == -1) {
        // Overflow.
        *(int32_t *)rd = 0;
    } else {
        *(int32_t *)rd = (*(int32_t *)rs1 % *(int32_t *)rs2);
    }
}

/**
 * Remainder unsigned.
 */
static inline void RiscvEmulatorREMU(
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

    if (*(uint32_t *)rs2 == 0) {
        // Division by zero.
        *(uint32_t *)rd = *(uint32_t *)rs1;
    } else {
        *(uint32_t *)rd = (*(uint32_t *)rs1 % *(uint32_t *)rs2);
    }
}

#endif

#endif