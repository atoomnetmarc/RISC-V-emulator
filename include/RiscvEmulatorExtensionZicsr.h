/*

Copyright 2023 Marc Ketel
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

// Get the address of an CSR structure
static inline void *RiscvEmulatorGetCSRAddress(RiscvEmulatorState_t *state)
{
    void *address = 0;
    switch (state->instruction.itypecsr.csr)
    {
        case 0x180:
            address = &state->csr.satp;
            break;
        case 0x300:
            address = &state->csr.mstatus;
            break;
        case 0x310:
            address = &state->csr.mstatush;
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
        case 0x341:
            address = &state->csr.mepc;
            break;
        case 0x3A0:
            address = &state->csr.pmpcfg0;
            break;
        case 0x3B0:
            address = &state->csr.pmpaddr0;
            break;
        case 0x744:
            address = &state->csr.mnstatus;
            break;
        case 0xF14:
            address = &state->csr.mhartid;
            break;
        default:
            RiscvEmulatorUnknownCSR(state);
    }

    return address;
}

// Atomic read and write CSR
static inline void RiscvEmulatorCSRRW(RiscvEmulatorState_t *state, void *rd, const void *rs1, void *csr)
{
    // Read old value into destination register when requested.
    if (*(uint32_t *)rd != 0)
        *(uint32_t *)rd = *(uint32_t *)csr;

    *(uint32_t *)csr = *(uint32_t *)rs1;
}

// Atomic read and set bits in CSR
static inline void RiscvEmulatorCSRRS(RiscvEmulatorState_t *state, void *rd, const void *rs1, void *csr)
{
    *(uint32_t *)rd = *(uint32_t *)csr;

    // Set bits when requested
    if (*(uint32_t *)rs1 != 0)
        *(uint32_t *)csr |= *(uint32_t *)rs1;
}

// Atomic read and clear bits in CSR
static inline void RiscvEmulatorCSRRC(RiscvEmulatorState_t *state, void *rd, const void *rs1, void *csr)
{
    *(uint32_t *)rd = *(uint32_t *)csr;

    // Clear bits when requested
    if (*(uint32_t *)rs1 != 0)
        *(uint32_t *)csr &= ~*(uint32_t *)rs1;
}

#endif

#endif