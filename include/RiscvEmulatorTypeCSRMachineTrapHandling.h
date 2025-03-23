/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeCSRMachineTrapHandling_H_
#define RiscvEmulatorTypeCSRMachineTrapHandling_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZICSR == 1)

/**
 * Scratch register for machine trap handlers.
 */
typedef struct __attribute__((packed)) {
    uint32_t mscratch;
} RiscvCSRmscratch_t;

/**
 *  Machine trap cause.
 */
typedef struct __attribute__((packed)) {
    uint32_t exceptioncode : 31;
    uint8_t interrupt : 1;
} RiscvCSRmcause_t;

/**
 *   Machine interrupt pending.
 */
typedef struct __attribute__((packed)) {
    uint32_t mip;
} RiscvCSRmip_t;

#endif

#endif
