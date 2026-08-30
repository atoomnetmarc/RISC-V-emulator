/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZbkx_H_
#define RiscvEmulatorExtensionZbkx_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZBKX == 1)

#include <stdint.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Crossbar permutation of nibbles: each nibble of rs2 selects a nibble of rs1, nibbles with an index of 8 or higher become zero.
 * rd[i] = (rs2[i] < 8) ? rs1[rs2[i]] : 0
 */
static inline void RiscvEmulatorXPERM4(
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
    hc.instruction = "xperm4";
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

    uint32_t input = *(const uint32_t *)rs2;
    uint32_t table = *(const uint32_t *)rs1;
    uint32_t output = 0;

    for (uint8_t i = 0; i < 8; i++) {
        uint8_t index = (input >> (i * 4)) & 0xF;
        if (index < 8) {
            output |= ((table >> (index * 4)) & 0xF) << (i * 4);
        }
    }

    *(uint32_t *)rd = output;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Crossbar permutation of bytes: each byte of rs2 selects a byte of rs1, bytes with an index of 4 or higher become zero.
 * rd[i] = (rs2[i] < 4) ? rs1[rs2[i]] : 0
 */
static inline void RiscvEmulatorXPERM8(
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
    hc.instruction = "xperm8";
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

    uint32_t input = *(const uint32_t *)rs2;
    uint32_t table = *(const uint32_t *)rs1;
    uint32_t output = 0;

    for (uint8_t i = 0; i < 4; i++) {
        uint8_t index = (input >> (i * 8)) & 0xFF;
        if (index < 4) {
            output |= ((table >> (index * 8)) & 0xFF) << (i * 8);
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
