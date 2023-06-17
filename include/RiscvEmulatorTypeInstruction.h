/*

Copyright 2023 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/


#ifndef RiscvEmulatorTypeInstruction_H_
#define RiscvEmulatorTypeInstruction_H_

#include <stdint.h>

#include "RiscvEmulatorTypeR.h"
#include "RiscvEmulatorTypeI.h"
#include "RiscvEmulatorTypeS.h"
#include "RiscvEmulatorTypeB.h"
#include "RiscvEmulatorTypeU.h"
#include "RiscvEmulatorTypeJ.h"

/**
 * Easy access to the opcode of an instruction when you do not know the instruction type yet.
 */
typedef struct __attribute__((packed))
{
    uint8_t opcode : 7;
    uint32_t : 25;
} RiscvInstructionOpcode_t;

/**
 * All the instruction types combined.
 */
typedef union
{
    uint32_t value;
    RiscvInstructionOpcode_t opcode;
    RiscvInstructionTypeR_t rtype;
    RiscvInstructionTypeI_t itype;
    RiscvInstructionTypeICSR_t itypecsr;
    RiscvInstructionTypeICSRImm_t itypecsrimm;
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
