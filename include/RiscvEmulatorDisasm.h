/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorDisasm_H_
#define RiscvEmulatorDisasm_H_

#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "RiscvEmulatorConfig.h"
#include "RiscvEmulatorDefineHook.h"
#include "RiscvEmulatorDefineOpcode.h"
#include "RiscvEmulatorTypeHook.h"
#include "RiscvEmulatorTypeEmulator.h"
#include "RiscvEmulatorDebug.h"

/**
 * Format-string storage for disassembly output. Defaults to a plain string
 * literal. A consumer may define this differently before including the
 * library (the AVR project maps it to PSTR(s) so format strings stay in
 * flash; see its RiscvEmulatorImplementationSpecific.h).
 */
#ifndef RVE_DISASM_FMT
#define RVE_DISASM_FMT(s) (s)
#endif

#if (RVE_E_DISASM == 1)

/**
 * Weak output function. Consumers override this to route disassembly output
 * to printf (Native), vfprintf_P (AVR), UART, TFT, etc. The default
 * implementation does nothing, so the feature costs nothing when unused.
 *
 * A consumer that provides its own override in the same translation unit
 * must define RVE_DISASM_PRINTF_OVERRIDE before including the library, so
 * the weak definition below is skipped (a weak definition cannot be
 * overridden within the same TU).
 */
void RiscvEmulatorDisasmPrintf(const char *fmt, ...) __attribute__((weak));

#ifndef RVE_DISASM_PRINTF_OVERRIDE
void RiscvEmulatorDisasmPrintf(const char *fmt, ...) {
    (void)fmt;
}
#endif

/**
 * Prints an integer value: hex, plus signed/unsigned decimal in parentheses.
 */
static inline void RiscvEmulatorDisasmPrintInteger(
    const char *name,
    const uint32_t value,
    const uint8_t length,
    const uint8_t issigned) {
    RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s: "), name);

    switch (length) {
        case 1: {
            RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("0x%02X"), (uint8_t)value);
            break;
        }
        case 2: {
            RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("0x%04X"), (uint16_t)value);
            break;
        }
        default: {
            RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("0x%08lX"), (unsigned long)value);
            break;
        }
    }

    if (issigned) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("(%ld)"), (long)(int32_t)value);
    } else {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("(%lu)"), (unsigned long)value);
    }
}

/**
 * Prints the "pc: ..., instruction: ..." header for the current instruction.
 */
static inline void RiscvEmulatorDisasmPrintHeader(
    const RiscvEmulatorState_t *state) {
    RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("pc: 0x%08lX"), (unsigned long)state->programcounter);
#if (RVE_E_C == 1)
    if (state->instruction.copcode.op != OPCODE16_QUADRANT_INVALID) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", instruction:     0x%04X"),
                                  (uint16_t)state->instruction.value);
    } else
#endif
    {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", instruction: 0x%08lX"),
                                  (unsigned long)state->instruction.value);
    }
}

static inline const char *RiscvEmulatorDisasmGetImmName(
    const RiscvEmulatorHookContext_t *hc) {
    if (hc->immname == NULL) {
        return "imm";
    }
    return hc->immname;
}

static inline uint8_t RiscvEmulatorDisasmGetImmLength(
    const RiscvEmulatorHookContext_t *hc) {
    if (hc->immlength == 0) {
        return 4;
    }
    return hc->immlength;
}

/**
 * Prints one register operand: ", label xN(name): 0x........".
 * Skipped entirely when the register pointer is not set.
 */
static inline void RiscvEmulatorDisasmPrintRegister(
    const char *label, uint8_t num, const char *name, const void *reg) {
    if (reg == NULL) {
        return;
    }
    RiscvEmulatorDisasmPrintf(
        RVE_DISASM_FMT(", %s x%u(%s): 0x%08lX"),
        label, num, name, (unsigned long)*(const uint32_t *)reg);
}

/**
 * Prints the END result line for a destination register.
 */
static inline void RiscvEmulatorDisasmPrintRegisterResult(
    uint8_t num, const char *name, const void *reg) {
    if (reg == NULL || num == 0) {
        return;
    }
    RiscvEmulatorDisasmPrintf(
        RVE_DISASM_FMT("                                         x%u(%s) = 0x%08lX\n"),
        num, name, (unsigned long)*(const uint32_t *)reg);
}

/**
 * R-type. BEGIN: rd, rs1, rs2. END: rd result.
 */
static inline void RiscvEmulatorDisasmPrintRType(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    (void)state;
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rd", hc->rdnum, hc->rdname, hc->rd);
        RiscvEmulatorDisasmPrintRegister("rs1", hc->rs1num, hc->rs1name, hc->rs1);
        RiscvEmulatorDisasmPrintRegister("rs2", hc->rs2num, hc->rs2name, hc->rs2);
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("\n"));
    } else if (hc->hook == HOOK_END) {
        RiscvEmulatorDisasmPrintRegisterResult(hc->rdnum, hc->rdname, hc->rd);
    }
}

