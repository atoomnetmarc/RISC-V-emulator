/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionI_H_
#define RiscvEmulatorExtensionI_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorHook.h"
#include "RiscvEmulatorType.h"

#include "RiscvEmulatorExtensionM.h"
#include "RiscvEmulatorExtensionZba.h"
#include "RiscvEmulatorExtensionZbb.h"
#include "RiscvEmulatorExtensionZbc.h"
#include "RiscvEmulatorExtensionZbkb.h"
#include "RiscvEmulatorExtensionZbkx.h"
#include "RiscvEmulatorExtensionZbs.h"
#include "RiscvEmulatorExtensionZicond.h"
#include "RiscvEmulatorExtensionZicsr.h"
#include "RiscvEmulatorExtensionZihintntl.h"
#include "RiscvEmulatorExtensionZihintpause.h"
#include "RiscvEmulatorExtensionZimop.h"

/**
 * Jump and link register: jump to rs1 plus the sign-extended offset, clearing the least significant bit, and store the return address in rd.
 * rd = pc + 4; pc = (rs1 + imm) & ~1
 */
static inline void RiscvEmulatorJALR(RiscvEmulatorState_t *state, uint8_t rdnum, void *rd, uint8_t rs1num, void *rs1, const int16_t imm) {
    uint32_t immu = (uint32_t)imm;
    uint32_t jumptoprogramcounter = (*(const uint32_t *)rs1 + immu) & (UINT32_MAX - 1);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    // Pseudoinstructions: ret (jalr x0, ra, 0) and jr (jalr x0, rs1, offset).
    if (rdnum == 0) {
        if (rs1num == 1 && imm == 0) {
            hc.instruction = "ret";
        } else {
            hc.instruction = "jr";
        }
    } else {
        hc.instruction = "jalr";
    }
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.imm = (uint32_t)imm;
    hc.immissigned = 1;
    hc.immname = "offset";
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintJType(state, &hc);
#endif
#endif

#if (RVE_E_ZICSR == 1) && (RVE_E_C != 1)
    // Check if jumptoprogramcounter is aligned.
    if ((jumptoprogramcounter & 0b11) != 0) {
        state->trapflag.instructionaddressmisaligned = 1;
        state->csr.mtval = jumptoprogramcounter;
        return;
    }
#endif

    // Set destination register to current next instruction acting as a return address.
    if (rdnum != 0) {
        *(uint32_t *)rd = state->programcounternext;
    }

    // Execute jump.
    state->programcounternext = jumptoprogramcounter;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintJType(state, &hc);
#endif
#endif
}

/**
 * Process JALR opcode.
 */
static inline void RiscvEmulatorOpcodeJumpAndLinkRegister(RiscvEmulatorState_t *state, uint8_t rdnum, void *rd, uint8_t rs1num, void *rs1, uint8_t funct3, int16_t imm) {
    if (funct3 == FUNCT3_JUMPANDLINKREGISTER_JALR) {
        RiscvEmulatorJALR(state, rdnum, rd, rs1num, rs1, imm);
    } else {
        state->trapflag.illegalinstruction = 1;
    }
}

/**
 * Operation funnel for add/addi, slt/slti, sltu/sltiu, xor/xori, or/ori and
 * and/andi (shared funct3). immvariant selects the immediate variant; the
 * second operand is the rs2 register value or the sign-extended immediate.
 * All instruction fields are decoded by the caller (decode-once). Returns 1
 * when handled, 0 when funct3 does not match a funneled operation.
 */
static inline uint8_t RiscvEmulatorOperation(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2 __attribute__((unused)),
    const uint8_t immvariant,
    const uint32_t imm __attribute__((unused)),
    const uint8_t funct3) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    // Instruction name, including pseudoinstructions derived from the decoded fields.
    switch (funct3) {
        case FUNCT3_IMMEDIATE_ADDI:
            if (immvariant == 0) {
                hc.instruction = "add";
            } else if (rdnum == 0) {
                hc.instruction = "nop"; // addi x0, x0, 0
            } else if (imm == 0) {
                hc.instruction = "mv"; // addi rd, rs1, 0
            } else {
                hc.instruction = "addi";
            }
            break;
        case FUNCT3_IMMEDIATE_SLTI:
            if (immvariant == 0) {
                if (rs2num == 0) {
                    hc.instruction = "sltz"; // slt rd, rs1, x0
                } else if (rs1num == 0) {
                    hc.instruction = "sgtz"; // slt rd, x0, rs2
                } else {
                    hc.instruction = "slt";
                }
            } else {
                hc.instruction = "slti";
            }
            break;
        case FUNCT3_IMMEDIATE_SLTIU:
            if (immvariant == 0) {
                hc.instruction = (rs1num == 0) ? "snez" : "sltu"; // snez: sltu rd, x0, rs2
            } else {
                hc.instruction = (imm == 1) ? "seqz" : "sltiu"; // seqz: sltiu rd, rs1, 1
            }
            break;
        case FUNCT3_IMMEDIATE_XORI:
            if (immvariant == 0) {
                hc.instruction = "xor";
            } else {
                hc.instruction = ((int16_t)imm == -1) ? "not" : "xori"; // not: xori rd, rs1, -1
            }
            break;
        case FUNCT3_IMMEDIATE_ORI:
            hc.instruction = (immvariant == 0) ? "or" : "ori";
            break;
        case FUNCT3_IMMEDIATE_ANDI:
            hc.instruction = (immvariant == 0) ? "and" : "andi";
            break;
    }
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    if (immvariant == 0) {
        hc.rs2num = rs2num;
        hc.rs2name = RiscvEmulatorGetRegisterSymbolicName(rs2num);
        hc.rs2 = rs2;
    } else {
        hc.imm = imm;
        hc.immissigned = 1;
        hc.immlength = (funct3 == FUNCT3_IMMEDIATE_SLTIU || funct3 == FUNCT3_IMMEDIATE_XORI) ? sizeof(uint32_t) : sizeof(int16_t);
    }
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    if (immvariant == 0) {
        RiscvEmulatorDisasmPrintRType(state, &hc);
    } else {
        RiscvEmulatorDisasmPrintIType(state, &hc);
    }
