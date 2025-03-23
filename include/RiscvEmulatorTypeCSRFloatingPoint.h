/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeCSRFloatingPoint_H_
#define RiscvEmulatorTypeCSRFloatingPoint_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZICSR == 1 && RVE_E_F == 1)

/**
 * Floating-Point Control and Status Register.
 */
typedef struct __attribute__((packed)) {
    /**
     * Floating-point rounding mode.
     */
    uint8_t frm : 3;

    /**
     * Invalid operation.
     */
    uint8_t nv : 1;

    /**
     * Divide by zero.
     */
    uint8_t dz : 1;

    /**
     * Overflow.
     */
    uint8_t of : 1;

    /**
     * Underflow.
     */
    uint8_t uf : 1;

    /**
     * Inexact.
     */
    uint8_t nx : 1;

    /**
     * Reserved bits.
     */
    uint32_t reserved : 27;
} RiscvCSRfcsr_t;

#endif

#endif