/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorDebug_H_
#define RiscvEmulatorDebug_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

/**
 * Debug function for returning the symbolic name of a register.
 */
static inline const char *RiscvEmulatorGetRegisterSymbolicName(const uint8_t r) {
    switch (r) {
        case 0:
            return "Zero";
        case 1:
            return "ra";
        case 2:
            return "sp";
        case 3:
            return "gp";
        case 4:
            return "tp";
        case 5:
            return "t0";
        case 6:
            return "t1";
        case 7:
            return "t2";
        case 8:
            return "s0/fp";
        case 9:
            return "s1";
        case 10:
            return "a0";
        case 11:
            return "a1";
        case 12:
            return "a2";
        case 13:
            return "a3";
        case 14:
            return "a4";
        case 15:
            return "a5";
        case 16:
            return "a6";
        case 17:
            return "a7";
        case 18:
            return "s2";
        case 19:
            return "s3";
        case 20:
            return "s4";
        case 21:
            return "s5";
        case 22:
            return "s6";
        case 23:
            return "s7";
        case 24:
            return "s8";
        case 25:
            return "s9";
        case 26:
            return "s10";
        case 27:
            return "s11";
        case 28:
            return "t3";
        case 29:
            return "t4";
        case 30:
            return "t5";
        case 31:
            return "t6";
        default:
            return "unknown";
    }
}

#if (RVE_E_ZICSR == 1)
#include "RiscvEmulatorTypeCSR.h"

/**
 * Debug function for returning the name of a CSR.
 */
static inline const char *RiscvEmulatorGetCSRName(const uint16_t csr) {
    const char *name;
    switch (csr) {
        // Machine Information Registers
        case RVE_CSR_MVENDORID:
            name = "mvendorid";
            break;
        case RVE_CSR_MARCHID:
            name = "marchid";
            break;
        case RVE_CSR_MIMPID:
            name = "mimpid";
            break;
        case RVE_CSR_MHARTID:
            name = "mhartid";
            break;
        case RVE_CSR_MCONFIGPTR:
            name = "mconfigptr";
            break;

        // Machine Trap Setup
        case RVE_CSR_MSTATUS:
            name = "mstatus";
            break;
        case RVE_CSR_MISA:
            name = "misa";
            break;
        case RVE_CSR_MEDELEG:
            name = "medeleg";
            break;
        case RVE_CSR_MIDELEG:
            name = "mideleg";
            break;
        case RVE_CSR_MIE:
            name = "mie";
            break;
        case RVE_CSR_MTVEC:
            name = "mtvec";
            break;
        case RVE_CSR_MCOUNTEREN:
            name = "mcounteren";
            break;
        case RVE_CSR_MENVCFG:
            name = "menvcfg";
            break;
        case RVE_CSR_MSTATUSH:
            name = "mstatush";
            break;
        case RVE_CSR_MEDELEGH:
            name = "medelegh";
            break;
        case RVE_CSR_MIDELEGH:
            name = "midelegh";
            break;
        case RVE_CSR_MIEH:
            name = "mieh";
            break;
        case RVE_CSR_MVIENH:
            name = "mvienh";
            break;
        case RVE_CSR_MVIPH:
            name = "mviph";
            break;
        case RVE_CSR_MCOUNTINHIBIT:
            name = "mcountinhibit";
            break;

        // Machine Trap Handling
        case RVE_CSR_MSCRATCH:
            name = "mscratch";
            break;
        case RVE_CSR_MEPC:
            name = "mepc";
            break;
        case RVE_CSR_MCAUSE:
            name = "mcause";
            break;
        case RVE_CSR_MTVAL:
            name = "mtval";
            break;
        case RVE_CSR_MIP:
            name = "mip";
            break;

        // Platform interrupt set and clear CSRs
        case RVE_CSR_MIPSET:
            name = "mipset";
            break;
        case RVE_CSR_MIPCLR:
            name = "mipclr";
            break;

        // Machine Memory Protection
        case RVE_CSR_PMPCFG0:
            name = "pmpcfg0";
            break;
        case RVE_CSR_PMPADDR0:
            name = "pmpaddr0";
            break;

        // Machine Non-Maskable Interrupt Handling
        case RVE_CSR_MNSTATUS:
            name = "mnstatus";
            break;

        // Machine Security Configuration
        case RVE_CSR_MSECCFG:
            name = "mseccfg";
            break;

            // Supervisor Trap Setup
#if (RVE_E_SV32 == 1)
        case RVE_CSR_SSTATUS:
            name = "sstatus";
            break;
        case RVE_CSR_SIE:
            name = "sie";
            break;
        case RVE_CSR_STVEC:
            name = "stvec";
            break;
        case RVE_CSR_SCOUNTEREN:
            name = "scounteren";
            break;
        case RVE_CSR_SENVCFG:
            name = "senvcfg";
            break;

        // Supervisor Trap Handling
        case RVE_CSR_SSCRATCH:
            name = "sscratch";
            break;
        case RVE_CSR_SEPC:
            name = "sepc";
            break;
        case RVE_CSR_SCAUSE:
            name = "scause";
            break;
        case RVE_CSR_STVAL:
            name = "stval";
            break;
        case RVE_CSR_SIP:
            name = "sip";
            break;
#endif

        // Supervisor Protection and Translation
        case RVE_CSR_SATP:
            name = "satp";
            break;

        // Counters
        case RVE_CSR_MCYCLE:
            name = "mcycle";
            break;
        case RVE_CSR_MINSTRET:
            name = "minstret";
            break;
        case RVE_CSR_MHPMCOUNTER3:
            name = "mhpmcounter3";
            break;
        case RVE_CSR_MCYCLEH:
            name = "mcycleh";
            break;
        case RVE_CSR_MINSTRETH:
            name = "minstreth";
            break;
        case RVE_CSR_MHPMCOUNTER3H:
            name = "mhpmcounter3h";
            break;
        case RVE_CSR_CYCLE:
            name = "cycle";
            break;
        case RVE_CSR_TIME:
            name = "time";
            break;
        case RVE_CSR_INSTRET:
            name = "instret";
            break;
        case RVE_CSR_CYCLEH:
            name = "cycleh";
            break;
        case RVE_CSR_TIMEH:
            name = "timeh";
            break;
        case RVE_CSR_INSTRETH:
            name = "instreth";
            break;

        default:
            name = "unknown";
            break;
    }

    return name;
}