#endif
#endif

    if (rdnum == 0) {
        return 1;
    }

    const uint32_t operand2 = (immvariant == 0) ? *(const uint32_t *)rs2 : imm; // rs2 value or sign-extended immediate

    uint32_t result;
    switch (funct3) {
        case FUNCT3_IMMEDIATE_SLTI:
            result = (*(const int32_t *)rs1 < (int32_t)operand2);
            break;
        case FUNCT3_IMMEDIATE_SLTIU:
            result = (*(const uint32_t *)rs1 < operand2);
            break;
        case FUNCT3_IMMEDIATE_ADDI:
        case FUNCT3_IMMEDIATE_XORI:
        case FUNCT3_IMMEDIATE_ORI:
        case FUNCT3_IMMEDIATE_ANDI: {
            const uint32_t a = *(const uint32_t *)rs1;
            switch (funct3) {
                case FUNCT3_IMMEDIATE_ADDI:
                    result = a + operand2;
                    break;
                case FUNCT3_IMMEDIATE_XORI:
                    result = a ^ operand2;
                    break;
                case FUNCT3_IMMEDIATE_ORI:
                    result = a | operand2;
                    break;
                default: // FUNCT3_IMMEDIATE_ANDI
                    result = a & operand2;
                    break;
            }
            break;
        }
        default:
            return 0;
    }
    *(uint32_t *)rd = result;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    if (immvariant == 0) {
        RiscvEmulatorDisasmPrintRType(state, &hc);
    } else {
        RiscvEmulatorDisasmPrintIType(state, &hc);
    }
#endif
#endif
    return 1;
}

/**
 * Subtract: subtract rs2 from rs1.
 * rd = rs1 - rs2
 */
static inline void RiscvEmulatorSUB(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t rs2num __attribute__((unused)),
    const void *rs2) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    // Pseudoinstruction: neg (sub rd, x0, rs2).
    if (rs1num == 0) {
        hc.instruction = "neg";
    } else {
        hc.instruction = "sub";
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

    *(int32_t *)rd = *(const int32_t *)rs1 - *(const int32_t *)rs2;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintRType(state, &hc);
#endif
#endif
}

/**
 * Shift operation funnel for the register variants sll, srl and sra. variant
 * is the combined funct7/funct3 decoder value (FUNCT7_FUNCT3_OPERATION_
 * SLL/SRL/SRA), decoded once by the caller and shared by a single call site.
 */
