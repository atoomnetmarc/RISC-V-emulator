/*

Copyright 2023 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorRv32I_H_
#define RiscvEmulatorRv32I_H_

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

// Jump and link register.
inline void RiscvEmulatorJALR(RiscvEmulatorState_t *state, uint32_t *programcounternext)
{
    uint32_t originalprogramcounternext = *programcounternext;

    // Execute jump.
    *programcounternext = state->registers.array.location[state->instruction.itype.rs1] + state->instruction.itype.imm;
    *programcounternext = *programcounternext & (UINT32_MAX - 1);

    // Set destination register to the original next instruction.
    if (state->instruction.itype.rd)
        state->registers.array.location[state->instruction.itype.rd] = originalprogramcounternext;
}

inline void RiscvEmulatorOpcodeJumpAndLinkRegister(RiscvEmulatorState_t *state, uint32_t *programcounternext)
{
    if (state->instruction.itype.funct3 == FUNCT3_JUMPANDLINKREGISTER_JALR)
        RiscvEmulatorJALR(state, programcounternext);
    else
        RiscvEmulatorUnknownInstruction(state);
}

// Add: rd = rs1 + rs2
inline void RiscvEmulatorADD(void *rd, const void *rs1, const void *rs2)
{
    *(int32_t *)rd = *(int32_t *)rs1 + *(int32_t *)rs2;
}

// Subtract: rd = rs1 - rs2
inline void RiscvEmulatorSUB(void *rd, const void *rs1, const void *rs2)
{
    *(int32_t *)rd = *(int32_t *)rs1 - *(int32_t *)rs2;
}

// Logical shift left: rd = rs1 << (rs2 & 0b11111)
inline void RiscvEmulatorSLL(void *rd, const void *rs1, const void *rs2)
{
    *(uint32_t *)rd = *(uint32_t *)rs1 << (*(uint32_t *)rs2 & 0b11111);
}

// Signed compare: rd = (rs1 < rs2)
inline void RiscvEmulatorSLT(void *rd, const void *rs1, const void *rs2)
{
    *(int32_t *)rd = (*(int32_t *)rs1 < *(int32_t *)rs2);
}

// Unsigned compare: rd = (rs1 < rs2)
inline void RiscvEmulatorSLTU(void *rd, const void *rs1, const void *rs2)
{
    *(uint32_t *)rd = (*(uint32_t *)rs1 < *(int32_t *)rs2);
}

// Exclusive or: rd = rs1 ^ rs2
inline void RiscvEmulatorXOR(void *rd, const void *rs1, const void *rs2)
{
    *(uint32_t *)rd = *(uint32_t *)rs1 ^ *(int32_t *)rs2;
}

// Logical shift right: rd = rs1 >> (rs2 & 0b11111)
inline void RiscvEmulatorSRL(void *rd, const void *rs1, const void *rs2)
{
    *(uint32_t *)rd = *(uint32_t *)rs1 >> (*(uint32_t *)rs2 & 0b11111);
}

// Arithmetic shift right: rd = rs1 >> (rs2 & 0b11111)
inline void RiscvEmulatorSRA(void *rd, const void *rs1, const void *rs2)
{
    *(int32_t *)rd = *(int32_t *)rs1 >> (*(uint32_t *)rs2 & 0b11111);
}

// Boolean or: rd = rs1 | rs2
inline void RiscvEmulatorOR(void *rd, const void *rs1, const void *rs2)
{
    *(int32_t *)rd = *(int32_t *)rs1 | *(int32_t *)rs2;
}

// Boolean and: rd = rs1 & rs2
inline void RiscvEmulatorAND(void *rd, const void *rs1, const void *rs2)
{
    *(int32_t *)rd = *(int32_t *)rs1 & *(int32_t *)rs2;
}

inline void RiscvEmulatorOpcodeOperation(RiscvEmulatorState_t *state)
{
    if (state->instruction.rtype.rd == 0)
        return;

    void *rd = &state->registers.array.location[state->instruction.rtype.rd];
    void *rs1 = &state->registers.array.location[state->instruction.rtype.rs1];
    void *rs2 = &state->registers.array.location[state->instruction.rtype.rs2];

    RiscvInstructionTypeRDecoderFunct7_3Funct7_3_u instruction_decoderhelper_rtype;
    instruction_decoderhelper_rtype.input.funct3 = state->instruction.rtype.funct3;
    instruction_decoderhelper_rtype.input.funct7 = state->instruction.rtype.funct7;

    switch (instruction_decoderhelper_rtype.output.funct7_3)
    {
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
        default:
            RiscvEmulatorUnknownInstruction(state);
            break;
    }
}

inline void RiscvEmulatorOpcodeImmediateShifts(RiscvEmulatorState_t *state, void *rd, void *rs1)
{
    uint32_t shamt = state->instruction.itypeshiftbyconstant.shamt;

    if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_SLLI &&
        state->instruction.itypeshiftbyconstant.imm11_5 == IMM11_7_IMMEDIATE_SLLI)
    {
        RiscvEmulatorSLL(rd, rs1, &shamt);
    }
    else if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_SR_I &&
             state->instruction.itypeshiftbyconstant.imm11_5 == IMM11_7_IMMEDIATE_SRLI)
    {
        RiscvEmulatorSRL(rd, rs1, &shamt);
    }
    else if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_SR_I &&
             state->instruction.itypeshiftbyconstant.imm11_5 == IMM11_7_IMMEDIATE_SRAI)
    {
        RiscvEmulatorSRA(rd, rs1, &shamt);
    }
    else
    {
        RiscvEmulatorUnknownInstruction(state);
    }
}

inline void RiscvEmulatorOpcodeImmediate(RiscvEmulatorState_t *state)
{
    if (state->instruction.itype.rd == 0)
        return;

    void *rd = &state->registers.array.location[state->instruction.itype.rd];
    void *rs1 = &state->registers.array.location[state->instruction.itype.rs1];

    if (state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_SLLI ||
        state->instruction.itype.funct3 == FUNCT3_IMMEDIATE_SR_I)
    {
        RiscvEmulatorOpcodeImmediateShifts(state, rd, rs1);
        return;
    }

    int32_t imm = state->instruction.itype.imm;

    switch (state->instruction.itype.funct3)
    {
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

// Load byte
inline void RiscvEmulatorLB(void *rd, uint32_t *memorylocation)
{
    int8_t int8 = 0;
    RiscvEmulatorLoad(*memorylocation, &int8, sizeof(int8_t));
    *(int32_t *)rd = int8;
}

// Load byte unsigned
inline void RiscvEmulatorLBU(void *rd, uint32_t *memorylocation)
{
    uint8_t uint8 = 0;
    RiscvEmulatorLoad(*memorylocation, &uint8, sizeof(uint8_t));
    *(uint32_t *)rd = uint8;
}

// Load half word
inline void RiscvEmulatorLH(void *rd, uint32_t *memorylocation)
{
    int16_t int16 = 0;
    RiscvEmulatorLoad(*memorylocation, &int16, sizeof(int16_t));
    *(int32_t *)rd = int16;
}

// Load half word unsigned
inline void RiscvEmulatorLHU(void *rd, uint32_t *memorylocation)
{
    uint16_t uint16 = 0;
    RiscvEmulatorLoad(*memorylocation, &uint16, sizeof(uint16_t));
    *(uint32_t *)rd = uint16;
}

// Load word
inline void RiscvEmulatorLW(void *rd, uint32_t *memorylocation)
{
    RiscvEmulatorLoad(*memorylocation, rd, sizeof(uint32_t));
}

inline void RiscvEmulatorOpcodeLoad(RiscvEmulatorState_t *state)
{
    if (state->instruction.itype.rd == 0)
        return;

    uint32_t memorylocation = state->instruction.itype.imm + state->registers.array.location[state->instruction.stype.rs1];
    void *rd = &state->registers.array.location[state->instruction.itype.rd];

    switch (state->instruction.itype.funct3)
    {
        case FUNCT3_LOAD_LB:
            RiscvEmulatorLB(rd, &memorylocation);
            break;
        case FUNCT3_LOAD_LBU:
            RiscvEmulatorLBU(rd, &memorylocation);
            break;
        case FUNCT3_LOAD_LH:
            RiscvEmulatorLH(rd, &memorylocation);
            break;
        case FUNCT3_LOAD_LHU:
            RiscvEmulatorLHU(rd, &memorylocation);
            break;
        case FUNCT3_LOAD_LW:
            RiscvEmulatorLW(rd, &memorylocation);
            break;
        default:
            RiscvEmulatorUnknownInstruction(state);
            break;
    }
}

inline void RiscvEmulatorOpcodeStore(RiscvEmulatorState_t *state)
{
    // Untangle the immediate bits.
    RiscvInstructionTypeSDecoderImm_u helper;
    helper.input.imm4_0 = state->instruction.stype.imm4_0;
    helper.input.imm11_5 = state->instruction.stype.imm11_5;

    uint32_t memorylocation = helper.output.imm + state->registers.array.location[state->instruction.stype.rs1];
    void *rs2 = &state->registers.array.location[state->instruction.stype.rs2];

    uint8_t length = 0;
    switch (state->instruction.stype.funct3)
    {
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

inline void RiscvEmulatorExecuteBranch(RiscvEmulatorState_t *state, uint32_t *programcounternext)
{
    // Untangle the immediate bits.
    RiscvInstructionTypeBDecoderImm_u helper;
    helper.input.imm0 = 0;
    helper.input.imm4_1 = state->instruction.btype.imm4_1;
    helper.input.imm10_5 = state->instruction.btype.imm10_5;
    helper.input.imm11 = state->instruction.btype.imm11;
    helper.input.imm12 = state->instruction.btype.imm12;

    *programcounternext = state->programcounter + helper.output.imm;
}

// Branch if equal
inline void RiscvEmulatorBEQ(const void *rs1, const void *rs2, uint8_t *executebranch)
{
    if (*(int32_t *)rs1 == *(int32_t *)rs2)
        *executebranch = BRANCH_YES;
}

// Branch if not equal
inline void RiscvEmulatorBNE(const void *rs1, const void *rs2, uint8_t *executebranch)
{
    if (*(int32_t *)rs1 != *(int32_t *)rs2)
        *executebranch = BRANCH_YES;
}

// Branch if greater than or equal
inline void RiscvEmulatorBGE(const void *rs1, const void *rs2, uint8_t *executebranch)
{
    if (*(int32_t *)rs1 >= *(int32_t *)rs2)
        *executebranch = BRANCH_YES;
}

// Branch if greater than or equal unsigned
inline void RiscvEmulatorBGEU(const void *rs1, const void *rs2, uint8_t *executebranch)
{
    if (*(uint32_t *)rs1 >= *(uint32_t *)rs2)
        *executebranch = BRANCH_YES;
}

// Branch if less than
inline void RiscvEmulatorBLT(const void *rs1, const void *rs2, uint8_t *executebranch)
{
    if (*(int32_t *)rs1 < *(int32_t *)rs2)
        *executebranch = BRANCH_YES;
}

// Branch if less than unsigned
inline void RiscvEmulatorBLTU(const void *rs1, const void *rs2, uint8_t *executebranch)
{
    if (*(uint32_t *)rs1 < *(uint32_t *)rs2)
        *executebranch = BRANCH_YES;
}

inline void RiscvEmulatorOpcodeBranch(RiscvEmulatorState_t *state, uint32_t *programcounternext)
{
    uint8_t executebranch = BRANCH_NO;
    void *rs1 = &state->registers.array.location[state->instruction.btype.rs1];
    void *rs2 = &state->registers.array.location[state->instruction.btype.rs2];

    switch (state->instruction.btype.funct3)
    {
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

    if (executebranch == BRANCH_YES)
        RiscvEmulatorExecuteBranch(state, programcounternext);
}

// Add upper immediate to program counter
inline void RiscvEmulatorAUIPC(RiscvEmulatorState_t *state)
{
    RiscvInstructionTypeUDecoderImm_u helper;
    helper.input.imm11_0 = 0;
    helper.input.imm31_12 = state->instruction.utype.imm31_12;

    state->registers.array.location[state->instruction.utype.rd] = state->programcounter + helper.output.imm;
}

// Load upper with immediate
inline void RiscvEmulatorLUI(RiscvEmulatorState_t *state)
{
    RiscvInstructionTypeUDecoderImm_u helper;
    helper.input.imm11_0 = 0;
    helper.input.imm31_12 = state->instruction.utype.imm31_12;

    state->registers.array.location[state->instruction.utype.rd] = helper.output.imm;
}

// Jump and link
inline void RiscvEmulatorJAL(RiscvEmulatorState_t *state, uint32_t *programcounternext)
{
    // Set destination register to current next instruction acting as a return address.
    if (state->instruction.jtype.rd)
        state->registers.array.location[state->instruction.jtype.rd] = *programcounternext;

    // Untangle the immediate bits.
    RiscvInstructionTypeJDecoderImm_u helper = {};
    helper.input.imm10_1 = state->instruction.jtype.imm10_1;
    helper.input.imm11 = state->instruction.jtype.imm11;
    helper.input.imm19_12 = state->instruction.jtype.imm19_12;
    helper.input.imm20 = state->instruction.jtype.imm20;

    // Execute jump.
    *programcounternext = state->programcounter + helper.output.imm;
}

#endif