/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorExtensionI_H_
#define RiscvEmulatorExtensionI_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

#include "RiscvEmulatorExtensionM.h"
#include "RiscvEmulatorExtensionZicsr.h"

/**
 * Jump and link register.
 */
static inline void RiscvEmulatorJALR(RiscvEmulatorState_t *state, uint32_t *programcounternext) {
    uint32_t originalprogramcounternext = *programcounternext;

    // Execute jump.
    *programcounternext = state->registers.array.location[state->instruction.itype.rs1] + state->instruction.itype.imm;
    *programcounternext = *programcounternext & (UINT32_MAX - 1);

    // Set destination register to the original next instruction.
    if (state->instruction.itype.rd) {
        state->registers.array.location[state->instruction.itype.rd] = originalprogramcounternext;
    }
}

/**
 * Process JALR opcode.
 */
static inline void RiscvEmulatorOpcodeJumpAndLinkRegister(RiscvEmulatorState_t *state, uint32_t *programcounternext) {
    if (state->instruction.itype.funct3 == FUNCT3_JUMPANDLINKREGISTER_JALR) {
        RiscvEmulatorJALR(state, programcounternext);
    } else {
        RiscvEmulatorUnknownInstruction(state);
    }
}

/**
 * Add: rd = rs1 + rs2
 */
static inline void RiscvEmulatorADD(void *rd, const void *rs1, const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rs1 + *(int32_t *)rs2;
}

/**
 * Subtract: rd = rs1 - rs2
 */
static inline void RiscvEmulatorSUB(void *rd, const void *rs1, const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rs1 - *(int32_t *)rs2;
}

/**
 * Logical shift left: rd = rs1 << (rs2 & 0b11111)
 */
static inline void RiscvEmulatorSLL(void *rd, const void *rs1, const void *rs2) {
    *(uint32_t *)rd = *(uint32_t *)rs1 << (*(uint32_t *)rs2 & 0b11111);
}

/**
 * Signed compare: rd = (rs1 < rs2)
 */
static inline void RiscvEmulatorSLT(void *rd, const void *rs1, const void *rs2) {
    *(int32_t *)rd = (*(int32_t *)rs1 < *(int32_t *)rs2);
}

/**
 * Unsigned compare: rd = (rs1 < rs2)
 */
static inline void RiscvEmulatorSLTU(void *rd, const void *rs1, const void *rs2) {
    *(uint32_t *)rd = (*(uint32_t *)rs1 < *(uint32_t *)rs2);
}

/**
 * Exclusive or: rd = rs1 ^ rs2
 */
static inline void RiscvEmulatorXOR(void *rd, const void *rs1, const void *rs2) {
    *(uint32_t *)rd = *(uint32_t *)rs1 ^ *(uint32_t *)rs2;
}

/**
 * Logical shift right: rd = rs1 >> (rs2 & 0b11111)
 */
static inline void RiscvEmulatorSRL(void *rd, const void *rs1, const void *rs2) {
    *(uint32_t *)rd = *(uint32_t *)rs1 >> (*(uint32_t *)rs2 & 0b11111);
}

/**
 * Arithmetic shift right: rd = rs1 >> (rs2 & 0b11111)
 */
static inline void RiscvEmulatorSRA(void *rd, const void *rs1, const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rs1 >> (*(uint32_t *)rs2 & 0b11111);
}

/**
 * Boolean or: rd = rs1 | rs2
 */
static inline void RiscvEmulatorOR(void *rd, const void *rs1, const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rs1 | *(int32_t *)rs2;
}

/**
 * Boolean and: rd = rs1 & rs2
 */
static inline void RiscvEmulatorAND(void *rd, const void *rs1, const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rs1 & *(int32_t *)rs2;
}

/**
 * Process operation opcodes.
 */