static inline void RiscvEmulatorShiftOperation(
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
    // Instruction name.
    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_SLL:
            hc.instruction = "sll";
            break;
        case FUNCT7_FUNCT3_OPERATION_SRL:
            hc.instruction = "srl";
            break;
        default: // FUNCT7_FUNCT3_OPERATION_SRA
            hc.instruction = "sra";
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

    const uint32_t value = *(const uint32_t *)rs1;
    const uint32_t amount = *(const uint32_t *)rs2 & 0b11111;

    uint32_t result;
    switch (variant) {
        case FUNCT7_FUNCT3_OPERATION_SLL:
            result = value << amount;
            break;
        case FUNCT7_FUNCT3_OPERATION_SRL:
            result = value >> amount;
            break;
        default: // FUNCT7_FUNCT3_OPERATION_SRA
            result = (uint32_t)((int32_t)value >> amount);
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
 * Shift operation funnel for the immediate variants slli, srli and srai.
 * variant is the combined imm11_5/funct3 decoder value (IMM11_5_FUNCT3_
 * IMMEDIATE_SLLI/SRLI/SRAI), decoded once by the caller and shared by a
 * single call site.
 */
static inline void RiscvEmulatorShiftOperationImmediate(
    RiscvEmulatorState_t *state __attribute__((unused)),
    const uint8_t rdnum,
    void *rd,
    const uint8_t rs1num __attribute__((unused)),
    const void *rs1,
    const uint8_t shamt,
    const uint16_t variant) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    // Instruction name.
    switch (variant) {
        case IMM11_5_FUNCT3_IMMEDIATE_SLLI:
            hc.instruction = "slli";
            break;
        case IMM11_5_FUNCT3_IMMEDIATE_SRLI:
            hc.instruction = "srli";
            break;
        default: // IMM11_5_FUNCT3_IMMEDIATE_SRAI
            hc.instruction = "srai";
            break;
    }
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.imm = shamt;
    hc.immlength = sizeof(shamt);
    hc.immname = "shamt";
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintIType(state, &hc);
#endif
#endif

    if (rdnum == 0) {
        return;
    }

    const uint32_t value = *(const uint32_t *)rs1;
    const uint32_t amount = shamt & 0b11111;

    uint32_t result;
    switch (variant) {
        case IMM11_5_FUNCT3_IMMEDIATE_SLLI:
            result = value << amount;
            break;
        case IMM11_5_FUNCT3_IMMEDIATE_SRLI:
            result = value >> amount;
            break;
        default: // IMM11_5_FUNCT3_IMMEDIATE_SRAI
            result = (uint32_t)((int32_t)value >> amount);
            break;
    }
    *(uint32_t *)rd = result;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintIType(state, &hc);
#endif
#endif
}

/**
 * Process operation opcodes.
 */
static inline void RiscvEmulatorOpcodeOperation(RiscvEmulatorState_t *state, uint8_t rdnum, void *rd, uint8_t rs1num, void *rs1, uint8_t rs2num, void *rs2, uint8_t funct3, uint8_t funct7) {
    int8_t detectedUnknownInstruction = 1;

    if (detectedUnknownInstruction == 1) {
        RiscvInstructionTypeRDecoderFunct7Funct3_u instruction_decoderhelper_rtype = {0};
        instruction_decoderhelper_rtype.funct3 = funct3;
        instruction_decoderhelper_rtype.funct7 = funct7;

        detectedUnknownInstruction = -1;
        switch (instruction_decoderhelper_rtype.funct7_3) {
            case FUNCT7_FUNCT3_OPERATION_ADD:
            case FUNCT7_FUNCT3_OPERATION_SLT:
            case FUNCT7_FUNCT3_OPERATION_SLTU:
            case FUNCT7_FUNCT3_OPERATION_XOR:
            case FUNCT7_FUNCT3_OPERATION_OR:
            case FUNCT7_FUNCT3_OPERATION_AND:
#if (RVE_E_ZIHINTNTL == 1)
                // ntl.p1/pall/s1/all are encoded as add x0, x0, x2/x3/x4/x5.
                if (funct3 == FUNCT3_IMMEDIATE_ADDI && rdnum == 0 && rs1num == 0) {
                    if (rs2num == FUNCT7_FUNCT3_RS2_NTL_P1) {
                        RiscvEmulatorNTLP1(state);
                        break;
                    }
                    if (rs2num == FUNCT7_FUNCT3_RS2_NTL_PALL) {
                        RiscvEmulatorNTLPALL(state);
                        break;
                    }
                    if (rs2num == FUNCT7_FUNCT3_RS2_NTL_S1) {
                        RiscvEmulatorNTLS1(state);
                        break;
                    }
                    if (rs2num == FUNCT7_FUNCT3_RS2_NTL_ALL) {
                        RiscvEmulatorNTLALL(state);
                        break;
                    }
                }
#endif
                if (RiscvEmulatorOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, 0, 0, funct3) == 0) {
                    detectedUnknownInstruction = 1;
                }
                break;
            case FUNCT7_FUNCT3_OPERATION_SUB:
                RiscvEmulatorSUB(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
            case FUNCT7_FUNCT3_OPERATION_SLL:
            case FUNCT7_FUNCT3_OPERATION_SRL:
            case FUNCT7_FUNCT3_OPERATION_SRA:
                RiscvEmulatorShiftOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, instruction_decoderhelper_rtype.funct7_3);
                break;
#if ((RVE_E_ZMMUL == 1) || (RVE_E_M == 1))
            case FUNCT7_FUNCT3_OPERATION_MUL:
            case FUNCT7_FUNCT3_OPERATION_MULH:
            case FUNCT7_FUNCT3_OPERATION_MULHSU:
            case FUNCT7_FUNCT3_OPERATION_MULHU:
#if (RVE_E_M == 1)
            case FUNCT7_FUNCT3_OPERATION_DIV:
            case FUNCT7_FUNCT3_OPERATION_DIVU:
            case FUNCT7_FUNCT3_OPERATION_REM:
            case FUNCT7_FUNCT3_OPERATION_REMU:
#endif
                RiscvEmulatorMOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, instruction_decoderhelper_rtype.funct7_3);
                break;
#endif
#if (RVE_E_ZBA == 1)
            case FUNCT7_FUNCT3_OPERATION_SH1ADD:
            case FUNCT7_FUNCT3_OPERATION_SH2ADD:
            case FUNCT7_FUNCT3_OPERATION_SH3ADD:
                RiscvEmulatorSHADD(state, rdnum, rd, rs1num, rs1, rs2num, rs2);
                break;
#endif
#if (RVE_E_ZBB == 1)
            case FUNCT7_FUNCT3_OPERATION_ANDN:
            case FUNCT7_FUNCT3_OPERATION_ORN:
            case FUNCT7_FUNCT3_OPERATION_XNOR:
            case FUNCT7_FUNCT3_OPERATION_MAX:
            case FUNCT7_FUNCT3_OPERATION_MAXU:
            case FUNCT7_FUNCT3_OPERATION_MIN:
            case FUNCT7_FUNCT3_OPERATION_MINU:
            case FUNCT7_FUNCT3_OPERATION_ROL:
            case FUNCT7_FUNCT3_OPERATION_ROR:
                RiscvEmulatorRTypeOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, instruction_decoderhelper_rtype.funct7_3);
                break;
#endif
#if ((RVE_E_ZBC == 1) || (RVE_E_ZBKC == 1))
            case FUNCT7_FUNCT3_OPERATION_CMUL:
            case FUNCT7_FUNCT3_OPERATION_CMULH:
#if (RVE_E_ZBC == 1)
            case FUNCT7_FUNCT3_OPERATION_CMULR:
#endif
                RiscvEmulatorCLMULOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, instruction_decoderhelper_rtype.funct7_3);
                break;
