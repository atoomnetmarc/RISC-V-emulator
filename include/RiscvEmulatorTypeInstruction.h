/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

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
 * Easy access to the opcode of an 32-bit instruction when you do not know the instruction type yet.
 */
typedef struct __attribute__((packed)) {
    uint8_t opcode : 7;
    uint32_t : 25;
} RiscvInstruction32Opcode_t;

/**
 * All the instruction types combined.
 */
typedef union {
    uint32_t value;

#if (RVE_E_C == 1)
    RiscvInstruction16_t value16;
    RiscvInstruction16Opcode_t copcode;
    RiscvInstruction16TypeCQ1v1_t cq1v1type;
#endif

    RiscvInstruction32Opcode_t opcode;
    RiscvInstruction32TypeR_t rtype;
#if (RVE_E_ZBA)
    RiscvInstruction32TypeRShift_t rtypeshift;
#endif
#if (RVE_E_A == 1)
    RiscvInstruction32TypeRAtomicMemoryOperation_t rtypeatomicmemoryoperation;
#endif
    RiscvInstruction32TypeI_t itype;
#if (RVE_E_ZICSR == 1)
    RiscvInstruction32TypeICSR_t itypecsr;
    RiscvInstruction32TypeICSRImm_t itypecsrimm;
#endif
    RiscvInstruction32TypeIMiscMemt_t itypemiscmem;
    RiscvInstruction32TypeIShiftByConstant_t itypeshiftbyconstant;
    RiscvInstruction32TypeIStystem_t itypesystem;
    RiscvInstruction32TypeIStystemCustom_t itypesystemcustom;
    RiscvInstruction32TypeS_t stype;
    RiscvInstruction32TypeB_t btype;
    RiscvInstruction32TypeU_t utype;
    RiscvInstruction32TypeJ_t jtype;
} RiscvInstruction32_u;

#endif
