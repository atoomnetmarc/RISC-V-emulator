/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionA_H_
#define RiscvEmulatorExtensionA_H_

#include "RiscvEmulatorConfig.h"

#if ((RVE_E_ZAAMO == 1) || (RVE_E_ZALRSC == 1))

#include <stdint.h>

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorType.h"

#include "RiscvEmulatorExtensionI.h"

/**
 * Add rd and rs2.
 */
static inline void RiscvEmulatorAMOADD_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rd + *(int32_t *)rs2;
}

/**
 * Swap rd and rs2.
 */
static inline void RiscvEmulatorAMOSWAP_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rs2;
}

/**
 * XOR rd and rs2.
 */
static inline void RiscvEmulatorAMOXOR_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    *(uint32_t *)rd = *(uint32_t *)rd ^ *(uint32_t *)rs2;
}

/**
 * OR rd and rs2.
 */
static inline void RiscvEmulatorAMOOR_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rd | *(int32_t *)rs2;
}

/**
 * AND rd and rs2.
 */
static inline void RiscvEmulatorAMOAND_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rd & *(int32_t *)rs2;
}

/**
 * If rs2 < rd, set rd to rs2.
 */
static inline void RiscvEmulatorAMOMIN_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    if (*(int32_t *)rs2 < *(int32_t *)rd) {
        *(int32_t *)rd = *(int32_t *)rs2;
    }
}

/**
 * If rs2 > rd, set rd to rs2.
 */
static inline void RiscvEmulatorAMOMAX_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    if (*(int32_t *)rs2 > *(int32_t *)rd) {
        *(int32_t *)rd = *(int32_t *)rs2;
    }
}

/**
 * If rs2 < rd, set rd to rs2.
 */
static inline void RiscvEmulatorAMOMINU_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    if (*(uint32_t *)rs2 < *(uint32_t *)rd) {
        *(uint32_t *)rd = *(uint32_t *)rs2;
    }
}

/**
 * If rs2 > rd, set rd to rs2.
 */
static inline void RiscvEmulatorAMOMAXU_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    if (*(uint32_t *)rs2 > *(uint32_t *)rd) {
        *(uint32_t *)rd = *(uint32_t *)rs2;
    }
}

#if (RVE_E_ZALRSC == 1)
/**
 * Load-reserved: load a word and register a reservation.
 */
static inline void RiscvEmulatorLR_W(
    RiscvEmulatorState_t *state,
    void *rd,
    uint32_t address) {
    RiscvEmulatorLoad(address, rd, sizeof(uint32_t));
    state->reservationvalid = 1;
    state->reservationaddress = address;
}

/**
 * Store-conditional: store a word only if the reservation is still valid.
 */
static inline void RiscvEmulatorSC_W(
    RiscvEmulatorState_t *state,
    void *rd,
    const void *rs2,
    uint32_t address) {
    if (state->reservationvalid == 1 && state->reservationaddress == address) {
        RiscvEmulatorStore(address, rs2, sizeof(uint32_t));
        *(uint32_t *)rd = 0; // Success.
    } else {
        *(uint32_t *)rd = 1; // Failure code 1: unspecified failure.
    }
    // Regardless of success or failure, executing an SC invalidates any
    // reservation held by this hart.
    state->reservationvalid = 0;
}
#endif

/**
 * Process atomic memory operation opcodes.
 */
static inline void RiscvEmulatorOpcodeAtomicMemoryOperation(
    RiscvEmulatorState_t *state) {
    uint8_t rdnum = state->instruction.rtypeatomicmemoryoperation.rd;
    void *rd = &state->reg.x[rdnum];
    uint8_t rs1num = state->instruction.rtypeatomicmemoryoperation.rs1;
    void *rs1 = &state->reg.x[rs1num];
    uint8_t rs2num = state->instruction.rtypeatomicmemoryoperation.rs2;
    void *rs2 = &state->reg.x[rs2num];

    // Remember original address stored in rs1.
    uint32_t originaladdressrs1 = *(uint32_t *)rs1;

    // Remember original value stored in rs2.
    uint32_t originalvaluers2 = *(uint32_t *)rs2;

#if (RVE_E_ZAAMO == 1)
    uint32_t loadedvalue = 0;
    RiscvEmulatorLoad(originaladdressrs1, &loadedvalue, sizeof(uint32_t));

    if (rdnum != 0) {
        // Place loaded value of original address in rd.
        *(uint32_t *)rd = loadedvalue;
    }
#endif

    RiscvInstructionTypeRDecoderFunct5Funct3_u instruction_decoderhelper_rtypeatomicmemoryoperation = {0};
    instruction_decoderhelper_rtypeatomicmemoryoperation.funct3 = state->instruction.rtypeatomicmemoryoperation.funct3;
    instruction_decoderhelper_rtypeatomicmemoryoperation.funct5 = state->instruction.rtypeatomicmemoryoperation.funct5;

    switch (instruction_decoderhelper_rtypeatomicmemoryoperation.funct5_3) {
#if (RVE_E_ZALRSC == 1)
        case FUNCT5_FUNCT3_OPERATION_LR_W:
            if (rdnum != 0) {
                RiscvEmulatorLR_W(state, rd, originaladdressrs1);
            } else {
                // Still register the reservation when writing to x0.
                state->reservationvalid = 1;
                state->reservationaddress = originaladdressrs1;
            }
            return;
        case FUNCT5_FUNCT3_OPERATION_SC_W:
            if (rdnum != 0) {
                RiscvEmulatorSC_W(state, rd, &originalvaluers2, originaladdressrs1);
            } else {
                // Discard the result but still perform the conditional store.
                uint32_t result = 0;
                RiscvEmulatorSC_W(state, &result, &originalvaluers2, originaladdressrs1);
            }
            return;
#endif
#if (RVE_E_ZAAMO == 1)
        case FUNCT5_FUNCT3_OPERATION_AMOADD_W:
            RiscvEmulatorAMOADD_W(state, &loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOSWAP_W:
            RiscvEmulatorAMOSWAP_W(state, &loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOXOR_W:
            RiscvEmulatorAMOXOR_W(state, &loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOOR_W:
            RiscvEmulatorAMOOR_W(state, &loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOAND_W:
            RiscvEmulatorAMOAND_W(state, &loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOMIN_W:
            RiscvEmulatorAMOMIN_W(state, &loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOMAX_W:
            RiscvEmulatorAMOMAX_W(state, &loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOMINU_W:
            RiscvEmulatorAMOMINU_W(state, &loadedvalue, &originalvaluers2);
            break;
        case FUNCT5_FUNCT3_OPERATION_AMOMAXU_W:
            RiscvEmulatorAMOMAXU_W(state, &loadedvalue, &originalvaluers2);
            break;
#endif
        default:
            state->trapflag.illegalinstruction = 1;
            return;
    }

#if (RVE_E_ZAAMO == 1)
    RiscvEmulatorStore(originaladdressrs1, &loadedvalue, sizeof(uint32_t));
#endif
}

#endif

#endif