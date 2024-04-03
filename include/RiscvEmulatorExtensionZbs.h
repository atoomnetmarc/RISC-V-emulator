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
 * Single-Bit Clear
 */
static inline void RiscvEmulatorBCLR(void *rd, const void *rs1, const void *rs2) {
    uint8_t index = *(uint32_t *)rs2 & 31;
    *(uint32_t *)rd = *(uint32_t *)rs1 & ~(1 << index);
}

/**
 * Single-Bit Extract
 */
static inline void RiscvEmulatorBEXT(void *rd, const void *rs1, const void *rs2) {
    uint8_t index = *(uint32_t *)rs2 & 31;
    *(uint32_t *)rd = (*(uint32_t *)rs1 >> index) & 1;
}

/**
 * Single-Bit Invert
 */
static inline void RiscvEmulatorBINV(void *rd, const void *rs1, const void *rs2) {
    uint8_t index = *(uint32_t *)rs2 & 31;
    *(uint32_t *)rd = *(uint32_t *)rs1 ^ (1 << index);
}

/**
 * Single-Bit Set
 */
static inline void RiscvEmulatorBSET(void *rd, const void *rs1, const void *rs2) {
    uint8_t index = *(uint32_t *)rs2 & 31;
    *(uint32_t *)rd = *(uint32_t *)rs1 | (1 << index);
}

#endif

#endif