/*

Copyright 2023-2024 Marc Ketel
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
 * Riscv emulator state flags.
 */
typedef union {
    struct
    {
        uint8_t illegalinstruction : 1;

#if (RVE_E_ZICSR == 1)
        uint8_t instructionaddressmisaligned : 1;
        uint8_t breakpoint : 1;
        uint8_t loadaddressmisaligned : 1;
        uint8_t storeaddressmisaligned : 1;
        uint8_t environmentcallfrommmode : 1;
#endif
    };

    uint8_t value;
} RiscvEmulatorTrapFlag_u;

/**
 * Riscv emulator state.
 */
typedef struct
{
    RiscvEmulatorTrapFlag_u trapflag;
    uint32_t programcounter;
    uint32_t programcounternext;
    RiscvInstruction_u instruction;
    RiscvRegister_u reg;

#if (RVE_E_HOOK == 1)
    uint8_t hookexists;
#endif

#if (RVE_E_ZICSR == 1)
    RiscvCSR_t csr;
#endif
} RiscvEmulatorState_t;

#endif