#endif
#if (RVE_E_ZBS == 1)
            case FUNCT7_FUNCT3_OPERATION_BCLR:
            case FUNCT7_FUNCT3_OPERATION_BEXT:
            case FUNCT7_FUNCT3_OPERATION_BINV:
            case FUNCT7_FUNCT3_OPERATION_BSET:
                RiscvEmulatorBitOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, instruction_decoderhelper_rtype.funct7_3);
                break;
#endif
#if (RVE_E_ZBKB == 1)
            case FUNCT7_FUNCT3_OPERATION_PACK:
            case FUNCT7_FUNCT3_OPERATION_PACKH:
                RiscvEmulatorPACKOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, instruction_decoderhelper_rtype.funct7_3);
                break;
#endif
#if (RVE_E_ZBKX == 1)
            case FUNCT7_FUNCT3_OPERATION_XPERM8:
            case FUNCT7_FUNCT3_OPERATION_XPERM4:
                RiscvEmulatorXPERMOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, instruction_decoderhelper_rtype.funct7_3);
                break;
#endif
#if (RVE_E_ZICOND == 1)
            case FUNCT7_FUNCT3_OPERATION_CZEROEQZ:
            case FUNCT7_FUNCT3_OPERATION_CZERONEZ:
                RiscvEmulatorCZEROOperation(state, rdnum, rd, rs1num, rs1, rs2num, rs2, instruction_decoderhelper_rtype.funct7_3);
                break;
#endif
            default:
                detectedUnknownInstruction = 1;
                break;
        }
    }

#if (RVE_E_ZBB == 1)
    if (detectedUnknownInstruction == 1) {
        RiscvInstructionTypeRDecoderFunct3Rs2Funct7_u instruction_decoderhelper_rtype_Funct3Rs2Funct7 = {0};
        instruction_decoderhelper_rtype_Funct3Rs2Funct7.funct3 = funct3;
        instruction_decoderhelper_rtype_Funct3Rs2Funct7.rs2 = rs2num;
        instruction_decoderhelper_rtype_Funct3Rs2Funct7.funct7 = funct7;

        detectedUnknownInstruction = -1;
        switch (instruction_decoderhelper_rtype_Funct3Rs2Funct7.funct3_rs2_funct7) {
            case FUNCT7_RS2_FUNCT3_OPERATION_ZEXTH:
                RiscvEmulatorZEXT_H(state, rdnum, rd, rs1num, rs1);
                break;

            default:
                detectedUnknownInstruction = 1;
                break;
        }
    }
#endif

    if (detectedUnknownInstruction == 1) {
        state->trapflag.illegalinstruction = 1;
    }
}

/**
 * Process immediate opcodes.
 */
