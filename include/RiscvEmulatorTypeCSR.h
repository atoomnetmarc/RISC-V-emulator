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
 * mepc and sepc: the low bit is always zero. With IALIGN=32 the two low bits
 * are always zero.
 */
#if (RVE_E_C == 1)
#define RVE_CSR_XEPC_WRITE_MASK (~(uint32_t)0x1)
#else
#define RVE_CSR_XEPC_WRITE_MASK (~(uint32_t)0x3)
#endif
#define RVE_CSR_MEPC_WRITE_MASK RVE_CSR_XEPC_WRITE_MASK
#define RVE_CSR_SEPC_WRITE_MASK RVE_CSR_XEPC_WRITE_MASK

/**
 * mtvec: mode 0 (direct) and 1 (vectored) are legal, bit 1 is reserved.
 */
#define RVE_CSR_MTVEC_WRITE_MASK (~(uint32_t)0x2)

/**
 * mie: the S-mode and M-mode software, timer and external interrupt bits
 * are implemented (bits 1, 3, 5, 7, 9 and 11). mip.SSIP is writable;
 * mip.STIP and mip.SEIP are set by the devices.
 */
#if (RVE_E_SV32 == 1)
#define RVE_CSR_MIE_WRITE_MASK 0xAAAu
#define RVE_CSR_MIP_WRITE_MASK 0x2u
#else
#define RVE_CSR_MIE_WRITE_MASK 0x888u
#define RVE_CSR_MIP_WRITE_MASK 0x0u
#endif

/**
 * mcounteren: only the CY, TM and IR bits are implemented.
 */
#define RVE_CSR_MCOUNTEREN_WRITE_MASK 0x7u

/**
 * mstatus: MIE, MPIE, MPP, SIE, SPIE, SPP, MPRV, SUM, MXR, TVM, TW and TSR
 * are writable. MPP is WARL and holds U-mode (0b00), S-mode (0b01) or M-mode
 * (0b11); the reserved value 0b10 maps to M-mode.
 */
#if (RVE_E_SV32 == 1)
#define RVE_CSR_MSTATUS_WRITE_MASK 0x7E19AAu
#else
#define RVE_CSR_MSTATUS_WRITE_MASK 0x1888u
#endif
#define RVE_CSR_MSTATUS_MPP_MASK     0x1800u
#define RVE_CSR_MSTATUS_MPP_RESERVED 0x1000u
#define RVE_CSR_MSTATUS_MPP_MMODE    0x1800u

/**
 * sstatus: the S-mode view of mstatus. FS, XS, UBE and SD read as zero.
 */
#define RVE_CSR_SSTATUS_WRITE_MASK 0xC0122u
#define RVE_CSR_SSTATUS_READ_MASK  0xC0122u

/**
 * stvec: mode 0 (direct) and 1 (vectored) are legal, bit 1 is reserved.
 * scounteren: only the CY, TM and IR bits are implemented.
 * senvcfg: FIOM is writable because satp.MODE is not read-only zero. The
 * cache-block, pointer-masking, Zicfilp and Zicfiss fields are read-only zero
 * because those extensions are not implemented.
 */
#define RVE_CSR_STVEC_WRITE_MASK      (~(uint32_t)0x2)
#define RVE_CSR_SCOUNTEREN_WRITE_MASK 0x7u
#define RVE_CSR_SENVCFG_WRITE_MASK    0x1u
#define RVE_CSR_SENVCFG_READ_MASK     0x1u

/**
 * medeleg, mideleg, mstatush, satp, pmpcfg0 and pmpaddr0: without S-mode,
 * the hypervisor extension or PMP enforcement, these are read-only zero.
 */
#if (RVE_E_SV32 == 1)
#define RVE_CSR_MEDELEG_WRITE_MASK 0xB1FFu
#define RVE_CSR_MIDELEG_WRITE_MASK 0x222u
#define RVE_CSR_SATP_WRITE_MASK    0xFFFFFFFFu
#else
#define RVE_CSR_MEDELEG_WRITE_MASK 0x0u
#define RVE_CSR_MIDELEG_WRITE_MASK 0x0u
#define RVE_CSR_SATP_WRITE_MASK    0x0u
#endif
#define RVE_CSR_MSTATUSH_WRITE_MASK 0x0u
#define RVE_CSR_PMPCFG0_WRITE_MASK  0x0u
#define RVE_CSR_PMPADDR0_WRITE_MASK 0x0u

/**
 * misa: the extension bits of implemented extensions are writable. The I and
 * U bits are always set. MXL (bits 31:30) is read-only.
 */
#define RVE_CSR_MISA_I (UINT32_C(1) << 8)
#define RVE_CSR_MISA_U (UINT32_C(1) << 20)

#if (RVE_E_SV32 == 1)
#define RVE_CSR_MISA_S (UINT32_C(1) << 18)
#else
#define RVE_CSR_MISA_S UINT32_C(0)
#endif

#if (RVE_E_M == 1)
#define RVE_CSR_MISA_M (1u << 12)
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

#define RVE_CSR_MISA_WRITE_MASK (RVE_CSR_MISA_I | RVE_CSR_MISA_S | RVE_CSR_MISA_M | RVE_CSR_MISA_A | RVE_CSR_MISA_C | RVE_CSR_MISA_B)
#define RVE_CSR_MISA_MXL_RV32   (UINT32_C(1) << 30)

/**
 * misa value at reset: the maximum set of implemented extensions.
 */
#define RVE_CSR_MISA_VALUE (RVE_CSR_MISA_I | RVE_CSR_MISA_S | RVE_CSR_MISA_U | RVE_CSR_MISA_M | RVE_CSR_MISA_A | RVE_CSR_MISA_C | RVE_CSR_MISA_B)

/**
 * WARL read masks: bits that are masked on reads.
 *
 * mepc and sepc: masking bit 1 on reads (IALIGN=32) is optional per the spec,
 * and the ACT reference model expects bit 1 to be readable, so nothing is
 * masked.
 */
#define RVE_CSR_MEPC_READ_MASK 0x0u
#define RVE_CSR_SEPC_READ_MASK 0x0u

/**
 * CSR access flags, returned by RiscvEmulatorGetCSRAddress().
 */
#define RVE_CSR_F_READONLY (1u << 0) // Writes raise an illegal-instruction trap.
#define RVE_CSR_F_ZERO     (1u << 1) // Reads as zero, writes are ignored.

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

    // Supervisor Trap Setup
    RiscvCSRmtvec_t stvec;
    uint32_t scounteren; // Supervisor counter enable.
    uint32_t senvcfg;    // Supervisor environment configuration.

    // Supervisor Trap Handling
    uint32_t sscratch; // Supervisor scratch register.
    uint32_t sepc;     // Supervisor exception program counter.
    RiscvCSRmcause_t scause;
    uint32_t stval; // Supervisor bad address or instruction.

    // Supervisor Protection and Translation
    RiscvCSRsatp_t satp;

    // Shared storage for read-only-zero CSRs.
    uint32_t zeroreg;

    // Machine Counter Setup
    uint32_t mcounteren; // Machine counter enable.

#if (RVE_E_ZICNTR == 1)
    uint64_t cycle;   // Machine cycle counter.
    uint64_t time;    // Machine real-time counter.
    uint64_t instret; // Machine instructions-retired counter.
#endif
} RiscvCSR_t;

#endif

#endif