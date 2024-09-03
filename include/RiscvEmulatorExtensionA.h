/*

Copyright 2023-2024 Marc Ketel
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

/**
 * Add rd and rs2.
 */
static inline void RiscvEmulatorAMOADD_W(void *rd, const void *rs2) {
    int32_t temp = *(int32_t *)rd;

    *(int32_t *)rd = *(int32_t *)rd + *(int32_t *)rs2;
    *(int32_t *)rs2 = temp;
}

/**
 * Swap rd and rs2.
 */
static inline void RiscvEmulatorAMOSWAP_W(void *rd, const void *rs2) {
    int32_t temp = *(int32_t *)rd;

    *(int32_t *)rd = *(int32_t *)rs2;
    *(int32_t *)rs2 = temp;
}

/**
 * XOR rd and rs2.
 */
static inline void RiscvEmulatorAMOXOR_W(void *rd, const void *rs2) {
    int32_t temp = *(int32_t *)rd;

    *(uint32_t *)rd = *(uint32_t *)rd ^ *(uint32_t *)rs2;
    *(int32_t *)rs2 = temp;
}

/**
 * OR rd and rs2.
 */
static inline void RiscvEmulatorAMOOR_W(void *rd, const void *rs2) {
    int32_t temp = *(int32_t *)rd;

    *(int32_t *)rd = *(int32_t *)rd | *(int32_t *)rs2;
    *(int32_t *)rs2 = temp;
}

/**
 * AND rd and rs2.
 */
static inline void RiscvEmulatorAMOAND_W(void *rd, const void *rs2) {
    int32_t temp = *(int32_t *)rd;

    *(int32_t *)rd = *(int32_t *)rd & *(int32_t *)rs2;
    *(int32_t *)rs2 = temp;
}

/**
 * If rs2 < rd, set rd to rs2.
 */
static inline void RiscvEmulatorAMOMIN_W(void *rd, const void *rs2) {
    if (*(int32_t *)rs2 < *(int32_t *)rd) {
        *(int32_t *)rd = *(int32_t *)rs2;
    }
}

/**
 * If rs2 > rd, set rd to rs2.
 */
static inline void RiscvEmulatorAMOMAX_W(void *rd, const void *rs2) {
    if (*(int32_t *)rs2 > *(int32_t *)rd) {
        *(int32_t *)rd = *(int32_t *)rs2;
    }
}

/**
 * If rs2 < rd, set rd to rs2.
 */
static inline void RiscvEmulatorAMOMINU_W(void *rd, const void *rs2) {
    if (*(uint32_t *)rs2 < *(uint32_t *)rd) {
        *(uint32_t *)rd = *(uint32_t *)rs2;
    }
}

/**
 * If rs2 > rd, set rd to rs2.
 */
static inline void RiscvEmulatorAMOMAXU_W(void *rd, const void *rs2) {
    if (*(uint32_t *)rs2 > *(uint32_t *)rd) {
        *(uint32_t *)rd = *(uint32_t *)rs2;
    }
}

/**
 * Process atomic memory operation opcodes.
 */
static inline void RiscvEmulatorOpcodeAtomicMemoryOperation(RiscvEmulatorState_t *state) {
    uint8_t rdnum = state->instruction.rtypeatomicmemoryoperation.rd;
    void *rd = &state->registers.array.location[rdnum];
    uint8_t rs1num = state->instruction.rtypeatomicmemoryoperation.rs1;
    void *rs1 = &state->registers.array.location[rs1num];
    uint8_t rs2num = state->instruction.rtypeatomicmemoryoperation.rs2;
    void *rs2 = &state->registers.array.location[rs2num];

    // Remember original address stored in rs1.
    uint32_t originaladdressrs1 = *(uint32_t *)rs1;

    // Remember original value stored in rs2.
    uint32_t originalvaluers2 = *(uint32_t *)rs2;

    uint32_t loadedvalue = 0;
    RiscvEmulatorLoad(originaladdressrs1, &loadedvalue, sizeof(uint32_t));

    if (state->instruction.rtypeatomicmemoryoperation.rd != 0) {
        // Place loaded value of original address in rd.
        *(uint32_t *)rd = loadedvalue;
    }

    RiscvInstructionTypeRDecoderFunct5Funct3_u instruction_decoderhelper_rtypeatomicmemoryoperation;
    instruction_decoderhelper_rtypeatomicmemoryoperation.input.funct3 = state->instruction.rtypeatomicmemoryoperation.funct3;
    instruction_decoderhelper_rtypeatomicmemoryoperation.input.funct5 = state->instruction.rtypeatomicmemoryoperation.funct5;

    switch (instruction_decoderhelper_rtypeatomicmemoryoperation.output.funct5_3) {
        case FUNCT5_FUNCT3_OPERATION_AMOADD_W:
            RiscvEmulatorAMOADD_W(&loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOSWAP_W:
            RiscvEmulatorAMOSWAP_W(&loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOXOR_W:
            RiscvEmulatorAMOXOR_W(&loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOOR_W:
            RiscvEmulatorAMOOR_W(&loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOAND_W:
            RiscvEmulatorAMOAND_W(&loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOMIN_W:
            RiscvEmulatorAMOMIN_W(&loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOMAX_W:
            RiscvEmulatorAMOMAX_W(&loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOMINU_W:
            RiscvEmulatorAMOMINU_W(&loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOMAXU_W:
            RiscvEmulatorAMOMAXU_W(&loadedvalue, &originalvaluers2);
            break;
        default:
            state->trapflags.bits.illegalinstruction = 1;
            return;
    }

    RiscvEmulatorStore(originaladdressrs1, &loadedvalue, sizeof(uint32_t));
}

#endif

#endif