static inline void RiscvEmulatorOpcodeOperation(RiscvEmulatorState_t *state) {
    if (state->instruction.rtype.rd == 0) {
        return;
    }

    void *rd = &state->registers.array.location[state->instruction.rtype.rd];
    void *rs1 = &state->registers.array.location[state->instruction.rtype.rs1];
    void *rs2 = &state->registers.array.location[state->instruction.rtype.rs2];

    RiscvInstructionTypeRDecoderFunct7_3Funct7_3_u instruction_decoderhelper_rtype;
    instruction_decoderhelper_rtype.input.funct3 = state->instruction.rtype.funct3;
    instruction_decoderhelper_rtype.input.funct7 = state->instruction.rtype.funct7;

    switch (instruction_decoderhelper_rtype.output.funct7_3) {
        case FUNCT7_3_OPERATION_ADD:
            RiscvEmulatorADD(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_SUB:
            RiscvEmulatorSUB(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_SLL:
            RiscvEmulatorSLL(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_SLT:
            RiscvEmulatorSLT(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_SLTU:
            RiscvEmulatorSLTU(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_XOR:
            RiscvEmulatorXOR(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_SRL:
            RiscvEmulatorSRL(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_SRA:
            RiscvEmulatorSRA(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_OR:
            RiscvEmulatorOR(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_AND:
            RiscvEmulatorAND(rd, rs1, rs2);
            break;
#if (RVE_E_M == 1)
        case FUNCT7_3_OPERATION_MUL:
            RiscvEmulatorMUL(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_MULH:
            RiscvEmulatorMULH(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_MULHSU:
            RiscvEmulatorMULHSU(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_MULHU:
            RiscvEmulatorMULHU(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_DIV:
            RiscvEmulatorDIV(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_DIVU:
            RiscvEmulatorDIVU(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_REM:
            RiscvEmulatorREM(rd, rs1, rs2);
            break;
        case FUNCT7_3_OPERATION_REMU:
            RiscvEmulatorREMU(rd, rs1, rs2);
            break;
#endif
        default:
            RiscvEmulatorUnknownInstruction(state);
            break;
    }
}

/**
 * Process immediate shifts opcodes.
 */
static inline void RiscvEmulatorOpcodeImmediateShifts(RiscvEmulatorState_t *state, void *rd, void *rs1) {
    uint32_t shamt = state->instruction.itypeshiftbyconstant.shamt;

    if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_SLLI &&
        state->instruction.itypeshiftbyconstant.imm11_5 == IMM11_7_IMMEDIATE_SLLI) {
        RiscvEmulatorSLL(rd, rs1, &shamt);
    } else if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_SR_I &&
               state->instruction.itypeshiftbyconstant.imm11_5 == IMM11_7_IMMEDIATE_SRLI) {
        RiscvEmulatorSRL(rd, rs1, &shamt);
    } else if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_SR_I &&
               state->instruction.itypeshiftbyconstant.imm11_5 == IMM11_7_IMMEDIATE_SRAI) {
        RiscvEmulatorSRA(rd, rs1, &shamt);
    } else {
        RiscvEmulatorUnknownInstruction(state);
    }
}

/**
 * Process immediate opcodes.
 */
static inline void RiscvEmulatorOpcodeImmediate(RiscvEmulatorState_t *state) {
    if (state->instruction.itype.rd == 0) {
        return;
    }

    void *rd = &state->registers.array.location[state->instruction.itype.rd];
    void *rs1 = &state->registers.array.location[state->instruction.itype.rs1];

    if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_SLLI ||
        state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_SR_I) {
        RiscvEmulatorOpcodeImmediateShifts(state, rd, rs1);
        return;
    }

    int32_t imm = state->instruction.itype.imm;

    switch (state->instruction.itype.funct3) {
        case FUNCT3_IMMEDIATE_ADDI:
            RiscvEmulatorADD(rd, rs1, &imm);
            break;
        case FUNCT3_IMMEDIATE_SLTI:
            RiscvEmulatorSLT(rd, rs1, &imm);
            break;
        case FUNCT3_IMMEDIATE_SLTIU:
            RiscvEmulatorSLTU(rd, rs1, &imm);
            break;
        case FUNCT3_IMMEDIATE_XORI:
            RiscvEmulatorXOR(rd, rs1, &imm);
            break;
        case FUNCT3_IMMEDIATE_ORI:
            RiscvEmulatorOR(rd, rs1, &imm);
            break;
        case FUNCT3_IMMEDIATE_ANDI:
            RiscvEmulatorAND(rd, rs1, &imm);
            break;
        default:
            RiscvEmulatorUnknownInstruction(state);
            break;
    }
}

/**
 * Process load opcodes.
 */
static inline void RiscvEmulatorOpcodeLoad(RiscvEmulatorState_t *state) {
    if (state->instruction.itype.rd == 0) {
        return;
    }

    uint32_t memorylocation = state->instruction.itype.imm + state->registers.array.location[state->instruction.stype.rs1];
    void *rd = &state->registers.array.location[state->instruction.itype.rd];

    uint8_t length = 0;
    switch (state->instruction.itype.funct3) {
        case FUNCT3_LOAD_LB:
        case FUNCT3_LOAD_LBU:
            length = sizeof(uint8_t);
            break;
        case FUNCT3_LOAD_LH:
        case FUNCT3_LOAD_LHU:
            length = sizeof(uint16_t);
            break;
        case FUNCT3_LOAD_LW:
            length = sizeof(uint32_t);
            break;
        default:
            RiscvEmulatorUnknownInstruction(state);
            break;
    }

    uint32_t value = 0;
    RiscvEmulatorLoad(memorylocation, &value, length);

    switch (state->instruction.itype.funct3) {
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
}

/**
 * Process store opcodes.
 */
static inline void RiscvEmulatorOpcodeStore(RiscvEmulatorState_t *state) {
    // Untangle the immediate bits.
    RiscvInstructionTypeSDecoderImm_u helper;
    helper.input.imm4_0 = state->instruction.stype.imm4_0;
    helper.input.imm11_5 = state->instruction.stype.imm11_5;

    uint32_t memorylocation = helper.output.imm + state->registers.array.location[state->instruction.stype.rs1];
    void *rs2 = &state->registers.array.location[state->instruction.stype.rs2];

    uint8_t length = 0;
    switch (state->instruction.stype.funct3) {
        case FUNCT3_STORE_SW:
            length = sizeof(uint32_t);
            break;
        case FUNCT3_STORE_SH:
            length = sizeof(uint16_t);
            break;
        case FUNCT3_STORE_SB:
            length = sizeof(uint8_t);
            break;
        default:
            RiscvEmulatorUnknownInstruction(state);
            break;
    }

    RiscvEmulatorStore(memorylocation, rs2, length);
}

/**
 * Executes a branch.
 */
static inline void RiscvEmulatorExecuteBranch(RiscvEmulatorState_t *state, uint32_t *programcounternext) {
    // Untangle the immediate bits.
    RiscvInstructionTypeBDecoderImm_u helper;
    helper.input.imm0 = 0;
    helper.input.imm4_1 = state->instruction.btype.imm4_1;
    helper.input.imm10_5 = state->instruction.btype.imm10_5;
    helper.input.imm11 = state->instruction.btype.imm11;
    helper.input.imm12 = state->instruction.btype.imm12;

    *programcounternext = state->programcounter + helper.output.imm;
}

/**
 * Branch if equal
 */
static inline void RiscvEmulatorBEQ(const void *rs1, const void *rs2, uint8_t *executebranch) {
    if (*(int32_t *)rs1 == *(int32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Branch if not equal
 */
static inline void RiscvEmulatorBNE(const void *rs1, const void *rs2, uint8_t *executebranch) {
    if (*(int32_t *)rs1 != *(int32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Branch if greater than or equal
 */
static inline void RiscvEmulatorBGE(const void *rs1, const void *rs2, uint8_t *executebranch) {
    if (*(int32_t *)rs1 >= *(int32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Branch if greater than or equal unsigned
 */
static inline void RiscvEmulatorBGEU(const void *rs1, const void *rs2, uint8_t *executebranch) {
    if (*(uint32_t *)rs1 >= *(uint32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Branch if less than
 */
static inline void RiscvEmulatorBLT(const void *rs1, const void *rs2, uint8_t *executebranch) {
    if (*(int32_t *)rs1 < *(int32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Branch if less than unsigned
 */
static inline void RiscvEmulatorBLTU(const void *rs1, const void *rs2, uint8_t *executebranch) {
    if (*(uint32_t *)rs1 < *(uint32_t *)rs2) {
        *executebranch = BRANCH_YES;
    }
}

/**
 * Process branch opcodes.
 */
static inline void RiscvEmulatorOpcodeBranch(RiscvEmulatorState_t *state, uint32_t *programcounternext) {
    uint8_t executebranch = BRANCH_NO;
    void *rs1 = &state->registers.array.location[state->instruction.btype.rs1];
    void *rs2 = &state->registers.array.location[state->instruction.btype.rs2];

    switch (state->instruction.btype.funct3) {
        case FUNCT3_BRANCH_BEQ:
            RiscvEmulatorBEQ(rs1, rs2, &executebranch);
            break;
        case FUNCT3_BRANCH_BNE:
            RiscvEmulatorBNE(rs1, rs2, &executebranch);
            break;
        case FUNCT3_BRANCH_BGE:
            RiscvEmulatorBGE(rs1, rs2, &executebranch);
            break;
        case FUNCT3_BRANCH_BGEU:
            RiscvEmulatorBGEU(rs1, rs2, &executebranch);
            break;
        case FUNCT3_BRANCH_BLT:
            RiscvEmulatorBLT(rs1, rs2, &executebranch);
            break;
        case FUNCT3_BRANCH_BLTU:
            RiscvEmulatorBLTU(rs1, rs2, &executebranch);
            break;
        default:
            RiscvEmulatorUnknownInstruction(state);
            break;
    }

    if (executebranch == BRANCH_YES) {
        RiscvEmulatorExecuteBranch(state, programcounternext);
    }
}

/**
 * Add upper immediate to program counter
 */
static inline void RiscvEmulatorAUIPC(RiscvEmulatorState_t *state) {
    RiscvInstructionTypeUDecoderImm_u helper;
    helper.input.imm11_0 = 0;
    helper.input.imm31_12 = state->instruction.utype.imm31_12;

    state->registers.array.location[state->instruction.utype.rd] = state->programcounter + helper.output.imm;
}

/**
 * Load upper with immediate
 */
static inline void RiscvEmulatorLUI(RiscvEmulatorState_t *state) {
    RiscvInstructionTypeUDecoderImm_u helper;
    helper.input.imm11_0 = 0;
    helper.input.imm31_12 = state->instruction.utype.imm31_12;

    state->registers.array.location[state->instruction.utype.rd] = helper.output.imm;
}

/**
 * Jump and link
 */
static inline void RiscvEmulatorJAL(RiscvEmulatorState_t *state, uint32_t *programcounternext) {
    // Set destination register to current next instruction acting as a return address.
    if (state->instruction.jtype.rd) {
        state->registers.array.location[state->instruction.jtype.rd] = *programcounternext;
    }

    // Untangle the immediate bits.
    RiscvInstructionTypeJDecoderImm_u helper = {};
    helper.input.imm10_1 = state->instruction.jtype.imm10_1;
    helper.input.imm11 = state->instruction.jtype.imm11;
    helper.input.imm19_12 = state->instruction.jtype.imm19_12;
    helper.input.imm20 = state->instruction.jtype.imm20;

    // Execute jump.
    *programcounternext = state->programcounter + helper.output.imm;
}

/**
 * Make a service request to the execution environment
 */
static inline void RiscvEmulatorECALL(RiscvEmulatorState_t *state) {
    RiscvEmulatorHandleECALL(state);
}

/**
 * Process system opcodes.
 */
static inline void RiscvEmulatorOpcodeSystem(RiscvEmulatorState_t *state, uint32_t *programcounternext) {
    uint8_t detectedUnknownSystemInstruction = 1;

    if (detectedUnknownSystemInstruction) {
        if (state->instruction.itypesystem.rd == 0 &&
            state->instruction.itypesystem.funct3 == 0 &&
            state->instruction.itypesystem.rs1 == 0) {
            detectedUnknownSystemInstruction = 0;
            switch (state->instruction.itypesystem.funct12) {
#if (RVE_E_ZICSR == 1)
                case FUNCT12_MRET:
                    RiscvEmulatorMRET(state, programcounternext);
                    break;
#endif
                case FUNCT12_ECALL:
                    RiscvEmulatorECALL(state);
                    break;
                default:
                    detectedUnknownSystemInstruction = 1;
                    break;
            }
        }
    }

#if (RVE_E_ZICSR == 1)
    if (detectedUnknownSystemInstruction) {
        detectedUnknownSystemInstruction = 0;
        void *rd = &state->registers.array.location[state->instruction.itypecsr.rd];
        void *rs1 = &state->registers.array.location[state->instruction.itypecsr.rs1];
        int32_t imm = state->instruction.itypecsrimm.imm;
        void *csr = RiscvEmulatorGetCSRAddress(state);

        switch (state->instruction.itypecsr.funct3) {
            case FUNCT3_CSR_CSRRW:
                RiscvEmulatorCSRRW(state, rd, rs1, csr);
                break;
            case FUNCT3_CSR_CSRRWI:
                RiscvEmulatorCSRRW(state, rd, &imm, csr);
                break;
            case FUNCT3_CSR_CSRRS:
                RiscvEmulatorCSRRS(state, rd, rs1, csr);
                break;
            case FUNCT3_CSR_CSRRSI:
                RiscvEmulatorCSRRS(state, rd, &imm, csr);
                break;
            case FUNCT3_CSR_CSRRC:
                RiscvEmulatorCSRRC(state, rd, rs1, csr);
                break;
            case FUNCT3_CSR_CSRRCI:
                RiscvEmulatorCSRRC(state, rd, &imm, csr);
                break;
            default:
                detectedUnknownSystemInstruction = 1;
                break;
        }
    }

#endif

    if (detectedUnknownSystemInstruction) {
        RiscvEmulatorUnknownInstruction(state);
    }
}

/**
 * Excutes the fence instuction.
 *
 * This does nothing in this emulator because all memory access is always completely processed.
 */
static inline void RiscvEmulatorFence(RiscvEmulatorState_t *state) {
}

/**
 * Process miscellaneous memory opcodes.
 */
static inline void RiscvEmulatorOpcodeMiscMem(RiscvEmulatorState_t *state) {
    uint8_t detectedUnknownSystemInstruction = 1;

    if (detectedUnknownSystemInstruction) {
        if (state->instruction.itypemiscmem.rd == 0 &&
            state->instruction.itypemiscmem.funct3 == FUNCT3_FENCE &&
            state->instruction.itypemiscmem.rs1 == 0) {
            detectedUnknownSystemInstruction = 0;
            RiscvEmulatorFence(state);
        }
    }

    if (detectedUnknownSystemInstruction) {
        RiscvEmulatorUnknownInstruction(state);
    }
}

#endif