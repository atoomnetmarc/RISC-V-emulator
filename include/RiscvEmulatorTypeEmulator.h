/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeEmulator_H_
#define RiscvEmulatorTypeEmulator_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#include "RiscvEmulatorTypeCSR.h"
#include "RiscvEmulatorTypeInstruction.h"
#include "RiscvEmulatorTypeRegister.h"

/**
 * Pending synchronous exception.
 *
 * Only one exception can be raised per instruction. The first raise wins;
 * later raises for the same instruction are ignored.
 */
typedef struct {
    uint8_t pending; // 0 = none, otherwise an mcause exception code.
    uint32_t tval;   // Value for mtval.
} RiscvEmulatorTrap_t;

/**
 * Riscv emulator state.
 */
typedef struct
{
    RiscvEmulatorTrap_t trap;
    uint32_t programcounter;
    uint32_t programcounternext;
    RiscvInstruction_u instruction;
    RiscvRegister_u reg;

#if (RVE_E_ZALRSC == 1)
    uint8_t reservationvalid;
    uint32_t reservationaddress;
#endif

#if (RVE_E_HOOK == 1 || RVE_E_DISASM == 1)
    uint8_t instructionhandled;
#endif

#if (RVE_E_ZICSR == 1)
    /**
     * Set by WFI. The hart is parked until a locally enabled interrupt
     * (mip & mie) is pending. Cleared by RiscvEmulatorLoop().
     */
    uint8_t waiting;

    /**
     * Current privilege mode: 0 = U-mode, 3 = M-mode.
     */
    uint8_t privilege;

    RiscvCSR_t csr;
#endif
} RiscvEmulatorState_t;

#endif