static inline void RiscvEmulatorOpcodeImmediate(RiscvEmulatorState_t *state, uint8_t rdnum, void *rd, uint8_t rs1num, void *rs1, uint8_t funct3, int16_t imm) {
    int8_t detectedUnknownInstruction = 1;

#if (RVE_E_ZBB == 1)
    if (detectedUnknownInstruction == 1) {
        // If funct3 == 0b001 or 0b101 then a whole set of functions are encoded in imm.
        if (funct3 == FUNCT3_IMMEDIATE_FUNCTIONS_1 ||
            funct3 == FUNCT3_IMMEDIATE_FUNCTIONS_5) {
            RiscvInstructionTypeIDecoderImmFunct3ImmFunct3_u instruction_decoderhelper_itype_functiongroup = {0};
            instruction_decoderhelper_itype_functiongroup.funct3 = funct3;
            instruction_decoderhelper_itype_functiongroup.imm = imm;

            detectedUnknownInstruction = -1;
            switch (instruction_decoderhelper_itype_functiongroup.immfunct3) {
                case IMM11_0_FUNCT3_IMMEDIATE_CLZ:
                case IMM11_0_FUNCT3_IMMEDIATE_CTZ:
                case IMM11_0_FUNCT3_IMMEDIATE_CPOP:
                case IMM11_0_FUNCT3_IMMEDIATE_SEXTB:
                case IMM11_0_FUNCT3_IMMEDIATE_SEXTH:
                    RiscvEmulatorUnaryOperation(state, rdnum, rd, rs1num, rs1, instruction_decoderhelper_itype_functiongroup.immfunct3);
                    break;
                case IMM11_0_FUNCT3_IMMEDIATE_ORCB:
                    RiscvEmulatorORC_B(state, rdnum, rd, rs1num, rs1);
                    break;
                case IMM11_0_FUNCT3_IMMEDIATE_REV8:
                    RiscvEmulatorREV8(state, rdnum, rd, rs1num, rs1);
                    break;
#if (RVE_E_ZBKB == 1)
                case IMM11_0_FUNCT3_IMMEDIATE_BREV8:
                    RiscvEmulatorBREV8(state, rdnum, rd, rs1num, rs1);
                    break;
#endif
                default:
                    detectedUnknownInstruction = 1;
                    break;
            }
        }
    }
#endif

    if (detectedUnknownInstruction == 1) {
        // If funct3 == 0b001 or 0b101 then a whole set of functions are encoded in parts of imm.
        if (funct3 == FUNCT3_IMMEDIATE_FUNCTIONS_1 ||
            funct3 == FUNCT3_IMMEDIATE_FUNCTIONS_5) {

            uint8_t shamt = state->instruction.itypeshiftbyconstant.shamt;

            RiscvInstructionTypeIDecoderImm11_7Funct3Imm11_7Funct3_u instruction_decoderhelper_itype_functions_shamt = {0};

            instruction_decoderhelper_itype_functions_shamt.funct3 = funct3;
            instruction_decoderhelper_itype_functions_shamt.imm11_5 = state->instruction.itypeshiftbyconstant.imm11_5;

            detectedUnknownInstruction = -1;
            switch (instruction_decoderhelper_itype_functions_shamt.imm11_5funct3) {
                case IMM11_5_FUNCT3_IMMEDIATE_SLLI:
                case IMM11_5_FUNCT3_IMMEDIATE_SRLI:
                case IMM11_5_FUNCT3_IMMEDIATE_SRAI:
                    RiscvEmulatorShiftOperationImmediate(state, rdnum, rd, rs1num, rs1, shamt, instruction_decoderhelper_itype_functions_shamt.imm11_5funct3);
                    break;
#if (RVE_E_ZBB == 1)
                case IMM11_5_FUNCT3_IMMEDIATE_RORI:
                    RiscvEmulatorRORI(state, rdnum, rd, rs1num, rs1, shamt);
                    break;
#endif
#if (RVE_E_ZBKB == 1)
                case IMM11_5_FUNCT3_IMMEDIATE_ZIP:
                    RiscvEmulatorZIP(state, rdnum, rd, rs1num, rs1);
                    break;
                case IMM11_5_FUNCT3_IMMEDIATE_UNZIP:
                    RiscvEmulatorUNZIP(state, rdnum, rd, rs1num, rs1);
                    break;
#endif
#if (RVE_E_ZBS == 1)
                case IMM11_5_FUNCT3_IMMEDIATE_BCLRI:
                case IMM11_5_FUNCT3_IMMEDIATE_BEXTI:
                case IMM11_5_FUNCT3_IMMEDIATE_BINVI:
                case IMM11_5_FUNCT3_IMMEDIATE_BSETI:
                    RiscvEmulatorBitOperationImmediate(state, rdnum, rd, rs1num, rs1, shamt, instruction_decoderhelper_itype_functions_shamt.imm11_5funct3);
                    break;
#endif
                default:
                    detectedUnknownInstruction = 1;
                    break;
            }
        }
    }

    if (detectedUnknownInstruction == 1) {
        detectedUnknownInstruction = -1;
        switch (funct3) {
            case FUNCT3_IMMEDIATE_ADDI:
            case FUNCT3_IMMEDIATE_SLTI:
            case FUNCT3_IMMEDIATE_SLTIU:
            case FUNCT3_IMMEDIATE_XORI:
            case FUNCT3_IMMEDIATE_ORI:
            case FUNCT3_IMMEDIATE_ANDI:
                if (RiscvEmulatorOperation(state, rdnum, rd, rs1num, rs1, 0, NULL, 1, (uint32_t)imm, funct3) == 0) {
                    detectedUnknownInstruction = 1;
                }
                break;
            default:
                detectedUnknownInstruction = 1;
                break;
        }
    }

    if (detectedUnknownInstruction == 1) {
        state->trapflag.illegalinstruction = 1;
    }
}

/**
 * Process load opcodes.
 */
static inline void RiscvEmulatorOpcodeLoad(RiscvEmulatorState_t *state, uint8_t rdnum, void *rd, uint8_t rs1num, void *rs1, int16_t imm, uint8_t funct3) {
    uint32_t memorylocation = (uint32_t)imm + *(const uint32_t *)rs1;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "unknown";
#endif

    uint8_t length = 0;
    switch (funct3) {
        case FUNCT3_LOAD_LB:
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
            hc.instruction = "lb";
#endif
            length = sizeof(uint8_t);
            break;
        case FUNCT3_LOAD_LBU:
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
            hc.instruction = "lbu";
#endif
            length = sizeof(uint8_t);
            break;
        case FUNCT3_LOAD_LH:
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
            hc.instruction = "lh";
#endif
            length = sizeof(uint16_t);
            break;
        case FUNCT3_LOAD_LHU:
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
            hc.instruction = "lhu";
#endif
            length = sizeof(uint16_t);
            break;
        case FUNCT3_LOAD_LW:
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
            hc.instruction = "lw";
#endif
            length = sizeof(uint32_t);
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }

#if (RVE_E_ZICSR == 1) && (RVE_E_MISALIGNED == 0)
    // Check if the load is aligned.
    if (length > 1) {
        // Only the last few bits need to be checked.
        if ((memorylocation & (length - 1)) != 0) {
            state->trapflag.loadaddressmisaligned = 1;
            state->csr.mtval = memorylocation;
        }
    }
#endif

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.imm = (uint32_t)imm;
    hc.immlength = (sizeof(imm));
    hc.immname = "offset";
    hc.memorylocation = memorylocation;
    hc.length = length;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintLoad(state, &hc);
#endif
#endif

    if (rdnum == 0) {
        return;
    }

#if (RVE_E_ZICSR == 1) && (RVE_E_MISALIGNED == 0)
    if (state->trapflag.loadaddressmisaligned == 1) {
        return;
    }
#endif

    uint32_t value = 0;
    RiscvEmulatorLoad(memorylocation, &value, length);

    switch (funct3) {
        case FUNCT3_LOAD_LB:
            *(int32_t *)rd = (int8_t)value;
            break;
        case FUNCT3_LOAD_LBU:
            *(uint32_t *)rd = (uint8_t)value;
            break;
        case FUNCT3_LOAD_LH:
            *(int32_t *)rd = (int16_t)value;
            break;
        case FUNCT3_LOAD_LHU:
            *(uint32_t *)rd = (uint16_t)value;
            break;
        case FUNCT3_LOAD_LW:
            *(uint32_t *)rd = (uint32_t)value;
            break;
    }

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintLoad(state, &hc);
#endif
#endif
}

