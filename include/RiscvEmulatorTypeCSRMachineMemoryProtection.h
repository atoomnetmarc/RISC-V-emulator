/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeCSRMachineMemoryProtection_H_
#define RiscvEmulatorTypeCSRMachineMemoryProtection_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZICSR == 1)

/**
 * Physical memory protection configuration.
 */
typedef struct __attribute__((packed)) {
    uint8_t pmp0cfg;
    uint8_t pmp1cfg;
    uint8_t pmp2cfg;
    uint8_t pmp3cfg;
} RiscvCSRpmpcfg0_t;

/**
 * Physical memory protection address register.
 */
typedef struct __attribute__((packed)) {
    uint32_t address;
} RiscvCSRpmpaddr0_t;

#endif

#endif
