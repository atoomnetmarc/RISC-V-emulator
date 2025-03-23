/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
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
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "mret";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
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
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Get the address of an CSR structure.
 *
 * Do not forget to update RiscvEmulatorGetCSRName()
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

#if (RVE_E_F == 1)
        // Floating-Point Control and Status Register
        case 0x003:
            address = &state->csr.fcsr;
            break;
#endif

        default:
            state->trapflag.illegalinstruction = 1;
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
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "csrrw";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.csrnum = csrnum;
    hc.csr = csr;
    RiscvEmulatorHook(state, &hc);
#endif

    uint32_t originalvaluers1 = *(uint32_t *)rs1;

    // Read old value into destination register when requested.
    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    *(uint32_t *)csr = originalvaluers1;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Atomic read and write CSR, immediate.
 */
static inline void RiscvEmulatorCSRRWI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    const void *rd,
    const uint8_t uimm,
    const uint16_t csrnum __attribute__((unused)),
    const void *csr) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "csrrwi";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = uimm;
    hc.csrnum = csrnum;
    hc.csr = csr;
    hc.immlength = sizeof(uimm);
    hc.immname = "uimm";
    RiscvEmulatorHook(state, &hc);
#endif

    // Read old value into destination register when requested.
    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    *(uint32_t *)csr = uimm;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
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
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "csrrs";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.csrnum = csrnum;
    hc.csr = csr;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    // Set bits when requested.
    if (initialrs1value != 0) {
        *(uint32_t *)csr |= initialrs1value;
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Atomic read and set bits in CSR, immediate.
 */
static inline void RiscvEmulatorCSRRSI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    const void *rd,
    const uint8_t uimm,
    const uint16_t csrnum __attribute__((unused)),
    const void *csr) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "csrrsi";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = uimm;
    hc.csrnum = csrnum;
    hc.csr = csr;
    hc.immlength = sizeof(uimm);
    hc.immname = "uimm";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    // Set bits when requested.
    if (uimm != 0) {
        *(uint32_t *)csr |= uimm;
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
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
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "csrrc";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.csrnum = csrnum;
    hc.csr = csr;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    // Clear bits when requested.
    if (initialrs1value != 0) {
        *(uint32_t *)csr &= ~initialrs1value;
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Atomic read and clear bits in CSR, immediate.
 */
static inline void RiscvEmulatorCSRRCI(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    const void *rd,
    const uint8_t uimm,
    const uint16_t csrnum __attribute__((unused)),
    const void *csr) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "csrrci";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.imm = uimm;
    hc.csrnum = csrnum;
    hc.csr = csr;
    hc.immlength = sizeof(uimm);
    hc.immname = "uimm";
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum != 0) {
        *(uint32_t *)rd = *(uint32_t *)csr;
    }

    // Clear bits when requested.
    if (uimm != 0) {
        *(uint32_t *)csr &= ~uimm;
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

#endif

#endif