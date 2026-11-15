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

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorTypeCSRMachineInformationRegister.h"
#include "RiscvEmulatorTypeCSRMachineMemoryProtection.h"
#include "RiscvEmulatorTypeCSRMachineNonMaskableInterruptHandling.h"
#include "RiscvEmulatorTypeCSRMachineTrapHandling.h"
#include "RiscvEmulatorTypeCSRMachineTrapSetup.h"
#include "RiscvEmulatorTypeCSRSupervisorProtectionAndTranslation.h"

/**
 * CSR addresses.
 */
#define RVE_CSR_MVENDORID      0xF11
#define RVE_CSR_MARCHID        0xF12
#define RVE_CSR_MIMPID         0xF13
#define RVE_CSR_MHARTID        0xF14
#define RVE_CSR_MCONFIGPTR     0xF15
#define RVE_CSR_MSTATUS        0x300
#define RVE_CSR_MISA           0x301
#define RVE_CSR_MEDELEG        0x302
#define RVE_CSR_MIDELEG        0x303
#define RVE_CSR_MIE            0x304
#define RVE_CSR_MTVEC          0x305
#define RVE_CSR_MCOUNTEREN     0x306
#define RVE_CSR_MENVCFG        0x30A
#define RVE_CSR_MSTATUSH       0x310
#define RVE_CSR_MEDELEGH       0x312
#define RVE_CSR_MIDELEGH       0x313
#define RVE_CSR_MIEH           0x314
#define RVE_CSR_MVIENH         0x318
#define RVE_CSR_MVIPH          0x319
#define RVE_CSR_MENVCFGH       0x31A
#define RVE_CSR_MCOUNTINHIBIT  0x320
#define RVE_CSR_MSCRATCH       0x340
#define RVE_CSR_MEPC           0x341
#define RVE_CSR_MCAUSE         0x342
#define RVE_CSR_MTVAL          0x343
#define RVE_CSR_MIP            0x344
#define RVE_CSR_MNSTATUS       0x744
#define RVE_CSR_MSECCFG        0x747
#define RVE_CSR_PMPCFG0        0x3A0
#define RVE_CSR_PMPADDR0       0x3B0
#define RVE_CSR_MIPSET         0x8C0
#define RVE_CSR_MIPCLR         0x8C1
#define RVE_CSR_SSTATUS        0x100
#define RVE_CSR_SIE            0x104
#define RVE_CSR_STVEC          0x105
#define RVE_CSR_SCOUNTEREN     0x106
#define RVE_CSR_SENVCFG        0x10A
#define RVE_CSR_SSCRATCH       0x140
#define RVE_CSR_SEPC           0x141
#define RVE_CSR_SCAUSE         0x142
#define RVE_CSR_STVAL          0x143
#define RVE_CSR_SIP            0x144
#define RVE_CSR_SATP           0x180
#define RVE_CSR_MCYCLE         0xB00
#define RVE_CSR_MINSTRET       0xB02
#define RVE_CSR_MHPMCOUNTER3   0xB03
#define RVE_CSR_MCYCLEH        0xB80
#define RVE_CSR_MINSTRETH      0xB82
#define RVE_CSR_MHPMCOUNTER3H  0xB83
#define RVE_CSR_CYCLE          0xC00
#define RVE_CSR_TIME           0xC01
#define RVE_CSR_INSTRET        0xC02
#define RVE_CSR_CYCLEH         0xC80
#define RVE_CSR_TIMEH          0xC81
#define RVE_CSR_INSTRETH       0xC82
#define RVE_CSR_MHPMEVENT3     0x323
#define RVE_CSR_MHPMEVENT31    0x33F
#define RVE_CSR_MHPMCOUNTER31  0xB1F
#define RVE_CSR_MHPMCOUNTERH3  0xB83
#define RVE_CSR_MHPMCOUNTERH31 0xB9F
#define RVE_CSR_PMPCFG1        0x3A1
#define RVE_CSR_PMPCFG3        0x3A3
#define RVE_CSR_PMPADDR1       0x3B1
#define RVE_CSR_PMPADDR15      0x3BF
#define RVE_CSR_MSTATEEN0      0x30C
#define RVE_CSR_MSTATEEN3      0x30F