/**
 * I-type. BEGIN: rd, rs1, imm. END: rd result.
 */
static inline void RiscvEmulatorDisasmPrintIType(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    (void)state;
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rd", hc->rdnum, hc->rdname, hc->rd);
        RiscvEmulatorDisasmPrintRegister("rs1", hc->rs1num, hc->rs1name, hc->rs1);
        RiscvEmulatorDisasmPrintInteger(
            RiscvEmulatorDisasmGetImmName(hc), hc->imm,
            RiscvEmulatorDisasmGetImmLength(hc), hc->immissigned);
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("\n"));
    } else if (hc->hook == HOOK_END) {
        RiscvEmulatorDisasmPrintRegisterResult(hc->rdnum, hc->rdname, hc->rd);
    }
}

/**
 * Load. BEGIN: rd, rs1, imm, memorylocation. END: width-aware rd result.
 */
static inline void RiscvEmulatorDisasmPrintLoad(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    (void)state;
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rd", hc->rdnum, hc->rdname, hc->rd);
        RiscvEmulatorDisasmPrintRegister("rs1", hc->rs1num, hc->rs1name, hc->rs1);
        RiscvEmulatorDisasmPrintInteger(
            RiscvEmulatorDisasmGetImmName(hc), hc->imm,
            RiscvEmulatorDisasmGetImmLength(hc), hc->immissigned);
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", memorylocation: 0x%08lX\n"),
                                  (unsigned long)hc->memorylocation);
    } else if (hc->hook == HOOK_END) {
        if (strcmp(hc->instruction, "lb") == 0) {
            RiscvEmulatorDisasmPrintf(
                RVE_DISASM_FMT("                                         x%u(%s) = %i\n"),
                hc->rdnum, hc->rdname, *(const int8_t *)hc->rd);
        } else if (strcmp(hc->instruction, "lbu") == 0) {
            RiscvEmulatorDisasmPrintf(
                RVE_DISASM_FMT("                                         x%u(%s) = %u\n"),
                hc->rdnum, hc->rdname, *(const uint8_t *)hc->rd);
        } else if (strcmp(hc->instruction, "lh") == 0) {
            RiscvEmulatorDisasmPrintf(
                RVE_DISASM_FMT("                                         x%u(%s) = %i\n"),
                hc->rdnum, hc->rdname, *(const int16_t *)hc->rd);
        } else if (strcmp(hc->instruction, "lhu") == 0) {
            RiscvEmulatorDisasmPrintf(
                RVE_DISASM_FMT("                                         x%u(%s) = %u\n"),
                hc->rdnum, hc->rdname, *(const uint16_t *)hc->rd);
        } else if (strcmp(hc->instruction, "c.lbu") == 0) {
            RiscvEmulatorDisasmPrintf(
                RVE_DISASM_FMT("                                         x%u(%s) = %u\n"),
                hc->rdnum, hc->rdname, *(const uint8_t *)hc->rd);
        } else if (strcmp(hc->instruction, "c.lhu") == 0) {
            RiscvEmulatorDisasmPrintf(
                RVE_DISASM_FMT("                                         x%u(%s) = %u\n"),
                hc->rdnum, hc->rdname, *(const uint16_t *)hc->rd);
        }
        RiscvEmulatorDisasmPrintf(
            RVE_DISASM_FMT("                                         x%u(%s) = 0x%08lX\n"),
            hc->rdnum, hc->rdname, (unsigned long)*(const uint32_t *)hc->rd);
    }
}

/**
 * Store. BEGIN: rs1, rs2, imm, memorylocation. END: width-aware memory result.
 */
static inline void RiscvEmulatorDisasmPrintStore(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    (void)state;
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rs1", hc->rs1num, hc->rs1name, hc->rs1);
        RiscvEmulatorDisasmPrintRegister("rs2", hc->rs2num, hc->rs2name, hc->rs2);
        RiscvEmulatorDisasmPrintInteger(
            RiscvEmulatorDisasmGetImmName(hc), hc->imm,
            RiscvEmulatorDisasmGetImmLength(hc), hc->immissigned);
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", memorylocation: 0x%08lX\n"),
                                  (unsigned long)hc->memorylocation);
    } else if (hc->hook == HOOK_END) {
        if (hc->length == 1) {
            RiscvEmulatorDisasmPrintf(
                RVE_DISASM_FMT("                                         0x%08lX = 0x%02X\n"),
                (unsigned long)hc->memorylocation, *(const uint8_t *)hc->rs2);
        } else if (hc->length == 2) {
            RiscvEmulatorDisasmPrintf(
                RVE_DISASM_FMT("                                         0x%08lX = 0x%04X\n"),
                (unsigned long)hc->memorylocation, *(const uint16_t *)hc->rs2);
        } else {
            RiscvEmulatorDisasmPrintf(
                RVE_DISASM_FMT("                                         0x%08lX = 0x%08lX\n"),
                (unsigned long)hc->memorylocation, (unsigned long)*(const uint32_t *)hc->rs2);
        }
    }
}

