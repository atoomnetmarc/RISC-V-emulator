/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeCSRMachineInformationRegister_H_
#define RiscvEmulatorTypeCSRMachineInformationRegister_H_

#include <stdint.h>

/**
 * Hart ID Register.
 */
typedef struct __attribute__((packed)) {
    uint32_t hartid : 32;
} RiscvCSRmhartid_t;

#endif
