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

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "brev8";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
#endif
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

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintPlain(state, &hc);
#endif
#endif
}

/**
 * Pack funnel for the register variants pack and packh. variant is the
 * combined funct7/funct3 decoder value (FUNCT7_FUNCT3_OPERATION_PACK/PACKH),
 * decoded once by the caller and shared by a single call site.
 */
static inline void RiscvEmulatorPACKOperation(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2,
    const uint16_t variant) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_PACK:
            hc.instruction = "pack";
            break;
        default: // FUNCT7_FUNCT3_OPERATION_PACKH
            hc.instruction = "packh";
            break;
    }
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2name = RiscvEmulatorGetRegisterSymbolicName(rs2num);
    hc.rs2 = rs2;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintRType(state, &hc);
#endif
#endif

    if (rdnum == 0) {
        return;
    }

    const uint32_t rs1value = *(const uint32_t *)rs1;
    const uint32_t rs2value = *(const uint32_t *)rs2;

    uint32_t result;

    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_PACK:
            // Pack halfwords: pack the lower halfword of rs1 and the lower
            // halfword of rs2 into rd. rd = (rs1 & 0xFFFF) | (rs2 << 16)
            result = (rs1value & 0xFFFF) | (rs2value << 16);
            break;
        default: // FUNCT7_FUNCT3_OPERATION_PACKH
            // Pack bytes: pack the lower byte of rs1 and the lower byte of
            // rs2 into rd. rd = (rs1 & 0xFF) | ((rs2 & 0xFF) << 8)
            result = (rs1value & 0xFF) | ((rs2value & 0xFF) << 8);
            break;
    }

    *(uint32_t *)rd = result;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
#endif
#endif
}

/**
 * Zip/unzip funnel.
 *
 * Zip: interleave the upper and lower halfwords of rs1 into the even and odd
 * bit positions of rd. rd = zip(rs1)
 * Unzip: gather the even bit positions of rs1 into the lower halfword of rd
 * and the odd bit positions into the upper halfword. rd = unzip(rs1)
 *
 * variant: IMM11_5_FUNCT3_IMMEDIATE_ZIP or IMM11_5_FUNCT3_IMMEDIATE_UNZIP.
 */
static inline void RiscvEmulatorZIPUNZIPOperation(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint16_t variant) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    switch (variant) {
        case IMM11_5_FUNCT3_IMMEDIATE_ZIP:
            hc.instruction = "zip";
            break;
        default: // IMM11_5_FUNCT3_IMMEDIATE_UNZIP
            hc.instruction = "unzip";
            break;
    }
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintRType(state, &hc);
#endif
#endif

    if (rdnum == 0) {
        return;
    }

    uint32_t input = *(const uint32_t *)rs1;
    uint32_t output = 0;

    switch (variant) {
        case IMM11_5_FUNCT3_IMMEDIATE_ZIP:
            for (uint8_t i = 0; i < 16; i++) {
                if ((input >> i) & 1) {
                    output |= (uint32_t)1 << (i * 2);
                }
                if ((input >> (i + 16)) & 1) {
                    output |= (uint32_t)1 << (i * 2 + 1);
                }
            }
            break;
        default: // IMM11_5_FUNCT3_IMMEDIATE_UNZIP
            for (uint8_t i = 0; i < 16; i++) {
                if ((input >> (i * 2)) & 1) {
                    output |= (uint32_t)1 << i;
                }
                if ((input >> (i * 2 + 1)) & 1) {
                    output |= (uint32_t)1 << (i + 16);
                }
            }
            break;
    }

    *(uint32_t *)rd = output;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
#endif
#endif
}

#endif

#endif
