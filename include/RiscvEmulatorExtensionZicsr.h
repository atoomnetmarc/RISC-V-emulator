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
#include "RiscvEmulatorHook.h"
#include "RiscvEmulatorType.h"

/**
 * Return from machine mode.
 */
static inline void RiscvEmulatorMRET(RiscvEmulatorState_t *state) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorMretHookBegin(state);
#endif

    // TODO: Determine what the new privilege mode will be according to the values of MPP and MPV in mstatus.
    // Could this always be M-mode for this emulator?

    state->csr.mstatush.mpv = 0;
    state->csr.mstatus.mpp = 0;
    state->csr.mstatus.mie = state->csr.mstatus.mpie;
    state->csr.mstatus.mpie = 1;

    // TODO: Set the privilege mode as previously determined.

    state->programcounternext = state->csr.mepc;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorMretHookEnd(state);
#endif
}

/**
 * Get the address of an CSR structure.
 *
 * Do not forget to update RiscvEmulatorGetRegisterSymbolicName()
 */
static inline void *RiscvEmulatorGetCSRAddress(RiscvEmulatorState_t *state, const uint16_t csr) {
    void *address = 0;
    switch (csr) {
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
static inline void RiscvEmulatorCSRRW(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint16_t csrnum __attribute__((unused)),
    const void *csr) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorCSRR_HookBegin("csrrw", state, rdnum, rd, rs1num, rs1, csrnum, csr);
#endif

    uint32_t originalvaluers1 = *(uint32_t *)rs1;

    // Read old value into destination register when requested.
    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    *(uint32_t *)csr = originalvaluers1;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorCSRR_HookEnd("csrrw", state, rdnum, rd, rs1num, rs1, csrnum, csr);
#endif
}

/**
 * Atomic read and write CSR, immediate.
 */
static inline void RiscvEmulatorCSRRWI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    const void *rd,
    const uint8_t imm,
    const uint16_t csrnum __attribute__((unused)),
    const void *csr) {

    // Read old value into destination register when requested.
    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    *(uint32_t *)csr = imm;
}

/**
 * Atomic read and set bits in CSR.
 */
static inline void RiscvEmulatorCSRRS(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint16_t csrnum __attribute__((unused)),
    const void *csr) {

    int32_t initialrs1value = *(uint32_t *)rs1;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorCSRR_HookBegin("csrrs", state, rdnum, rd, rs1num, rs1, csrnum, csr);
#endif

    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    // Set bits when requested.
    if (initialrs1value != 0) {
        *(uint32_t *)csr |= initialrs1value;
    }

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorCSRR_HookEnd("csrrs", state, rdnum, rd, rs1num, rs1, csrnum, csr);
#endif
}

/**
 * Atomic read and set bits in CSR, immediate.
 */
static inline void RiscvEmulatorCSRRSI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    const void *rd,
    const uint8_t imm,
    const uint16_t csrnum __attribute__((unused)),
    const void *csr) {

    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    // Set bits when requested.
    if (imm != 0) {
        *(uint32_t *)csr |= imm;
    }
}

/**
 * Atomic read and clear bits in CSR.
 */
static inline void RiscvEmulatorCSRRC(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    const void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint16_t csrnum __attribute__((unused)),
    const void *csr) {

    int32_t initialrs1value = *(uint32_t *)rs1;

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorCSRR_HookBegin("csrrc", state, rdnum, rd, rs1num, rs1, csrnum, csr);
#endif

    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    // Clear bits when requested.
    if (initialrs1value != 0) {
        *(uint32_t *)csr &= ~initialrs1value;
    }

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorCSRR_HookEnd("csrrc", state, rdnum, rd, rs1num, rs1, csrnum, csr);
#endif
}

/**
 * Atomic read and clear bits in CSR, immediate.
 */
static inline void RiscvEmulatorCSRRCI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    const void *rd,
    const uint8_t imm,
    const uint16_t csrnum __attribute__((unused)),
    const void *csr) {

    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    // Clear bits when requested.
    if (imm != 0) {
        *(uint32_t *)csr &= ~imm;
    }
}

#endif

#endif