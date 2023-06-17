/*

Copyright 2023 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeCSR_H_
#define RiscvEmulatorTypeCSR_H_

#include "RiscvEmulatorTypeCSRMachineInformationRegister.h"
#include "RiscvEmulatorTypeCSRMachineMemoryProtection.h"
#include "RiscvEmulatorTypeCSRMachineNonMaskableInterruptHandling.h"
#include "RiscvEmulatorTypeCSRMachineTrapHandling.h"
#include "RiscvEmulatorTypeCSRMachineTrapSetup.h"
#include "RiscvEmulatorTypeCSRSupervisorProtectionAndTranslation.h"

/**
 * Collection of control and status registers.
 */
typedef struct __attribute__((packed))
{
    RiscvCSRmhartid_t mhartid;

    RiscvCSRmstatus_t mstatus;
    RiscvCSRmstatush_t mstatush;
    RiscvCSRmisa_u misa;
    RiscvCSRmedeleg_t medeleg;
    RiscvCSRmideleg_t mideleg;
    RiscvCSRmie_u mie;
    RiscvCSRmtvec_t mtvec;

    RiscvCSRmepc_t mepc;

    RiscvCSRpmpcfg0_t pmpcfg0;
    RiscvCSRpmpaddr0_t pmpaddr0;

    RiscvCSRmnstatus_t mnstatus;

    RiscvCSRsatp_t satp;
} RiscvCSR_t;

#endif