/**
 * Process store opcodes.
 */
static inline void RiscvEmulatorOpcodeStore(RiscvEmulatorState_t *state, uint8_t rs1num, void *rs1, uint8_t rs2num, void *rs2, uint8_t funct3) {
    RiscvInstructionTypeSDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm4_0 = state->instruction.stype.imm4_0;
    immdecoder.bit.imm11_5 = state->instruction.stype.imm11_5;
    int16_t offset = immdecoder.imm;

    uint32_t memorylocation = (uint32_t)offset + *(const uint32_t *)rs1;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "unkown";
#endif

    uint8_t length = 0;
    switch (funct3) {
        case FUNCT3_STORE_SW:
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
            hc.instruction = "sw";
#endif
            length = sizeof(uint32_t);
            break;
        case FUNCT3_STORE_SH:
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
            hc.instruction = "sh";
#endif
            length = sizeof(uint16_t);
            break;
        case FUNCT3_STORE_SB:
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
            hc.instruction = "sb";
#endif
            length = sizeof(uint8_t);
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }

#if (RVE_E_ZICSR == 1) && (RVE_E_MISALIGNED == 0)
    // Check if the store is aligned.
    if (length > 1) {
        // Only the last few bits need to be checked.
        if ((memorylocation & (length - 1)) != 0) {
            state->trapflag.storeaddressmisaligned = 1;
            state->csr.mtval = memorylocation;
        }
    }
#endif

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    hc.hook = HOOK_BEGIN;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2name = RiscvEmulatorGetRegisterSymbolicName(rs2num);
    hc.rs2 = rs2;
    hc.imm = (uint32_t)offset;
    hc.immname = "offset";
    hc.immlength = sizeof(offset);
    hc.immissigned = 1;
    hc.memorylocation = memorylocation;
    hc.length = length;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintStore(state, &hc);
#endif
#endif

#if (RVE_E_ZICSR == 1) && (RVE_E_MISALIGNED == 0)
    if (state->trapflag.storeaddressmisaligned == 1) {
        return;
    }
#endif

    RiscvEmulatorStore(memorylocation, rs2, length);

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintStore(state, &hc);
#endif
#endif
}

/**
 * Process branch opcodes.
 */
static inline void RiscvEmulatorOpcodeBranch(RiscvEmulatorState_t *state, uint8_t rs1num, void *rs1, uint8_t rs2num, void *rs2, uint8_t funct3) {
    RiscvInstructionTypeBDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm4_1 = state->instruction.btype.imm4_1;
    immdecoder.bit.imm10_5 = state->instruction.btype.imm10_5;
    immdecoder.bit.imm11 = state->instruction.btype.imm11;
    immdecoder.bit.imm12 = state->instruction.btype.imm12;
    int16_t imm = immdecoder.imm;

    uint8_t executebranch = BRANCH_NO;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    RiscvEmulatorHookContext_t hc = {0};
#else
    RiscvEmulatorHookContext_t hc __attribute__((unused));
#endif

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    switch (funct3) {
        case FUNCT3_BRANCH_BEQ:
            // Pseudoinstruction: beqz (beq rs1, x0, offset).
            if (rs2num == 0) {
                hc.instruction = "beqz";
            } else {
                hc.instruction = "beq";
            }
            break;
        case FUNCT3_BRANCH_BNE:
            // Pseudoinstruction: bnez (bne rs1, x0, offset).
            if (rs2num == 0) {
                hc.instruction = "bnez";
            } else {
                hc.instruction = "bne";
            }
            break;
        case FUNCT3_BRANCH_BGE:
            // Pseudoinstructions: bgez (bge rs1, x0, offset) and blez (bge x0, rs2, offset).
            if (rs2num == 0) {
                hc.instruction = "bgez";
            } else if (rs1num == 0) {
                hc.instruction = "blez";
            } else {
                hc.instruction = "bge";
            }
            break;
        case FUNCT3_BRANCH_BGEU:
            hc.instruction = "bgeu";
            break;
        case FUNCT3_BRANCH_BLT:
            // Pseudoinstructions: bltz (blt rs1, x0, offset) and bgtz (blt x0, rs2, offset).
            if (rs2num == 0) {
                hc.instruction = "bltz";
            } else if (rs1num == 0) {
                hc.instruction = "bgtz";
            } else {
                hc.instruction = "blt";
            }
            break;
        case FUNCT3_BRANCH_BLTU:
            hc.instruction = "bltu";
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }
    hc.hook = HOOK_BEGIN;
    hc.rs1num = rs1num;
    hc.rs1name = RiscvEmulatorGetRegisterSymbolicName(rs1num);
    hc.rs1 = rs1;
    hc.rs2num = rs2num;
    hc.rs2name = RiscvEmulatorGetRegisterSymbolicName(rs2num);
    hc.rs2 = rs2;
    hc.imm = (uint32_t)imm;
    hc.immissigned = 1;
    hc.immlength = sizeof(imm);
    hc.immname = "offset";
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintBranch(state, &hc);
#endif
#endif

    switch (funct3) {
        case FUNCT3_BRANCH_BEQ:
            if (*(const int32_t *)rs1 == *(const int32_t *)rs2) {
                executebranch = BRANCH_YES;
            }
            break;
        case FUNCT3_BRANCH_BNE:
            if (*(const int32_t *)rs1 != *(const int32_t *)rs2) {
                executebranch = BRANCH_YES;
            }
            break;
        case FUNCT3_BRANCH_BGE:
            if (*(const int32_t *)rs1 >= *(const int32_t *)rs2) {
                executebranch = BRANCH_YES;
            }
            break;
        case FUNCT3_BRANCH_BGEU:
            if (*(const uint32_t *)rs1 >= *(const uint32_t *)rs2) {
                executebranch = BRANCH_YES;
            }
            break;
        case FUNCT3_BRANCH_BLT:
            if (*(const int32_t *)rs1 < *(const int32_t *)rs2) {
                executebranch = BRANCH_YES;
            }
            break;
        case FUNCT3_BRANCH_BLTU:
            if (*(const uint32_t *)rs1 < *(const uint32_t *)rs2) {
                executebranch = BRANCH_YES;
            }
            break;
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }

    if (executebranch == BRANCH_YES) {
        uint32_t immu = (uint32_t)imm;
        state->programcounternext = state->programcounter + immu;

#if (RVE_E_ZICSR == 1) && (RVE_E_C != 1)
        // Check if programcounternext is aligned.
        if ((state->programcounternext & 0b11) != 0) {
            state->trapflag.instructionaddressmisaligned = 1;
            state->csr.mtval = state->programcounternext;
        }
#endif

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
        hc.hook = HOOK_END;
        RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
        RiscvEmulatorDisasmPrintBranch(state, &hc);
#endif
#endif
    }
}