/**
 * Branch. BEGIN: rs1, rs2, imm. END: pc.
 */
static inline void RiscvEmulatorDisasmPrintBranch(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rs1", hc->rs1num, hc->rs1name, hc->rs1);
        RiscvEmulatorDisasmPrintRegister("rs2", hc->rs2num, hc->rs2name, hc->rs2);
        RiscvEmulatorDisasmPrintInteger(
            RiscvEmulatorDisasmGetImmName(hc), hc->imm,
            RiscvEmulatorDisasmGetImmLength(hc), hc->immissigned);
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("\n"));
    } else if (hc->hook == HOOK_END) {
        RiscvEmulatorDisasmPrintf(
            RVE_DISASM_FMT("                                         pc = 0x%08lX\n"),
            (unsigned long)state->programcounternext);
    }
}

/**
 * U-type. BEGIN: rd, imm. END: rd result.
 */
static inline void RiscvEmulatorDisasmPrintUType(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    (void)state;
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rd", hc->rdnum, hc->rdname, hc->rd);
        RiscvEmulatorDisasmPrintInteger(
            RiscvEmulatorDisasmGetImmName(hc), hc->imm,
            RiscvEmulatorDisasmGetImmLength(hc), hc->immissigned);
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("\n"));
    } else if (hc->hook == HOOK_END) {
        RiscvEmulatorDisasmPrintRegisterResult(hc->rdnum, hc->rdname, hc->rd);
    }
}

/**
 * J-type. BEGIN: rd, imm. END: rd result + pc.
 */
static inline void RiscvEmulatorDisasmPrintJType(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rd", hc->rdnum, hc->rdname, hc->rd);
        RiscvEmulatorDisasmPrintInteger(
            RiscvEmulatorDisasmGetImmName(hc), hc->imm,
            RiscvEmulatorDisasmGetImmLength(hc), hc->immissigned);
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("\n"));
    } else if (hc->hook == HOOK_END) {
        RiscvEmulatorDisasmPrintRegisterResult(hc->rdnum, hc->rdname, hc->rd);
        RiscvEmulatorDisasmPrintf(
            RVE_DISASM_FMT("                                         pc = 0x%08lX\n"),
            (unsigned long)state->programcounternext);
    }
}

/**
 * CSR. BEGIN: rd, rs1 (register form) or imm (immediate form), csr.
 * END: csr result + rd result.
 */
static inline void RiscvEmulatorDisasmPrintCSR(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    (void)state;
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rd", hc->rdnum, hc->rdname, hc->rd);
        if (hc->rs1 != NULL) {
            RiscvEmulatorDisasmPrintRegister("rs1", hc->rs1num, hc->rs1name, hc->rs1);
        } else {
            RiscvEmulatorDisasmPrintInteger(
                RiscvEmulatorDisasmGetImmName(hc), hc->imm,
                RiscvEmulatorDisasmGetImmLength(hc), hc->immissigned);
        }
        RiscvEmulatorDisasmPrintf(
            RVE_DISASM_FMT(", csr 0x%04X(%s): 0x%08lX\n"),
            hc->csrnum, hc->csrname, (unsigned long)*(const uint32_t *)hc->csr);
    } else if (hc->hook == HOOK_END) {
        RiscvEmulatorDisasmPrintf(
            RVE_DISASM_FMT("                                         %s = 0x%08lX\n"),
            hc->csrname, (unsigned long)*(const uint32_t *)hc->csr);
        RiscvEmulatorDisasmPrintRegisterResult(hc->rdnum, hc->rdname, hc->rd);
    }
}

/**
 * Plain instructions (fence, ecall, ebreak, mret, pause, ntl.*, mop.*,
 * c.ebreak). BEGIN: mnemonic only. END: pc for mret.
 */
static inline void RiscvEmulatorDisasmPrintPlain(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s\n"), hc->instruction);
    } else if (hc->hook == HOOK_END) {
        if (strcmp(hc->instruction, "mret") == 0) {
            RiscvEmulatorDisasmPrintf(
                RVE_DISASM_FMT("                                         pc = 0x%08lX\n"),
                (unsigned long)state->programcounternext);
        }
    }
}

