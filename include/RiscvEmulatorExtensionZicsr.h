/*
 *
 * Copyright Marc Ketel
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
 * Apply WARL behavior to a value written to a CSR.
 */
static inline uint32_t RiscvEmulatorCSRWriteMask(RiscvEmulatorState_t *state __attribute__((unused)), const uint16_t csrnum, uint32_t value) {
    switch (csrnum) {
        case 0x300:
            value &= RVE_CSR_MSTATUS_WRITE_MASK;
            value |= RVE_CSR_MSTATUS_MPP_MMODE;
            break;
        case 0x301:
            value &= RVE_CSR_MISA_WRITE_MASK;
            break;
        case 0x302:
            value &= RVE_CSR_MEDELEG_WRITE_MASK;
            break;
        case 0x303:
            value &= RVE_CSR_MIDELEG_WRITE_MASK;
            break;
        case 0x304:
            value &= RVE_CSR_MIE_WRITE_MASK;
            break;
        case 0x305:
            value &= RVE_CSR_MTVEC_WRITE_MASK;
            break;
        case 0x310:
            value &= RVE_CSR_MSTATUSH_WRITE_MASK;
            break;
        case 0x180:
            value &= RVE_CSR_SATP_WRITE_MASK;
            break;
        case 0x341:
            value &= RVE_CSR_MEPC_WRITE_MASK;
            break;
        case 0x344:
            value &= RVE_CSR_MIP_WRITE_MASK;
            break;
        case 0x3A0:
            value &= RVE_CSR_PMPCFG0_WRITE_MASK;
            break;
        case 0x3B0:
            value &= RVE_CSR_PMPADDR0_WRITE_MASK;
            break;
        default:
            break;
    }

    return value;
}

/**
 * Apply WARL behavior to a value read from a CSR.
 */
static inline uint32_t RiscvEmulatorCSRReadMask(RiscvEmulatorState_t *state __attribute__((unused)), const uint16_t csrnum, uint32_t value) {
    switch (csrnum) {
        case 0x301:
            // misa: MXL is read-only RV32 and the I bit is always set.
            value &= ~(3ul << 30);
            value |= RVE_CSR_MISA_MXL_RV32 | RVE_CSR_MISA_I;
            break;
        case 0x341:
            value &= ~(uint32_t)RVE_CSR_MEPC_READ_MASK;
            break;
        default:
            break;
    }

    return value;
}

/**
 * Machine return: return from a trap in machine mode by restoring mstatus and jumping to mepc.
 * pc = mepc
 */
static inline void RiscvEmulatorMRET(RiscvEmulatorState_t *state) {
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "mret";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
#endif
#endif

    // TODO: Determine what the new privilege mode will be according to the values of MPP and MPV in mstatus.
    // Could this always be M-mode for this emulator?

    state->csr.mstatush.mpv = 0;
    state->csr.mstatus.mpp = 0;
    state->csr.mstatus.mie = state->csr.mstatus.mpie;
    state->csr.mstatus.mpie = 1;

    // TODO: Set the privilege mode as previously determined.

    state->programcounternext = RiscvEmulatorCSRReadMask(state, 0x341, state->csr.mepc);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintPlain(state, &hc);
#endif
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

            // Machine Counter Setup
#if (RVE_E_ZICNTR == 1)
        case 0xC00:
            address = (uint32_t *)(uintptr_t)&state->csr.cycle;
            break;
        case 0xC80:
            // The 0x?80 CSRs are the upper half of the 64-bit counter. The
            // counters are little-endian, so the upper half is the second
            // uint32_t of the uint64_t.
            address = (uint32_t *)(uintptr_t)&state->csr.cycle + 1;
            break;
        case 0xC01:
            address = (uint32_t *)(uintptr_t)&state->csr.time;
            break;
        case 0xC81:
            address = (uint32_t *)(uintptr_t)&state->csr.time + 1;
            break;
        case 0xC02:
            address = (uint32_t *)(uintptr_t)&state->csr.instret;
            break;
        case 0xC82:
            address = (uint32_t *)(uintptr_t)&state->csr.instret + 1;
            break;
#endif

        default:
            state->trapflag.illegalinstruction = 1;
            RiscvEmulatorUnknownCSR(state);
    }

    return address;
}