/**
 * Debug function to get exception description.
 */
static inline const char *RiscvEmulatorGetMcauseException(const uint8_t interrupt, const uint32_t exceptioncode) {
    if (interrupt) {
        switch (exceptioncode) {
            case 1:
                return "Supervisor software interrupt";
            case 3:
                return "Machine software interrupt";
            case 5:
                return "Supervisor timer interrupt";
            case 7:
                return "Machine timer interrupt";
            case 9:
                return "Supervisor external interrupt";
            case 11:
                return "Machine external interrupt";
            case 13:
                return "Counter-overflow interrupt";
            default:
                return "Unknown Interrupt";
        }
    } else {
        switch (exceptioncode) {
            case 0:
                return "Instruction Address Misaligned";
            case 1:
                return "Instruction Access Fault";
            case 2:
                return "Illegal Instruction";
            case 3:
                return "Breakpoint";
            case 4:
                return "Load Address Misaligned";
            case 5:
                return "Load Access Fault";
            case 6:
                return "Store/AMO Address Misaligned";
            case 7:
                return "Store/AMO Access Fault";
            case 8:
                return "Environment Call from U-mode";
            case 9:
                return "Environment Call from S-mode";
            case 11:
                return "Environment Call from M-mode";
            case 12:
                return "Instruction page fault";
            case 13:
                return "Load page fault";
            case 15:
                return "Store/AMO page fault";
            case 16:
                return "Double trap";
            case 18:
                return "Software check";
            case 19:
                return "Hardware error";
            default:
                return "Unknown Exception";
        }
    }
}

#endif

#endif
