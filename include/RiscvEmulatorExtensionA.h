/*

Copyright 2023 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorExtensionA_H_
#define RiscvEmulatorExtensionA_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_A == 1)

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

#include "RiscvEmulatorExtensionI.h"

// Swap rd and rs2
static inline void RiscvEmulatorAMOSWAP_W(void *rd, const void *rs2)
{
    int32_t temp = *(int32_t *)rd;

    *(int32_t *)rd = *(int32_t *)rs2;
    *(int32_t *)rs2 = temp;
}

// If rs2 < rd, set rd to rs2
static inline void RiscvEmulatorAMOMIN_W(void *rd, const void *rs2)
{
    if (*(int32_t *)rs2 < *(int32_t *)rd)
        *(int32_t *)rd = *(int32_t *)rs2;
}

// If rs2 > rd, set rd to rs2
static inline void RiscvEmulatorAMOMAX_W(void *rd, const void *rs2)
{
    if (*(int32_t *)rs2 > *(int32_t *)rd)
        *(int32_t *)rd = *(int32_t *)rs2;
}

// If rs2 < rd, set rd to rs2
static inline void RiscvEmulatorAMOMINU_W(void *rd, const void *rs2)
{
    if (*(uint32_t *)rs2 < *(uint32_t *)rd)
        *(uint32_t *)rd = *(uint32_t *)rs2;
}

// If rs2 > rd, set rd to rs2
static inline void RiscvEmulatorAMOMAXU_W(void *rd, const void *rs2)
{
    if (*(uint32_t *)rs2 > *(uint32_t *)rd)
        *(uint32_t *)rd = *(uint32_t *)rs2;
}

static inline void RiscvEmulatorOpcodeAtomicMemoryOperation(RiscvEmulatorState_t *state)
{
    void *rd = &state->registers.array.location[state->instruction.rtypeatomicmemoryoperation.rd];
    void *rs1 = &state->registers.array.location[state->instruction.rtypeatomicmemoryoperation.rs1];
    void *rs2 = &state->registers.array.location[state->instruction.rtypeatomicmemoryoperation.rs2];

    RiscvEmulatorLoad(*(uint32_t *)rs1, rd, sizeof(uint32_t));

    uint32_t temp = *(uint32_t *)rd;

    RiscvInstructionTypeRDecoderFunct5_3Funct5_3_u instruction_decoderhelper_rtypeatomicmemoryoperation;
    instruction_decoderhelper_rtypeatomicmemoryoperation.input.funct3 = state->instruction.rtypeatomicmemoryoperation.funct3;
    instruction_decoderhelper_rtypeatomicmemoryoperation.input.funct5 = state->instruction.rtypeatomicmemoryoperation.funct5;

    switch (instruction_decoderhelper_rtypeatomicmemoryoperation.output.funct5_3)
    {
        case FUNCT5_3_OPERATION_AMOADD_W:
            RiscvEmulatorADD(&temp, &temp, rs2);
            break;
        case FUNCT5_3_OPERATION_AMOSWAP_W:
            RiscvEmulatorAMOSWAP_W(&temp, rs2);
            break;
        case FUNCT5_3_OPERATION_AMOXOR_W:
            RiscvEmulatorXOR(&temp, &temp, rs2);
            break;
        case FUNCT5_3_OPERATION_AMOOR_W:
            RiscvEmulatorOR(&temp, &temp, rs2);
            break;
        case FUNCT5_3_OPERATION_AMOAND_W:
            RiscvEmulatorAND(&temp, &temp, rs2);
            break;
        case FUNCT5_3_OPERATION_AMOMIN_W:
            RiscvEmulatorAMOMIN_W(&temp, rs2);
            break;
        case FUNCT5_3_OPERATION_AMOMAX_W:
            RiscvEmulatorAMOMAX_W(&temp, rs2);
            break;
        case FUNCT5_3_OPERATION_AMOMINU_W:
            RiscvEmulatorAMOMINU_W(&temp, rs2);
            break;
        case FUNCT5_3_OPERATION_AMOMAXU_W:
            RiscvEmulatorAMOMAXU_W(&temp, rs2);
            break;
        default:
            RiscvEmulatorUnknownInstruction(state);
            break;
    }

    RiscvEmulatorStore(*(uint32_t *)rs1, &temp, sizeof(uint32_t));
}

#endif

#endif