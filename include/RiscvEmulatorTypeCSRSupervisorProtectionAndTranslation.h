/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeCSRSupervisorProtectionAndTranslation_H_
#define RiscvEmulatorTypeCSRSupervisorProtectionAndTranslation_H_

#include <stdint.h>

/**
 * Supervisor Address Translation and Protection (satp) Register.
 */
typedef struct __attribute__((packed)) {
    /**
     * physical page number.
     */
    uint32_t ppn : 22;

    /**
     * address space identifier.
     */
    uint16_t asid : 9;

    /**
     * current address-translation scheme.
     */
    uint8_t mode : 1;

} RiscvCSRsatp_t;

#endif