/**
 * CSR address privilege encoding: csr[9:8] is the lowest privilege level.
 */
#define RVE_CSR_ADDRESS_PRIVILEGE_SHIFT 8
#define RVE_CSR_ADDRESS_PRIVILEGE_MASK  (UINT32_C(3) << RVE_CSR_ADDRESS_PRIVILEGE_SHIFT)

/**
 * WARL write masks.
 *
 * mepc and sepc: the low bit is always zero. With IALIGN=32 the two low bits
 * are always zero.
 */
#define RVE_CSR_XEPC_WRITE_MASK (~(uint32_t)RVE_IALIGN_MASK)
#define RVE_CSR_MEPC_WRITE_MASK RVE_CSR_XEPC_WRITE_MASK
#define RVE_CSR_SEPC_WRITE_MASK RVE_CSR_XEPC_WRITE_MASK

/**
 * mtvec and stvec: mode 0 (direct) and 1 (vectored) are legal, bit 1 is
 * reserved. BASE holds address[31:2].
 */
#define RVE_CSR_MTVEC_MODE_MASK     (UINT32_C(3) << 0)
#define RVE_CSR_MTVEC_MODE_RESERVED (UINT32_C(1) << 1)
#define RVE_CSR_MTVEC_WRITE_MASK    (~RVE_CSR_MTVEC_MODE_RESERVED)
#define RVE_CSR_MTVEC_BASE_SHIFT    2
#define RVE_CSR_STVEC_MODE_MASK     RVE_CSR_MTVEC_MODE_MASK
#define RVE_CSR_STVEC_MODE_RESERVED RVE_CSR_MTVEC_MODE_RESERVED
#define RVE_CSR_STVEC_WRITE_MASK    (~RVE_CSR_STVEC_MODE_RESERVED)
#define RVE_CSR_STVEC_BASE_SHIFT    2

/**
 * mie and mip: the S-mode and M-mode software, timer and external interrupt
 * bits are implemented (bits 1, 3, 5, 7, 9 and 11).
 */
#define RVE_CSR_MIE_SSI  (UINT32_C(1) << 1)
#define RVE_CSR_MIE_MSI  (UINT32_C(1) << 3)
#define RVE_CSR_MIE_STI  (UINT32_C(1) << 5)
#define RVE_CSR_MIE_MTI  (UINT32_C(1) << 7)
#define RVE_CSR_MIE_SEI  (UINT32_C(1) << 9)
#define RVE_CSR_MIE_MEI  (UINT32_C(1) << 11)
#define RVE_CSR_MIP_SSIP RVE_CSR_MIE_SSI
#define RVE_CSR_MIP_STIP RVE_CSR_MIE_STI
#define RVE_CSR_MIP_SEIP RVE_CSR_MIE_SEI

#if (RVE_E_SV32 == 1)
#define RVE_CSR_MIE_WRITE_MASK (RVE_CSR_MIE_SSI | RVE_CSR_MIE_MSI | RVE_CSR_MIE_STI | RVE_CSR_MIE_MTI | RVE_CSR_MIE_SEI | RVE_CSR_MIE_MEI)
#define RVE_CSR_MIP_WRITE_MASK (RVE_CSR_MIP_SSIP | RVE_CSR_MIP_STIP | RVE_CSR_MIP_SEIP)
#else
#define RVE_CSR_MIE_WRITE_MASK (RVE_CSR_MIE_MSI | RVE_CSR_MIE_MTI | RVE_CSR_MIE_MEI)
#define RVE_CSR_MIP_WRITE_MASK UINT32_C(0)
#endif

/**
 * mcounteren and scounteren: only the CY, TM and IR bits are implemented.
 */
#define RVE_CSR_MCOUNTEREN_CY         (UINT32_C(1) << 0)
#define RVE_CSR_MCOUNTEREN_TM         (UINT32_C(1) << 1)
#define RVE_CSR_MCOUNTEREN_IR         (UINT32_C(1) << 2)
#define RVE_CSR_MCOUNTEREN_WRITE_MASK (RVE_CSR_MCOUNTEREN_CY | RVE_CSR_MCOUNTEREN_TM | RVE_CSR_MCOUNTEREN_IR)
#define RVE_CSR_SCOUNTEREN_WRITE_MASK RVE_CSR_MCOUNTEREN_WRITE_MASK