/**
 * Add upper immediate to program counter.
 * rd = pc + imm
 */
static inline void RiscvEmulatorAUIPC(RiscvEmulatorState_t *state, uint8_t rdnum, void *rd __attribute__((unused))) {
    RiscvInstructionTypeUDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm31_12 = state->instruction.utype.imm31_12;
    uint32_t imm = immdecoder.imm;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "auipc";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.upperimmediate = state->instruction.utype.imm31_12;
    hc.imm = imm;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintUType(state, &hc);
#endif
#endif

    if (rdnum != 0) {
        state->reg.x[rdnum] = state->programcounter + imm;
    }

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintUType(state, &hc);
#endif
#endif
}

/**
 * Load upper immediate.
 * rd = imm
 */
static inline void RiscvEmulatorLUI(RiscvEmulatorState_t *state, uint8_t rdnum, void *rd) {
    RiscvInstructionTypeUDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm31_12 = state->instruction.utype.imm31_12;
    uint32_t imm = immdecoder.imm;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "lui";
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.imm = imm;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintUType(state, &hc);
#endif
#endif

    if (rdnum != 0) {
        *(uint32_t *)rd = imm;
    }

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintUType(state, &hc);
#endif
#endif
}

/**
 * Jump and link: jump to the program counter offset by the immediate and store the return address in rd.
 * rd = pc + 4; pc += offset
 */
static inline void RiscvEmulatorJAL(RiscvEmulatorState_t *state, uint8_t rdnum, void *rd) {
    RiscvInstructionTypeJDecoderImm_u immdecoder = {0};
    immdecoder.bit.imm10_1 = state->instruction.jtype.imm10_1;
    immdecoder.bit.imm11 = state->instruction.jtype.imm11;
    immdecoder.bit.imm19_12 = state->instruction.jtype.imm19_12;
    immdecoder.bit.imm20 = state->instruction.jtype.imm20;
    int32_t imm = immdecoder.imm;

    uint32_t immu = (uint32_t)imm;
    uint32_t jumptoprogramcounter = state->programcounter + immu;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    // Pseudoinstruction: j (jal x0, offset).
    if (rdnum == 0) {
        hc.instruction = "j";
    } else {
        hc.instruction = "jal";
    }
    hc.hook = HOOK_BEGIN;
    hc.rdnum = rdnum;
    hc.rdname = RiscvEmulatorGetRegisterSymbolicName(rdnum);
    hc.rd = rd;
    hc.imm = (uint32_t)imm;
    hc.immissigned = 1;
    hc.immname = "offset";
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintJType(state, &hc);
#endif
#endif
#if (RVE_E_ZICSR == 1) && (RVE_E_C != 1)
    // Check if jumptoprogramcounter is aligned.
    if ((jumptoprogramcounter & 0b11) != 0) {
        state->trapflag.instructionaddressmisaligned = 1;
        state->csr.mtval = jumptoprogramcounter;
        return;
    }
#endif

    // Set destination register to current next instruction acting as a return address.
    if (rdnum != 0) {
        *(uint32_t *)rd = state->programcounternext;
    }

    // Execute jump.
    state->programcounternext = jumptoprogramcounter;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintJType(state, &hc);
#endif
#endif
}

