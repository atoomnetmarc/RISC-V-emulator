/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorExtensionZbb_H_
#define RiscvEmulatorExtensionZbb_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZBB == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * AND with inverted operand.
 */
static inline void RiscvEmulatorANDN(
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

    *(uint32_t *)rd = *(uint32_t *)rs1 & ~*(uint32_t *)rs2;
}

/**
 * OR with inverted operand.
 */
static inline void RiscvEmulatorORN(
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

    *(uint32_t *)rd = *(uint32_t *)rs1 | ~*(uint32_t *)rs2;
}

/**
 * Exclusive NOR.
 */
static inline void RiscvEmulatorXNOR(
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

    *(uint32_t *)rd = ~(*(uint32_t *)rs1 ^ *(uint32_t *)rs2);
}

/**
 * Count leading zero bits.
 */
static inline void RiscvEmulatorCLZ(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

    if (rdnum == 0) {
        return;
    }

    uint8_t leadingZeros = 32;

    for (int8_t i = 31; i >= 0; i--) {
        if (*(uint32_t *)rs1 & (1 << i)) {
            leadingZeros = 31 - i;
            break;
        }
    }

    *(uint32_t *)rd = leadingZeros;
}

/**
 * Count trailing zero bits.
 */
static inline void RiscvEmulatorCTZ(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

    if (rdnum == 0) {
        return;
    }

    uint8_t trailingZeros = 32;

    for (int8_t i = 0; i <= 31; i++) {
        if (*(uint32_t *)rs1 & (1 << i)) {
            trailingZeros = i;
            break;
        }
    }

    *(uint32_t *)rd = trailingZeros;
}

/**
 * Count set bits.
 */
static inline void RiscvEmulatorCPOP(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

    if (rdnum == 0) {
        return;
    }

    uint8_t setBits = 0;

    for (int8_t i = 0; i <= 31; i++) {
        if (*(uint32_t *)rs1 & (1 << i)) {
            setBits++;
        }
    }

    *(uint32_t *)rd = setBits;
}

/**
 * Maximum.
 */
static inline void RiscvEmulatorMAX(
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

    if (*(int32_t *)rs1 > *(int32_t *)rs2) {
        *(int32_t *)rd = *(int32_t *)rs1;
    } else {
        *(int32_t *)rd = *(int32_t *)rs2;
    }
}

/**
 * Unsigned maximum.
 */
static inline void RiscvEmulatorMAXU(
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

    if (*(uint32_t *)rs1 > *(uint32_t *)rs2) {
        *(uint32_t *)rd = *(uint32_t *)rs1;
    } else {
        *(uint32_t *)rd = *(uint32_t *)rs2;
    }
}

/**
 * Minimum.
 */
static inline void RiscvEmulatorMIN(
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

    if (*(int32_t *)rs1 < *(int32_t *)rs2) {
        *(int32_t *)rd = *(int32_t *)rs1;
    } else {
        *(int32_t *)rd = *(int32_t *)rs2;
    }
}

/**
 * Unsigned minimum.
 */
static inline void RiscvEmulatorMINU(
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

    if (*(uint32_t *)rs1 < *(uint32_t *)rs2) {
        *(uint32_t *)rd = *(uint32_t *)rs1;
    } else {
        *(uint32_t *)rd = *(uint32_t *)rs2;
    }
}

/**
 * Sign-extend byte.
 */
static inline void RiscvEmulatorSEXTB(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int8_t *)rs1;
}

/**
 * Sign-extend halfword.
 */
static inline void RiscvEmulatorSEXTH(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

    if (rdnum == 0) {
        return;
    }

    *(int32_t *)rd = *(int16_t *)rs1;
}

/**
 * Zero-extend halfword.
 */
static inline void RiscvEmulatorZEXTH(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = *(uint16_t *)rs1;
}

/**
 * Rotate left.
 */
static inline void RiscvEmulatorROL(
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

    *(uint32_t *)rd =
        *(uint32_t *)rs1 << *(uint32_t *)rs2 |
        *(uint32_t *)rs1 >> (32 - *(uint32_t *)rs2);
}

/**
 * Rotate right.
 */
static inline void RiscvEmulatorROR(
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

    *(uint32_t *)rd =
        *(uint32_t *)rs1 >> *(uint32_t *)rs2 |
        *(uint32_t *)rs1 << (32 - *(uint32_t *)rs2);
}

/**
 * Rotate right.
 */
static inline void RiscvEmulatorRORI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt) {

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd =
        *(uint32_t *)rs1 >> shamt |
        *(uint32_t *)rs1 << (32 - shamt);
}

/**
 * Bitwise OR-Combine, byte granule.
 */
static inline void RiscvEmulatorORCB(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

    if (rdnum == 0) {
        return;
    }

    for (uint8_t i = 0; i <= 3; i++) {
        if (((uint8_t *)rs1)[i] == 0) {
            ((uint8_t *)rd)[i] = 0;
        } else {
            ((uint8_t *)rd)[i] = UINT8_MAX;
        }
    }
}

/**
 * Byte-reverse register.
 */
static inline void RiscvEmulatorREV8(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

    if (rdnum == 0) {
        return;
    }

    uint32_t temp = 0;

    for (uint8_t i = 0; i <= 3; i++) {
        ((uint8_t *)&temp)[i] = ((uint8_t *)rs1)[3 - i];
    }

    *(uint32_t *)rd = temp;
}

#endif

#endif