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
 * Machine exception program counter.
 */
typedef struct __attribute__((packed)) {
    uint32_t mepc;
} RiscvCSRmepc_t;

#endif