/**
 * Environment call: make a service request to the execution environment.
 * raise environment-call-from-M-mode exception
 */
static inline void RiscvEmulatorECALL(RiscvEmulatorState_t *state) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "ecall";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
#endif
#endif

#if (RVE_E_ZICSR == 1)
    state->trapflag.environmentcallfrommmode = 1;
#endif

    RiscvEmulatorHandleECALL(state);
}

/**
 * Environment break: cause control to be transferred back to a debugging environment.
 * raise breakpoint exception
 */
static inline void RiscvEmulatorEBREAK(RiscvEmulatorState_t *state) {

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "ebreak";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
#endif
#endif

#if (RVE_E_ZICSR == 1)
    state->trapflag.breakpoint = 1;
    state->csr.mtval = state->programcounter;
#endif

    RiscvEmulatorHandleEBREAK(state);
}

/**
 * Process system opcodes.
 */
static inline void RiscvEmulatorOpcodeSystem(RiscvEmulatorState_t *state, uint8_t rdnum, void *rd __attribute__((unused)), uint8_t rs1num, void *rs1 __attribute__((unused)), uint8_t funct3) {
    uint16_t funct12 = state->instruction.itypesystem.funct12;

    int8_t detectedUnknownInstruction = 1;

    if (detectedUnknownInstruction == 1) {
        if (rdnum == 0 &&
            funct3 == 0 &&
            rs1num == 0) {
            detectedUnknownInstruction = -1;
            switch (funct12) {
#if (RVE_E_ZICSR == 1)
                case FUNCT12_MRET:
                    RiscvEmulatorMRET(state);
                    break;
#endif
#if (RVE_E_ZIHINTPAUSE == 1)
                case FUNCT12_PAUSE:
                    RiscvEmulatorPAUSE(state);
                    break;
#endif
                case FUNCT12_ECALL:
                    RiscvEmulatorECALL(state);
                    break;
                case FUNCT12_EBREAK:
                    RiscvEmulatorEBREAK(state);
                    break;
                default:
                    detectedUnknownInstruction = 1;
                    break;
            }
        }
    }

#if (RVE_E_ZIMOP == 1)
    if (detectedUnknownInstruction == 1) {
        // May-Be-Operation instructions are SYSTEM opcode instructions with
        // funct3 4 and a fixed bit pattern in the upper bits. The n field
        // selects the instruction variant but all variants behave the same.
        uint32_t instructionvalue = state->instruction.value;
        if ((instructionvalue & MASK_ZIMOP_MOPR) == PATTERN_ZIMOP_MOPR) {
            detectedUnknownInstruction = -1;
            RiscvEmulatorMOPR(state, rdnum, rd);
        } else if ((instructionvalue & MASK_ZIMOP_MOPRR) == PATTERN_ZIMOP_MOPRR) {
            detectedUnknownInstruction = -1;
            RiscvEmulatorMOPRR(state, rdnum, rd);
        }
    }
#endif

#if (RVE_E_ZICSR == 1)
    if (detectedUnknownInstruction == 1) {
        detectedUnknownInstruction = -1;

        uint16_t csrnum = funct12;
        void *csr = RiscvEmulatorGetCSRAddress(state, csrnum);

        if (state->trapflag.value > 0) {
            return;
        }

        // The immediate variants (csrrwi, csrrsi, csrrci) hold a zero-extended
        // 5-bit immediate in the rs1 field instead of a register number.
        const uint8_t imm = state->instruction.itypesystem.rs1;

        if (RiscvEmulatorCSR(state, rdnum, rd, rs1num, rs1, funct3, csr, csrnum, imm) == 0) {
            detectedUnknownInstruction = 1;
        }
    }

#endif

    if (detectedUnknownInstruction == 1) {
        state->trapflag.illegalinstruction = 1;
    }
}

/**
 * Fence: order device I/O and memory accesses as observed by other RISC-V harts and external devices.
 * This does nothing in this emulator because all memory access is always completely processed.
 */
static inline void RiscvEmulatorFence(
    RiscvEmulatorState_t *state __attribute__((unused))) {
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "fence";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
#endif
#endif
}

#if (RVE_E_ZIFENCEI == 1)
/**
 * Fence instruction: synchronize the instruction and data streams.
 * This does nothing in this emulator because all memory access is always completely processed.
 */
static inline void RiscvEmulatorFencei(
    RiscvEmulatorState_t *state __attribute__((unused))) {
#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = "fencei";
    hc.hook = HOOK_BEGIN;
    RiscvEmulatorHook(state, &hc);
#if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
#endif
#endif
}
#endif

/**
 * Process miscellaneous memory opcodes.
 */
static inline void RiscvEmulatorOpcodeMiscMem(RiscvEmulatorState_t *state, uint8_t funct3) {
    uint8_t detectedUnknownInstruction = 1;

    if (detectedUnknownInstruction) {
        if (funct3 == FUNCT3_FENCE) {
            detectedUnknownInstruction = 0;
            RiscvEmulatorFence(state);
        }
    }

#if (RVE_E_ZIFENCEI == 1)
    if (detectedUnknownInstruction) {
        if (funct3 == FUNCT3_FENCEI) {
            detectedUnknownInstruction = 0;
            RiscvEmulatorFencei(state);
        }
    }
#endif

    if (detectedUnknownInstruction == 1) {
        state->trapflag.illegalinstruction = 1;
    }
}

#endif