/**
 * mstatus: MIE, MPIE, MPP, SIE, SPIE, SPP, MPRV, SUM, MXR, TVM, TW and TSR
 * are writable. MPP is WARL and holds U-mode (0b00), S-mode (0b01) or M-mode
 * (0b11); the reserved value 0b10 maps to M-mode.
 */
#define RVE_CSR_MSTATUS_SIE_SHIFT  1
#define RVE_CSR_MSTATUS_MIE_SHIFT  3
#define RVE_CSR_MSTATUS_SPIE_SHIFT 5
#define RVE_CSR_MSTATUS_UBE_SHIFT  6
#define RVE_CSR_MSTATUS_MPIE_SHIFT 7
#define RVE_CSR_MSTATUS_SPP_SHIFT  8
#define RVE_CSR_MSTATUS_MPP_SHIFT  11
#define RVE_CSR_MSTATUS_MPRV_SHIFT 17
#define RVE_CSR_MSTATUS_SUM_SHIFT  18
#define RVE_CSR_MSTATUS_MXR_SHIFT  19
#define RVE_CSR_MSTATUS_TVM_SHIFT  20
#define RVE_CSR_MSTATUS_TW_SHIFT   21
#define RVE_CSR_MSTATUS_TSR_SHIFT  22
#define RVE_CSR_MSTATUS_SD_SHIFT   31

#define RVE_CSR_MSTATUS_SIE          (UINT32_C(1) << RVE_CSR_MSTATUS_SIE_SHIFT)
#define RVE_CSR_MSTATUS_MIE          (UINT32_C(1) << RVE_CSR_MSTATUS_MIE_SHIFT)
#define RVE_CSR_MSTATUS_SPIE         (UINT32_C(1) << RVE_CSR_MSTATUS_SPIE_SHIFT)
#define RVE_CSR_MSTATUS_UBE          (UINT32_C(1) << RVE_CSR_MSTATUS_UBE_SHIFT)
#define RVE_CSR_MSTATUS_MPIE         (UINT32_C(1) << RVE_CSR_MSTATUS_MPIE_SHIFT)
#define RVE_CSR_MSTATUS_SPP          (UINT32_C(1) << RVE_CSR_MSTATUS_SPP_SHIFT)
#define RVE_CSR_MSTATUS_MPP_MASK     (UINT32_C(3) << RVE_CSR_MSTATUS_MPP_SHIFT)
#define RVE_CSR_MSTATUS_MPP_RESERVED (UINT32_C(1) << (RVE_CSR_MSTATUS_MPP_SHIFT + 1))
#define RVE_CSR_MSTATUS_MPP_MMODE    (UINT32_C(3) << RVE_CSR_MSTATUS_MPP_SHIFT)
#define RVE_CSR_MSTATUS_MPRV         (UINT32_C(1) << RVE_CSR_MSTATUS_MPRV_SHIFT)
#define RVE_CSR_MSTATUS_SUM          (UINT32_C(1) << RVE_CSR_MSTATUS_SUM_SHIFT)
#define RVE_CSR_MSTATUS_MXR          (UINT32_C(1) << RVE_CSR_MSTATUS_MXR_SHIFT)
#define RVE_CSR_MSTATUS_TVM          (UINT32_C(1) << RVE_CSR_MSTATUS_TVM_SHIFT)
#define RVE_CSR_MSTATUS_TW           (UINT32_C(1) << RVE_CSR_MSTATUS_TW_SHIFT)
#define RVE_CSR_MSTATUS_TSR          (UINT32_C(1) << RVE_CSR_MSTATUS_TSR_SHIFT)
#define RVE_CSR_MSTATUS_SD           (UINT32_C(1) << RVE_CSR_MSTATUS_SD_SHIFT)

