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
 * Atomic add: atomically load the word in rd and write back the sum of rd and rs2.
 * rd = rd + rs2
 */
static inline void RiscvEmulatorAMOADD_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rd + *(const int32_t *)rs2;
}

/**
 * Atomic swap: atomically load the word in rd and write back rs2.
 * rd = rs2
 */
static inline void RiscvEmulatorAMOSWAP_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    *(int32_t *)rd = *(const int32_t *)rs2;
}

/**
 * Atomic xor: atomically load the word in rd and write back the xor of rd and rs2.
 * rd = rd ^ rs2
 */
static inline void RiscvEmulatorAMOXOR_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    *(uint32_t *)rd = *(uint32_t *)rd ^ *(const uint32_t *)rs2;
}

/**
 * Atomic or: atomically load the word in rd and write back the or of rd and rs2.
 * rd = rd | rs2
 */
static inline void RiscvEmulatorAMOOR_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rd | *(const int32_t *)rs2;
}

/**
 * Atomic and: atomically load the word in rd and write back the and of rd and rs2.
 * rd = rd & rs2
 */
static inline void RiscvEmulatorAMOAND_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    *(int32_t *)rd = *(int32_t *)rd & *(const int32_t *)rs2;
}

/**
 * Signed minimum: atomically load the word in rd and write back the smaller of rd and rs2.
 * rd = (rs2 < rd) ? rs2 : rd
 */
static inline void RiscvEmulatorAMOMIN_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    if (*(const int32_t *)rs2 < *(int32_t *)rd) {
        *(int32_t *)rd = *(const int32_t *)rs2;
    }
}

/**
 * Signed maximum: atomically load the word in rd and write back the larger of rd and rs2.
 * rd = (rs2 > rd) ? rs2 : rd
 */
static inline void RiscvEmulatorAMOMAX_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    if (*(const int32_t *)rs2 > *(int32_t *)rd) {
        *(int32_t *)rd = *(const int32_t *)rs2;
    }
}

/**
 * Unsigned minimum: atomically load the word in rd and write back the smaller of rd and rs2 as unsigned integers.
 * rd = (rs2 < rd) ? rs2 : rd
 */
static inline void RiscvEmulatorAMOMINU_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    if (*(const uint32_t *)rs2 < *(uint32_t *)rd) {
        *(uint32_t *)rd = *(const uint32_t *)rs2;
    }
}

/**
 * Unsigned maximum: atomically load the word in rd and write back the larger of rd and rs2 as unsigned integers.
 * rd = (rs2 > rd) ? rs2 : rd
 */
static inline void RiscvEmulatorAMOMAXU_W(
    RiscvEmulatorState_t *state __attribute__((unused)),
    void *rd,
    const void *rs2) {
    if (*(const uint32_t *)rs2 > *(uint32_t *)rd) {
        *(uint32_t *)rd = *(const uint32_t *)rs2;
    }
}

#if (RVE_E_ZALRSC == 1)
/**
 * Load-reserved: load a word and register a reservation on the address.
 * rd = memory[address]; reservation = address
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
 * Store-conditional: store a word only if the reservation on the address is still valid, writing a success code of zero to rd.
 * if (reservation == address) memory[address] = rs2; rd = 0 else rd = 1
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
static inline void RiscvEmulatorOpcodeAtomicMemoryOperation(RiscvEmulatorState_t *state, uint8_t rdnum, void *rd, uint8_t rs1num, void *rs1, uint8_t rs2num, void *rs2, uint8_t funct3) {
    // Remember original address stored in rs1.
    uint32_t originaladdressrs1 = *(const uint32_t *)rs1;

    // Remember original value stored in rs2.
    uint32_t originalvaluers2 = *(const uint32_t *)rs2;

    // LR and SC perform their own memory operation and must not run the
    // store at the end of this function.
    uint8_t skipstore = 0;

    RiscvInstructionTypeRDecoderFunct5Funct3_u instruction_decoderhelper_rtypeatomicmemoryoperation = {0};
    instruction_decoderhelper_rtypeatomicmemoryoperation.funct3 = funct3;
    instruction_decoderhelper_rtypeatomicmemoryoperation.funct5 = state->instruction.rtypeatomicmemoryoperation.funct5;

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    const uint8_t funct5 = instruction_decoderhelper_rtypeatomicmemoryoperation.funct5;
    const char *instructionname = "unknown";
    switch (funct5) {
        case 0b00000:
            instructionname = "amoadd.w";
            break;
        case 0b00001:
            instructionname = "amoswap.w";
            break;
        case 0b00010:
            instructionname = "lr.w";
            break;
        case 0b00011:
            instructionname = "sc.w";
            break;
        case 0b00100:
            instructionname = "amoxor.w";
            break;
        case 0b01000:
            instructionname = "amoor.w";
            break;
        case 0b01100:
            instructionname = "amoand.w";
            break;
        case 0b10000:
            instructionname = "amomin.w";
            break;
        case 0b10100:
            instructionname = "amomax.w";
            break;
        case 0b11000:
            instructionname = "amominu.w";
            break;
        case 0b11100:
            instructionname = "amomaxu.w";
            break;
    }

    state->instructionhandled = 1;
    RiscvEmulatorHookContext_t hc = {0};
    hc.instruction = instructionname;
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
    hc.memorylocation = originaladdressrs1;
    hc.length = sizeof(uint32_t);
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintHeader(state);
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif

#if (RVE_E_ZAAMO == 1)
    // Word AMOs operate on sizeof(uint32_t) bytes.
    uint8_t amolength = sizeof(uint32_t);
    uint32_t loadedvalue = 0;
    RiscvEmulatorLoad(originaladdressrs1, &loadedvalue, amolength);

    if (rdnum != 0) {
        // Place loaded value of original address in rd.
        *(uint32_t *)rd = loadedvalue;
    }
#endif

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
            skipstore = 1;
            break;
        case FUNCT5_FUNCT3_OPERATION_SC_W:
            if (rdnum != 0) {
                RiscvEmulatorSC_W(state, rd, &originalvaluers2, originaladdressrs1);
            } else {
                // Discard the result but still perform the conditional store.
                uint32_t result = 0;
                RiscvEmulatorSC_W(state, &result, &originalvaluers2, originaladdressrs1);
            }
            skipstore = 1;
            break;
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
            skipstore = 1;
            break;
    }

#if (RVE_E_ZAAMO == 1)
    if (skipstore == 0) {
        RiscvEmulatorStore(originaladdressrs1, &loadedvalue, amolength);
    }
#else
    // Suppress unused-variable warning in Zalrsc-only builds.
    (void)skipstore;
#endif

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    hc.hook = HOOK_END;
    RiscvEmulatorHook(state, &hc);
    #if (RVE_E_DISASM == 1)
    RiscvEmulatorDisasmPrintPlain(state, &hc);
    #endif
#endif
}

#endif

#endif