/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionM_H_
#define RiscvEmulatorExtensionM_H_

#if (RVE_E_ZMMUL == 1)

#include <stdint.h>

#include "RiscvEmulatorConfig.h"
#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

/**
 * Multiply: multiply the lower 32 bits of rs1 and rs2, keeping the lower 32 bits of the result.
 * rd = (rs1 * rs2)[31:0]
 */
static inline void RiscvEmulatorMUL(
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
    hc.instruction = "mul";
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

    *(uint32_t *)rd = (*(const uint32_t *)rs1 * *(const uint32_t *)rs2);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Multiply high signed: multiply rs1 and rs2 as signed integers, keeping the upper 32 bits of the 64-bit result.
 * rd = (rs1 *s rs2)[63:32]
 */
static inline void RiscvEmulatorMULH(
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
    hc.instruction = "mulh";
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

    int64_t result = (int64_t)(*(const int32_t *)rs1 * (int64_t)*(const int32_t *)rs2);
    *(int32_t *)rd = (result >> 32);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Multiply high signed unsigned: multiply rs1 as signed and rs2 as unsigned, keeping the upper 32 bits of the 64-bit result.
 * rd = (rs1 *s rs2)[63:32]
 */
static inline void RiscvEmulatorMULHSU(
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
    hc.instruction = "mulhsu";
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

    int64_t result = (int64_t)*(const int32_t *)rs1 * (int64_t)(uint64_t)*(const uint32_t *)rs2;
    *(int32_t *)rd = (result >> 32);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Multiply high unsigned: multiply rs1 and rs2 as unsigned integers, keeping the upper 32 bits of the 64-bit result.
 * rd = (rs1 *u rs2)[63:32]
 */
static inline void RiscvEmulatorMULHU(
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
    hc.instruction = "mulhu";
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

    uint64_t result = (uint64_t)(*(const uint32_t *)rs1 * (uint64_t)*(const uint32_t *)rs2);
    *(uint32_t *)rd = (result >> 32);

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

#endif

#if (RVE_E_M == 1)

/**
 * Divide signed: divide rs1 by rs2 as signed integers.
 * rd = rs1 /s rs2
 */
static inline void RiscvEmulatorDIV(
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
    hc.instruction = "div";
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

    if (*(const int32_t *)rs2 == 0) {
        // Division by zero.
        *(int32_t *)rd = -1;
    } else if (*(const int32_t *)rs1 == INT32_MIN && *(const int32_t *)rs2 == -1) {
        // Overflow.
        *(int32_t *)rd = INT32_MIN;
    } else {
        *(int32_t *)rd = (*(const int32_t *)rs1 / *(const int32_t *)rs2);
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Divide unsigned: divide rs1 by rs2 as unsigned integers.
 * rd = rs1 /u rs2
 */
static inline void RiscvEmulatorDIVU(
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
    hc.instruction = "divu";
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

    if (*(const uint32_t *)rs2 == 0) {
        // Division by zero.
        *(uint32_t *)rd = UINT32_MAX;
    } else {
        *(uint32_t *)rd = (*(const uint32_t *)rs1 / *(const uint32_t *)rs2);
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Remainder signed: take the remainder of rs1 divided by rs2 as signed integers.
 * rd = rs1 %s rs2
 */
static inline void RiscvEmulatorREM(
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
    hc.instruction = "rem";
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

    if (*(const int32_t *)rs2 == 0) {
        // Division by zero.
        *(int32_t *)rd = *(const int32_t *)rs1;
    } else if (*(const int32_t *)rs1 == INT32_MIN && *(const int32_t *)rs2 == -1) {
        // Overflow.
        *(int32_t *)rd = 0;
    } else {
        *(int32_t *)rd = (*(const int32_t *)rs1 % *(const int32_t *)rs2);
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

/**
 * Remainder unsigned: take the remainder of rs1 divided by rs2 as unsigned integers.
 * rd = rs1 %u rs2
 */
static inline void RiscvEmulatorREMU(
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
    hc.instruction = "remu";
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

    if (*(const uint32_t *)rs2 == 0) {
        // Division by zero.
        *(uint32_t *)rd = *(const uint32_t *)rs1;
    } else {
        *(uint32_t *)rd = (*(const uint32_t *)rs1 % *(const uint32_t *)rs2);
    }

#if (RVE_E_HOOK == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#endif
}

#endif

#endif