/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZbkb_H_
#define RiscvEmulatorExtensionZbkb_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZBKB == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Reverse bits in bytes: reverse the bits in each byte of rs1.
 * rd = brev8(rs1)
 */
static inline void RiscvEmulatorBREV8(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "brev8";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    uint32_t input = *(const uint32_t *)rs1;
    uint32_t output = 0;

    for (uint8_t i = 0; i < 32; i++) {
        if ((input >> i) & 1) {
            output |= (uint32_t)1 << ((i & ~0x7) | (7 - (i & 0x7)));
        }
    }

    *(uint32_t *)rd = output;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Pack halfwords: pack the lower halfword of rs1 and the lower halfword of rs2 into rd.
 * rd = (rs1 & 0xFFFF) | (rs2 << 16)
 */
static inline void RiscvEmulatorPACK(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "pack";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = (*(const uint32_t *)rs1 & 0xFFFF) | (*(const uint32_t *)rs2 << 16);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Pack bytes: pack the lower byte of rs1 and the lower byte of rs2 into rd.
 * rd = (rs1 & 0xFF) | ((rs2 & 0xFF) << 8)
 */
static inline void RiscvEmulatorPACKH(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "packh";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    *(uint32_t *)rd = (*(const uint32_t *)rs1 & 0xFF) | ((*(const uint32_t *)rs2 & 0xFF) << 8);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Zip: interleave the upper and lower halfwords of rs1 into the even and odd bit positions of rd.
 * rd = zip(rs1)
 */
static inline void RiscvEmulatorZIP(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "zip";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    uint32_t input = *(const uint32_t *)rs1;
    uint32_t output = 0;

    for (uint8_t i = 0; i < 16; i++) {
        if ((input >> i) & 1) {
            output |= (uint32_t)1 << (i * 2);
        }
        if ((input >> (i + 16)) & 1) {
            output |= (uint32_t)1 << (i * 2 + 1);
        }
    }

    *(uint32_t *)rd = output;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Unzip: gather the even bit positions of rs1 into the lower halfword of rd and the odd bit positions into the upper halfword.
 * rd = unzip(rs1)
 */
static inline void RiscvEmulatorUNZIP(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1) {

#if (RVE_E_HOOK == 1)
    state->hookexists = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "unzip";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
#endif

    if (rdnum == 0) {
        return;
    }

    uint32_t input = *(const uint32_t *)rs1;
    uint32_t output = 0;

    for (uint8_t i = 0; i < 16; i++) {
        if ((input >> (i * 2)) & 1) {
            output |= (uint32_t)1 << i;
        }
        if ((input >> (i * 2 + 1)) & 1) {
            output |= (uint32_t)1 << (i + 16);
        }
    }

    *(uint32_t *)rd = output;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

#endif

#endif