/**
 * Control and status register access: perform the requested atomic
 * read-modify-write operation on the CSR.
 *
 * Returns 1 when the instruction was handled, 0 for an unknown funct3.
 */
static inline uint8_t RiscvEmulatorCSR(
    RiscvEmulatorState_t *state,
    void *rd,
    const void *rs1,
    void *csr) {

    const uint8_t rdnum = state->instruction.itypecsr.rd;
    const uint8_t rs1num __attribute__((unused)) = state->instruction.itypecsr.rs1;
    const uint8_t funct3 = state->instruction.itypecsr.funct3;
    const uint16_t csrnum = state->instruction.itypecsr.csr;

    // The immediate variants (csrrwi, csrrsi, csrrci) hold a zero-extended
    // 5-bit immediate in the rs1 field instead of a register number.
    uint8_t immvariant;
    switch (funct3) {
        case FUNCT3_CSR_CSRRW:
        case FUNCT3_CSR_CSRRS:
        case FUNCT3_CSR_CSRRC:
            immvariant = 0;
            break;
        case FUNCT3_CSR_CSRRWI:
        case FUNCT3_CSR_CSRRSI:
        case FUNCT3_CSR_CSRRCI:
            immvariant = 1;
            break;
        default:
            return 0;
    }

    uint32_t value;
    if (immvariant) {
        value = state->instruction.itypecsrimm.imm;
    } else {
        value = *(const uint32_t *)rs1;
    }

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    const char *instructionname;
    switch (funct3) {
        case FUNCT3_CSR_CSRRW:
            instructionname = "csrrw";
            break;
        case FUNCT3_CSR_CSRRS:
            instructionname = "csrrs";
            break;
        case FUNCT3_CSR_CSRRC:
            instructionname = "csrrc";
            break;
        case FUNCT3_CSR_CSRRWI:
            instructionname = "csrrwi";
            break;
        case FUNCT3_CSR_CSRRSI:
            instructionname = "csrrsi";
            break;
        default:
            instructionname = "csrrci";
            break;
    }
    hc.instruction = instructionname;
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    if (immvariant) {
        hc.imm = rs1num;
        hc.immlength = sizeof(rs1num);
        hc.immname = "uimm";
    } else {
        hc.rs1num = rs1num;
        hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
        hc.rs1 = rs1;
    }
    hc.csrnum = csrnum;
    hc.csrname = RiscvEmulatorGetCSRName(csrnum);
    hc.csr = csr;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintCSR(state, &hc);
#endif
#endif

    // Read old value into destination register when requested.
    if (rdnum != 0) {
        *(uint32_t *)rd = RiscvEmulatorCSRReadMask(state, csrnum, *(uint32_t *)csr);
    }

    // csrrw always writes; csrrs and csrrc only write when the value is nonzero.
    if ((funct3 == FUNCT3_CSR_CSRRW) || (funct3 == FUNCT3_CSR_CSRRWI) || (value != 0)) {
        uint32_t oldvalue = *(uint32_t *)csr;
        uint32_t newvalue;
        if ((funct3 == FUNCT3_CSR_CSRRW) || (funct3 == FUNCT3_CSR_CSRRWI)) {
            newvalue = value;
        } else if ((funct3 == FUNCT3_CSR_CSRRS) || (funct3 == FUNCT3_CSR_CSRRSI)) {
            newvalue = oldvalue | value;
        } else {
            newvalue = oldvalue & ~value;
        }
        *(uint32_t *)csr = RiscvEmulatorCSRWriteMask(state, csrnum, newvalue);
    }

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintCSR(state, &hc);
#endif
#endif

    return 1;
}

#endif

#endif