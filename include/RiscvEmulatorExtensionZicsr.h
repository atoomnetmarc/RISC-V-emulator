/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorExtensionZicsr_H_
#define RiscvEmulatorExtensionZicsr_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZICSR == 1)

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Return from machine mode.
 */
static inline void RiscvEmulatorMRET(RiscvEmulatorState_t *state) {

    //TODO: Determine what the new privilege mode will be according to the values of MPP and MPV in mstatus.
    //Could this always be M-mode for this emulator?

    state->csr.mstatush.mpv = 0;
    state->csr.mstatus.mpp = 0;
    state->csr.mstatus.mie = state->csr.mstatus.mpie;
    state->csr.mstatus.mpie = 1;

    // TODO: Set the privilege mode as previously determined.

    state->programcounternext = state->csr.mepc;
}

/**
 * Get the address of an CSR structure.
 */
static inline void *RiscvEmulatorGetCSRAddress(RiscvEmulatorState_t *state) {
    void *address = 0;
    switch (state->instruction.itypecsr.csr) {
        // Machine Information Registers
        case 0xF14:
            address = &state->csr.mhartid;
            break;

        // Machine Trap Setup
        case 0x300:
            address = &state->csr.mstatus;
            break;
        case 0x301:
            address = &state->csr.misa;
            break;
        case 0x302:
            address = &state->csr.medeleg;
            break;
        case 0x303:
            address = &state->csr.mideleg;
            break;
        case 0x304:
            address = &state->csr.mie;
            break;
        case 0x305:
            address = &state->csr.mtvec;
            break;
        case 0x310:
            address = &state->csr.mstatush;
            break;

        // Machine Trap Handling
        case 0x340:
            address = &state->csr.mscratch;
            break;
        case 0x341:
            address = &state->csr.mepc;
            break;
        case 0x342:
            address = &state->csr.mcause;
            break;
        case 0x343:
            address = &state->csr.mtval;
            break;
        case 0x344:
            address = &state->csr.mip;
            break;

        // Machine Memory Protection
        case 0x3A0:
            address = &state->csr.pmpcfg0;
            break;
        case 0x3B0:
            address = &state->csr.pmpaddr0;
            break;

        // Machine Non-Maskable Interrupt Handling
        case 0x744:
            address = &state->csr.mnstatus;
            break;

        // Supervisor Protection and Translation
        case 0x180:
            address = &state->csr.satp;
            break;

        default:
            RiscvEmulatorUnknownCSR(state);
    }

    return address;
}

/**
 * Atomic read and write CSR.
 */
static inline void RiscvEmulatorCSRRW(void *rd, const void *rs1, void *csr) {
    uint32_t initialrs1value = *(uint32_t *)rs1;

    // Read old value into destination register when requested.
    if (*(uint32_t *)rd != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    *(uint32_t *)csr = initialrs1value;
}

/**
 * Atomic read and set bits in CSR.
 */
static inline void RiscvEmulatorCSRRS(void *rd, const void *rs1, void *csr) {
    int32_t initialrs1value = *(uint32_t *)rs1;

    *(uint32_t *)rd = *(uint32_t *)csr;

    // Set bits when requested.
    if (initialrs1value != 0) {
        *(uint32_t *)csr |= initialrs1value;
    }
}

/**
 * Atomic read and clear bits in CSR.
 */
static inline void RiscvEmulatorCSRRC(void *rd, const void *rs1, void *csr) {
    int32_t initialrs1value = *(uint32_t *)rs1;

    *(uint32_t *)rd = *(uint32_t *)csr;

    // Clear bits when requested.
    if (initialrs1value != 0) {
        *(uint32_t *)csr &= ~initialrs1value;
    }
}

#endif

#endif