/**
 * c.swsp. BEGIN: rs2, sp, imm, memorylocation. END: memory result.
 */
static inline void RiscvEmulatorDisasmPrintCSwsp(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    (void)state;
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rs2", hc->rs2num, hc->rs2name, hc->rs2);
        RiscvEmulatorDisasmPrintf(
            RVE_DISASM_FMT(", sp: 0x%08lX"), (unsigned long)*(const uint32_t *)&state->reg.sp);
        RiscvEmulatorDisasmPrintInteger(
            RiscvEmulatorDisasmGetImmName(hc), hc->imm,
            RiscvEmulatorDisasmGetImmLength(hc), hc->immissigned);
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", memorylocation: 0x%08lX\n"),
                                  (unsigned long)hc->memorylocation);
    } else if (hc->hook == HOOK_END) {
        RiscvEmulatorDisasmPrintf(
            RVE_DISASM_FMT("                                         0x%08lX = 0x%08lX\n"),
            (unsigned long)hc->memorylocation, (unsigned long)*(const uint32_t *)hc->rs2);
    }
}

/**
 * c.addi4spn. BEGIN: rd, sp, imm. END: rd result.
 */
static inline void RiscvEmulatorDisasmPrintCAddi4spn(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    (void)state;
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rd", hc->rdnum, hc->rdname, hc->rd);
        RiscvEmulatorDisasmPrintf(
            RVE_DISASM_FMT(", sp: 0x%08lX"), (unsigned long)*(const uint32_t *)&state->reg.sp);
        RiscvEmulatorDisasmPrintInteger(
            RiscvEmulatorDisasmGetImmName(hc), hc->imm,
            RiscvEmulatorDisasmGetImmLength(hc), hc->immissigned);
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("\n"));
    } else if (hc->hook == HOOK_END) {
        RiscvEmulatorDisasmPrintRegisterResult(hc->rdnum, hc->rdname, hc->rd);
    }
}

/**
 * c.lwsp. BEGIN: rd, sp, imm, memorylocation. END: rd result.
 */
static inline void RiscvEmulatorDisasmPrintCLwsp(
    const RiscvEmulatorState_t *state, const RiscvEmulatorHookContext_t *hc) {
    (void)state;
    if (hc->hook == HOOK_BEGIN) {
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", %s"), hc->instruction);
        RiscvEmulatorDisasmPrintRegister("rd", hc->rdnum, hc->rdname, hc->rd);
        RiscvEmulatorDisasmPrintf(
            RVE_DISASM_FMT(", sp: 0x%08lX"), (unsigned long)*(const uint32_t *)&state->reg.sp);
        RiscvEmulatorDisasmPrintInteger(
            RiscvEmulatorDisasmGetImmName(hc), hc->imm,
            RiscvEmulatorDisasmGetImmLength(hc), hc->immissigned);
        RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT(", memorylocation: 0x%08lX\n"),
                                  (unsigned long)hc->memorylocation);
    } else if (hc->hook == HOOK_END) {
        RiscvEmulatorDisasmPrintf(
            RVE_DISASM_FMT("                                         x%u(%s) = 0x%08lX\n"),
            hc->rdnum, hc->rdname, (unsigned long)*(const uint32_t *)hc->rd);
    }
}

#if (RVE_E_ZICSR == 1)
/**
 * Trap. Reads state->csr directly: mcause, mtval, mstatus, mepc, pc.
 * Called once (no BEGIN/END distinction).
 */
static inline void RiscvEmulatorDisasmPrintTrap(
    const RiscvEmulatorState_t *state) {
    const char *causedescription = RiscvEmulatorGetMcauseException(
        state->csr.mcause.interrupt,
        state->csr.mcause.exceptioncode);

    RiscvEmulatorDisasmPrintf(
        RVE_DISASM_FMT(", trap, interrupt: %d, exception code %d: %s\n"),
        state->csr.mcause.interrupt,
        state->csr.mcause.exceptioncode,
        causedescription);
    RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("                                         mtval = 0x%08lX\n"),
                              (unsigned long)state->csr.mtval);
    RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("                                         mstatus.mpp = %d\n"),
                              state->csr.mstatus.mpp);
    RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("                                         mstatus.mpie = %d\n"),
                              state->csr.mstatus.mpie);
    RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("                                         mstatus.mie = %d\n"),
                              state->csr.mstatus.mie);
    RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("                                         mepc = 0x%08lX\n"),
                              (unsigned long)state->csr.mepc);
    RiscvEmulatorDisasmPrintf(RVE_DISASM_FMT("                                         pc = 0x%08lX\n"),
                              (unsigned long)state->programcounternext);
}
#endif

#endif

#endif
