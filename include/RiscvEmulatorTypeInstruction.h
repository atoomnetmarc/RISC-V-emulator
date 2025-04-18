/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeInstruction_H_
#define RiscvEmulatorTypeInstruction_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#include "RiscvEmulatorTypeB.h"
#include "RiscvEmulatorTypeC.h"
#include "RiscvEmulatorTypeI.h"
#include "RiscvEmulatorTypeJ.h"
#include "RiscvEmulatorTypeR.h"
#include "RiscvEmulatorTypeS.h"
#include "RiscvEmulatorTypeU.h"

/**
 * All the instruction types combined.
 */
typedef union {
    uint32_t value;

    /**
     * Easy access to the opcode of an 32-bit instruction when you do not know the instruction type yet.
     */
    struct __attribute__((packed)) {
        uint8_t opcode : 7;
    };

#if (RVE_E_C == 1)
    struct __attribute__((packed)) {
        uint16_t L;
        uint16_t H;
    };
    RiscvInstructionOpcodeC_t copcode;
    RiscvInstructionTypeCR_t crtype;
    RiscvInstructionTypeCI_t citype;
    RiscvInstructionTypeCIAddi16sp_t ciaddi16sp;
    RiscvInstructionTypeCILui_t cilui;
    RiscvInstructionTypeCILwsp_t cilwsp;
    RiscvInstructionTypeCSS_t csstype;
    RiscvInstructionTypeCSSFsdsp_t cssfsdsp;
    RiscvInstructionTypeCIW_t ciwtype;
    RiscvInstructionTypeCL_t cltype;
    RiscvInstructionTypeCS_t cstype;
    RiscvInstructionTypeCA_t catype;
    RiscvInstructionTypeCB_t cbtype;
    RiscvInstructionTypeCBImm_t cbimm;
    RiscvInstructionTypeCJ_t cjtype;
#endif

    RiscvInstructionTypeR_t rtype;
#if (RVE_E_ZBA)
    RiscvInstructionTypeRShift_t rtypeshift;
#endif
#if (RVE_E_A == 1)
    RiscvInstructionTypeRAtomicMemoryOperation_t rtypeatomicmemoryoperation;
#endif
    RiscvInstructionTypeI_t itype;
#if (RVE_E_ZICSR == 1)
    RiscvInstructionTypeICSR_t itypecsr;
    RiscvInstructionTypeICSRImm_t itypecsrimm;
#endif
    RiscvInstructionTypeIMiscMemt_t itypemiscmem;
    RiscvInstructionTypeIShiftByConstant_t itypeshiftbyconstant;
    RiscvInstructionTypeIStystem_t itypesystem;
    RiscvInstructionTypeIStystemCustom_t itypesystemcustom;
    RiscvInstructionTypeS_t stype;
    RiscvInstructionTypeB_t btype;
    RiscvInstructionTypeU_t utype;
    RiscvInstructionTypeJ_t jtype;
} RiscvInstruction_u;

#endif
