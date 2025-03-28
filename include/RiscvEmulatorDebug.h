/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorDebug_H_
#define RiscvEmulatorDebug_H_

#include <stdint.h>

/**
 * Debug function for returning the symbolic name of a register.
 */
const char *RiscvEmulatorGetRegisterSymbolicName(const uint8_t r) {
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
/**
 * Debug function for returning the name of a CSR.
 */
const char *RiscvEmulatorGetCSRName(const uint16_t csr) {
    const char *name;
    switch (csr) {
        // Machine Information Registers
        case 0xF14:
            name = "mhartid";
            break;

        // Machine Trap Setup
        case 0x300:
            name = "mstatus";
            break;
        case 0x301:
            name = "misa";
            break;
        case 0x302:
            name = "medeleg";
            break;
        case 0x303:
            name = "mideleg";
            break;
        case 0x304:
            name = "mie";
            break;
        case 0x305:
            name = "mtvec";
            break;
        case 0x310:
            name = "mstatush";
            break;

        // Machine Trap Handling
        case 0x340:
            name = "mscratch";
            break;
        case 0x341:
            name = "mepc";
            break;
        case 0x342:
            name = "mcause";
            break;
        case 0x343:
            name = "mtval";
            break;
        case 0x344:
            name = "mip";
            break;

        // Machine Memory Protection
        case 0x3A0:
            name = "pmpcfg0";
            break;
        case 0x3B0:
            name = "pmpaddr0";
            break;

        // Machine Non-Maskable Interrupt Handling
        case 0x744:
            name = "mnstatus";
            break;

        // Supervisor Protection and Translation
        case 0x180:
            name = "satp";
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
const char *RiscvEmulatorGetMcauseException(const uint8_t interrupt, const uint32_t exceptioncode) {
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