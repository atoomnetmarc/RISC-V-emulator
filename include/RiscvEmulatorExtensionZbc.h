/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionZbc_H_
#define RiscvEmulatorExtensionZbc_H_

#include "RiscvEmulatorConfig.h"

#if ((RVE_E_ZBC == 1) || (RVE_E_ZBKC == 1))

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Carry-less multiply low-part: multiply rs1 and rs2 without carries, keeping the lower 32 bits of the result.
 * rd = clmul(rs1, rs2)[31:0]
 */
static inline void RiscvEmulatorCLMUL(
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
    hc.instruction = "clmul";
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

    uint32_t output = 0;

    for (uint8_t i = 0; i < 32; i++) {
        if ((*(const uint32_t *)rs2 >> i) & 1) {
            output ^= *(const uint32_t *)rs1 << i;
        }
    }

    *(uint32_t *)rd = output;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Carry-less multiply high-part: multiply rs1 and rs2 without carries, keeping the upper 32 bits of the result.
 * rd = clmul(rs1, rs2)[63:32]
 */
static inline void RiscvEmulatorCLMULH(
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
    hc.instruction = "clmulh";
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

    uint32_t output = 0;

    for (uint8_t i = 1; i < 32; i++) {
        if ((*(const uint32_t *)rs2 >> i) & 1) {
            output ^= *(const uint32_t *)rs1 >> (32 - i);
        }
    }

    *(uint32_t *)rd = output;

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

#if (RVE_E_ZBC == 1)
/**
 * Carry-less multiply reversed: multiply rs1 and rs2 without carries, keeping bits 31 to 0 of the reversed result.
 * rd = clmulr(rs1, rs2)
 */
static inline void RiscvEmulatorCLMULR(
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
    hc.instruction = "clmulr";
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

    uint32_t output = 0;

    for (uint8_t i = 0; i < 32; i++) {
        if ((*(const uint32_t *)rs2 >> i) & 1) {
            output ^= *(const uint32_t *)rs1 >> (32 - i - 1);
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

#endif