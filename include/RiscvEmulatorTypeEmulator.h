/*

Copyright 2023 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeEmulator_H_
#define RiscvEmulatorTypeEmulator_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"
#include "RiscvEmulatorTypeCSR.h"
#include "RiscvEmulatorTypeInstruction.h"
#include "RiscvEmulatorTypeRegister.h"

/**
 * Riscv emulator state.
 */
typedef struct
{
    uint32_t programcounter;
    RiscvInstruction_u instruction;
    RiscvRegister_u registers;

#if (RVE_E_ZICSR == 1)
    RiscvCSR_t csr;
#endif
} RiscvEmulatorState_t;

#endif
