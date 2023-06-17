/*

Copyright 2023 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeCSRMachineNonMaskableInterruptHandling_H_
#define RiscvEmulatorTypeCSRMachineNonMaskableInterruptHandling_H_

#include <stdint.h>

/**
 * Resumable NMI status.
 */
typedef struct __attribute__((packed))
{
    uint8_t : 3;
    uint8_t nmie : 1;
    uint8_t : 3;
    uint8_t mnpv : 1;
    uint8_t : 3;
    uint8_t mnpp : 2;
    uint32_t : 19;
} RiscvCSRmnstatus_t;

#endif
