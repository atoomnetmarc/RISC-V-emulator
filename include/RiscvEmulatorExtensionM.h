/*

Copyright 2023 Marc Ketel
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

// Multiply signed or unsigned
static inline void RiscvEmulatorMUL(void *rd, const void *rs1, const void *rs2)
{
    *(uint32_t *)rd = (*(uint32_t *)rs1 * *(uint32_t *)rs2);
}

// Multiply signed, return 32 bit MSB of resulting 64-bit value
static inline void RiscvEmulatorMULH(void *rd, const void *rs1, const void *rs2)
{
    int64_t result = (int64_t)(*(int32_t *)rs1 * (int64_t) * (int32_t *)rs2);
    *(int32_t *)rd = (result >> 32);
}

// Multiply signed rs1 and unsigned rs2, return 32 bit MSB of resulting unsigned 64-bit value
static inline void RiscvEmulatorMULHSU(void *rd, const void *rs1, const void *rs2)
{
    int64_t result = (int64_t)(*(int32_t *)rs1 * (uint64_t) * (uint32_t *)rs2);
    *(int32_t *)rd = (result >> 32);
}

// Multiply unsigned, return 32 bit MSB of resulting unsigned 64-bit value
static inline void RiscvEmulatorMULHU(void *rd, const void *rs1, const void *rs2)
{
    uint64_t result = (uint64_t)(*(uint32_t *)rs1 * (uint64_t) * (uint32_t *)rs2);
    *(uint32_t *)rd = (result >> 32);
}

// Divide signed
static inline void RiscvEmulatorDIV(void *rd, const void *rs1, const void *rs2)
{
    if (*(int32_t *)rs2 == 0)
        *(int32_t *)rd = -1;
    else
        *(int32_t *)rd = (*(int32_t *)rs1 / *(int32_t *)rs2);
}

// Divide unsigned
static inline void RiscvEmulatorDIVU(void *rd, const void *rs1, const void *rs2)
{
    *(uint32_t *)rd = (*(uint32_t *)rs1 / *(uint32_t *)rs2);
}

// Remainder signed
static inline void RiscvEmulatorREM(void *rd, const void *rs1, const void *rs2)
{
    *(int32_t *)rd = (*(int32_t *)rs1 % *(int32_t *)rs2);
}

// Remainder unsigned
static inline void RiscvEmulatorREMU(void *rd, const void *rs1, const void *rs2)
{
    *(uint32_t *)rd = (*(uint32_t *)rs1 % *(uint32_t *)rs2);
}

#endif

#endif