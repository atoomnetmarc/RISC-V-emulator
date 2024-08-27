/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorExtensionZbs_H_
#define RiscvEmulatorExtensionZbs_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZBS == 1)

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Single-Bit Clear.
 */
static inline void RiscvEmulatorBCLR(
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

    uint8_t index = *(uint32_t *)rs2 & 31;
    *(uint32_t *)rd = *(uint32_t *)rs1 & ~(1 << index);
}

/**
 * Single-Bit Clear.
 */
static inline void RiscvEmulatorBCLRI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint32_t shamt) {

    if (rdnum == 0) {
        return;
    }

    uint8_t index = shamt & 31;
    *(uint32_t *)rd = *(uint32_t *)rs1 & ~(1 << index);
}

/**
 * Single-Bit Extract.
 */
static inline void RiscvEmulatorBEXT(
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

    uint8_t index = *(uint32_t *)rs2 & 31;
    *(uint32_t *)rd = (*(uint32_t *)rs1 >> index) & 1;
}

/**
 * Single-Bit Extract.
 */
static inline void RiscvEmulatorBEXTI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint32_t shamt) {

    if (rdnum == 0) {
        return;
    }

    uint8_t index = shamt & 31;
    *(uint32_t *)rd = (*(uint32_t *)rs1 >> index) & 1;
}

/**
 * Single-Bit Invert.
 */
static inline void RiscvEmulatorBINV(
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

    uint8_t index = *(uint32_t *)rs2 & 31;
    *(uint32_t *)rd = *(uint32_t *)rs1 ^ (1 << index);
}

/**
 * Single-Bit Invert.
 */
static inline void RiscvEmulatorBINVI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint32_t shamt) {

    if (rdnum == 0) {
        return;
    }

    uint8_t index = shamt & 31;
    *(uint32_t *)rd = *(uint32_t *)rs1 ^ (1 << index);
}

/**
 * Single-Bit Set.
 */
static inline void RiscvEmulatorBSET(
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

    uint8_t index = *(uint32_t *)rs2 & 31;
    *(uint32_t *)rd = *(uint32_t *)rs1 | (1 << index);
}

/**
 * Single-Bit Set.
 */
static inline void RiscvEmulatorBSETI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint32_t shamt) {

    if (rdnum == 0) {
        return;
    }

    uint8_t index = shamt & 31;
    *(uint32_t *)rd = *(uint32_t *)rs1 | (1 << index);
}

#endif

#endif