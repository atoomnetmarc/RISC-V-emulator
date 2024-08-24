/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeCSRMachineTrapHandling_H_
#define RiscvEmulatorTypeCSRMachineTrapHandling_H_

#include <stdint.h>

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
 *  Machine bad address or instruction.
 */
typedef struct __attribute__((packed)) {
    uint32_t mtval;
} RiscvCSRmtval_t;

/**
 *   Machine interrupt pending.
 */
typedef struct __attribute__((packed)) {
    uint32_t mip;
} RiscvCSRmip_t;

#endif
