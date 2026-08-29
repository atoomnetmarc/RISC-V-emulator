/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeCSR_H_
#define RiscvEmulatorTypeCSR_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZICSR == 1)

#include "RiscvEmulatorTypeCSRMachineInformationRegister.h"
#include "RiscvEmulatorTypeCSRMachineMemoryProtection.h"
#include "RiscvEmulatorTypeCSRMachineNonMaskableInterruptHandling.h"
#include "RiscvEmulatorTypeCSRMachineTrapHandling.h"
#include "RiscvEmulatorTypeCSRMachineTrapSetup.h"
#include "RiscvEmulatorTypeCSRSupervisorProtectionAndTranslation.h"

/**
 * Collection of control and status registers.
 */
typedef struct __attribute__((packed)) {
    // Machine Information Registers
    RiscvCSRmhartid_t mhartid;

    // Machine Trap Setup
    RiscvCSRmstatus_t mstatus;
    RiscvCSRmisa_u misa;
    RiscvCSRmedeleg_t medeleg;
    RiscvCSRmideleg_t mideleg;
    RiscvCSRmie_u mie;
    RiscvCSRmtvec_t mtvec;
    RiscvCSRmstatush_t mstatush;

    // Machine Trap Handling
    RiscvCSRmscratch_t mscratch;
    uint32_t mepc; // Machine exception program counter.
    RiscvCSRmcause_t mcause;
    uint32_t mtval; // Machine bad address or instruction.
    RiscvCSRmip_t mip;

    // Machine Memory Protection
    RiscvCSRpmpcfg0_t pmpcfg0;
    RiscvCSRpmpaddr0_t pmpaddr0;

    // Machine Non-Maskable Interrupt Handling
    RiscvCSRmnstatus_t mnstatus;

    // Supervisor Protection and Translation
    RiscvCSRsatp_t satp;

    // Machine Counter Setup
#if (RVE_E_ZICNTR == 1)
    uint64_t cycle;    // Machine cycle counter.
    uint64_t time;     // Machine real-time counter.
    uint64_t instret;  // Machine instructions-retired counter.
#endif
} RiscvCSR_t;

#endif

#endif