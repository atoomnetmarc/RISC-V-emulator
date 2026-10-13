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
 * WARL write masks.
 *
 * mepc: the low bit is always zero.
 */
#define RVE_CSR_MEPC_WRITE_MASK (~0x1ul)

/**
 * mtvec: mode 0 (direct) and 1 (vectored) are legal, bit 1 is reserved.
 */
#define RVE_CSR_MTVEC_WRITE_MASK (~0x2ul)

/**
 * mie and mip: only the M-mode software, timer and external interrupt bits
 * are implemented (bits 3, 7 and 11).
 */
#define RVE_CSR_MIE_WRITE_MASK 0x888u
#define RVE_CSR_MIP_WRITE_MASK 0x888u

/**
 * mstatus: only MIE, MPIE and MPP are writable without S-mode, U-mode,
 * the F extension or the V extension. MPP is forced to M-mode (0b11).
 */
#define RVE_CSR_MSTATUS_WRITE_MASK 0x1888u
#define RVE_CSR_MSTATUS_MPP_MMODE  0x1800u

/**
 * medeleg, mideleg, mstatush, satp, pmpcfg0 and pmpaddr0: without S-mode,
 * the hypervisor extension or PMP enforcement, these are read-only zero.
 */
#define RVE_CSR_MEDELEG_WRITE_MASK  0x0u
#define RVE_CSR_MIDELEG_WRITE_MASK  0x0u
#define RVE_CSR_MSTATUSH_WRITE_MASK 0x0u
#define RVE_CSR_SATP_WRITE_MASK     0x0u
#define RVE_CSR_PMPCFG0_WRITE_MASK  0x0u
#define RVE_CSR_PMPADDR0_WRITE_MASK 0x0u

/**
 * misa: the extension bits of implemented extensions are writable. The I bit
 * (bit 8) is always set. MXL (bits 31:30) is read-only.
 */
#define RVE_CSR_MISA_I (1u << 8)

#if (RVE_E_M == 1)
#define RVE_CSR_MISA_M (1u << 13)
#else
#define RVE_CSR_MISA_M 0u
#endif

#if (RVE_E_A == 1)
#define RVE_CSR_MISA_A (1u << 0)
#else
#define RVE_CSR_MISA_A 0u
#endif

#if (RVE_E_C == 1)
#define RVE_CSR_MISA_C (1u << 2)
#else
#define RVE_CSR_MISA_C 0u
#endif

#if (RVE_E_B == 1)
#define RVE_CSR_MISA_B (1u << 1)
#else
#define RVE_CSR_MISA_B 0u
#endif

#define RVE_CSR_MISA_WRITE_MASK (RVE_CSR_MISA_I | RVE_CSR_MISA_M | RVE_CSR_MISA_A | RVE_CSR_MISA_C | RVE_CSR_MISA_B)
#define RVE_CSR_MISA_MXL_RV32   (1ul << 30)

/**
 * WARL read masks: bits that are masked on reads.
 *
 * mepc: masking bit 1 on reads (IALIGN=32) is optional per the spec, and the
 * ACT reference model expects bit 1 to be readable, so nothing is masked.
 */
#define RVE_CSR_MEPC_READ_MASK 0x0u

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
    uint64_t cycle;   // Machine cycle counter.
    uint64_t time;    // Machine real-time counter.
    uint64_t instret; // Machine instructions-retired counter.
#endif
} RiscvCSR_t;

#endif

#endif