#if (RVE_E_SV32 == 1)
#define RVE_CSR_MSTATUS_WRITE_MASK (RVE_CSR_MSTATUS_SIE | RVE_CSR_MSTATUS_MIE | RVE_CSR_MSTATUS_SPIE | RVE_CSR_MSTATUS_MPIE | RVE_CSR_MSTATUS_SPP | RVE_CSR_MSTATUS_MPP_MASK | RVE_CSR_MSTATUS_MPRV | RVE_CSR_MSTATUS_SUM | RVE_CSR_MSTATUS_MXR | RVE_CSR_MSTATUS_TVM | RVE_CSR_MSTATUS_TW | RVE_CSR_MSTATUS_TSR)
#else
#define RVE_CSR_MSTATUS_WRITE_MASK (RVE_CSR_MSTATUS_MIE | RVE_CSR_MSTATUS_MPIE | RVE_CSR_MSTATUS_MPP_MASK)
#endif

/**
 * sstatus: the S-mode view of mstatus. FS, XS, UBE and SD read as zero.
 */
#define RVE_CSR_SSTATUS_WRITE_MASK (RVE_CSR_MSTATUS_SIE | RVE_CSR_MSTATUS_SPIE | RVE_CSR_MSTATUS_SPP | RVE_CSR_MSTATUS_SUM | RVE_CSR_MSTATUS_MXR)
#define RVE_CSR_SSTATUS_READ_MASK  RVE_CSR_SSTATUS_WRITE_MASK

/**
 * senvcfg and menvcfg: FIOM is writable because satp.MODE is not read-only
 * zero. The cache-block, pointer-masking, Zicfilp and Zicfiss fields are
 * read-only zero because those extensions are not implemented.
 */
#define RVE_CSR_SENVCFG_FIOM       (UINT32_C(1) << 0)
#define RVE_CSR_MENVCFG_FIOM       (UINT32_C(1) << 0)
#define RVE_CSR_SENVCFG_WRITE_MASK RVE_CSR_SENVCFG_FIOM
#define RVE_CSR_SENVCFG_READ_MASK  RVE_CSR_SENVCFG_FIOM
#define RVE_CSR_MENVCFG_WRITE_MASK RVE_CSR_MENVCFG_FIOM
#define RVE_CSR_MENVCFG_READ_MASK  RVE_CSR_MENVCFG_FIOM

/**
 * medeleg: the standard exceptions plus software check (18) and hardware
 * error (19) are delegatable. mideleg, mstatush, satp, pmpcfg0 and pmpaddr0:
 * without S-mode, the hypervisor extension or PMP enforcement, these are
 * read-only zero.
 */
#if (RVE_E_SV32 == 1)
#define RVE_CSR_MEDELEG_WRITE_MASK (                                        \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_INSTRUCTION_ADDRESS_MISALIGNED) | \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_INSTRUCTION_ACCESS_FAULT) |       \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_ILLEGAL_INSTRUCTION) |            \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_BREAKPOINT) |                     \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_LOAD_ADDRESS_MISALIGNED) |        \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_LOAD_ACCESS_FAULT) |              \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_STORE_ADDRESS_MISALIGNED) |       \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_STORE_ACCESS_FAULT) |             \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_ENVIRONMENT_CALL_FROM_UMODE) |    \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_INSTRUCTION_PAGE_FAULT) |         \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_LOAD_PAGE_FAULT) |                \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_STORE_PAGE_FAULT) |               \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_SOFTWARE_CHECK) |                 \
    (UINT32_C(1) << MCAUSE_EXCEPTION_CODE_HARDWARE_ERROR))
#define RVE_CSR_MIDELEG_WRITE_MASK (RVE_CSR_MIE_SSI | RVE_CSR_MIE_STI | RVE_CSR_MIE_SEI)
#define RVE_CSR_SATP_WRITE_MASK    UINT32_C(0xFFFFFFFF)
#else
#define RVE_CSR_MEDELEG_WRITE_MASK UINT32_C(0)
#define RVE_CSR_MIDELEG_WRITE_MASK UINT32_C(0)
#define RVE_CSR_SATP_WRITE_MASK    UINT32_C(0)
#endif
#define RVE_CSR_MSTATUSH_WRITE_MASK UINT32_C(0)
#define RVE_CSR_PMPCFG0_WRITE_MASK  UINT32_C(0)
#define RVE_CSR_PMPADDR0_WRITE_MASK UINT32_C(0)

/**
 * satp: MODE is bit 31.
 */
#define RVE_CSR_SATP_MODE_SHIFT 31
#define RVE_CSR_SATP_MODE_MASK  (UINT32_C(1) << RVE_CSR_SATP_MODE_SHIFT)

/**
 * misa: the extension bits of implemented extensions are writable. The I and
 * U bits are always set. MXL (bits 31:30) is read-only.
 */
#define RVE_CSR_MISA_MXL_SHIFT 30
#define RVE_CSR_MISA_MXL_MASK  (UINT32_C(3) << RVE_CSR_MISA_MXL_SHIFT)
#define RVE_CSR_MISA_I         (UINT32_C(1) << 8)
#define RVE_CSR_MISA_U         (UINT32_C(1) << 20)

#if (RVE_E_SV32 == 1)
#define RVE_CSR_MISA_S (UINT32_C(1) << 18)
#else
#define RVE_CSR_MISA_S UINT32_C(0)
#endif

#if (RVE_E_M == 1)
#define RVE_CSR_MISA_M (UINT32_C(1) << 12)
#else
#define RVE_CSR_MISA_M UINT32_C(0)
#endif

#if (RVE_E_A == 1)
#define RVE_CSR_MISA_A (UINT32_C(1) << 0)
#else
#define RVE_CSR_MISA_A UINT32_C(0)
#endif

#if (RVE_E_C == 1)
#define RVE_CSR_MISA_C (UINT32_C(1) << 2)
#else
#define RVE_CSR_MISA_C UINT32_C(0)
#endif

#if (RVE_E_B == 1)
#define RVE_CSR_MISA_B (UINT32_C(1) << 1)
#else
#define RVE_CSR_MISA_B UINT32_C(0)
#endif

#define RVE_CSR_MISA_WRITE_MASK (RVE_CSR_MISA_I | RVE_CSR_MISA_S | RVE_CSR_MISA_M | RVE_CSR_MISA_A | RVE_CSR_MISA_C | RVE_CSR_MISA_B)
#define RVE_CSR_MISA_MXL_RV32   (UINT32_C(1) << RVE_CSR_MISA_MXL_SHIFT)

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
#define RVE_CSR_MEPC_READ_MASK UINT32_C(0)
#define RVE_CSR_SEPC_READ_MASK UINT32_C(0)

/**
 * CSR access flags, returned by RiscvEmulatorGetCSRAddress().
 */
#define RVE_CSR_F_READONLY (UINT32_C(1) << 0) // Writes raise an illegal-instruction trap.
#define RVE_CSR_F_ZERO     (UINT32_C(1) << 1) // Reads as zero, writes are ignored.

/**
 * Collection of control and status registers.
 */
typedef struct __attribute__((packed)) {
    // Machine Information Registers
    RiscvCSRmhartid_t mhartid;
    uint32_t marchid; // Machine architecture ID.

    // Machine Trap Setup
    RiscvCSRmstatus_t mstatus;
    RiscvCSRmisa_u misa;
    RiscvCSRmedeleg_t medeleg;
    RiscvCSRmideleg_t mideleg;
    RiscvCSRmie_u mie;
    RiscvCSRmtvec_t mtvec;
    RiscvCSRmstatush_t mstatush;
    uint32_t menvcfg; // Machine environment configuration.

    // Machine Trap Handling
    RiscvCSRmscratch_t mscratch;
    uint32_t mepc; // Machine exception program counter.
    RiscvCSRmcause_t mcause;
    uint32_t mtval; // Machine bad address or instruction.
    RiscvCSRmip_t mip;
    uint32_t mip_platform;          // Platform interrupt signals, ORed into mip reads.
    uint32_t mip_platform_software; // Software-set platform bits, ORed into mip reads.

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
    uint64_t cycle;        // Machine cycle counter.
    uint64_t time;         // Machine real-time counter.
    uint64_t instret;      // Machine instructions-retired counter.
    uint64_t mhpmcounter3; // Machine performance-monitoring counter 3.
#endif
} RiscvCSR_t;

#endif

#endif
