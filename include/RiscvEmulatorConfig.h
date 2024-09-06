/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorConfig_H_
#define RiscvEmulatorConfig_H_

// Integer Multiplication and Division.
#ifndef RVE_E_M
#define RVE_E_M 0
#endif

// Atomics.
#ifndef RVE_E_A
#define RVE_E_A 0
#endif

// Single-Precision Floating-Point.
#ifndef RVE_E_F
#define RVE_E_F 0
#endif

// Double-Precision Floating-Point.
#ifndef RVE_E_D
#define RVE_E_D 0
#endif

// Compressed instructions.
#ifndef RVE_E_C
#define RVE_E_C 0
#endif

// Control and Status Register Access.
#ifndef RVE_E_ZICSR
#define RVE_E_ZICSR 0
#endif

// Instruction-Fetch Fence.
#ifndef RVE_E_ZIFENCEI
#define RVE_E_ZIFENCEI 0
#endif

// Address calculation.
#ifndef RVE_E_ZBA
#define RVE_E_ZBA 0
#endif

// Basic bit manipulation.
#ifndef RVE_E_ZBB
#define RVE_E_ZBB 0
#endif

// Carry-less multiplication.
#ifndef RVE_E_ZBC
#define RVE_E_ZBC 0
#endif

// Single-bit operation.
#ifndef RVE_E_ZBS
#define RVE_E_ZBS 0
#endif

// Enable weak function hook.
#ifndef RVE_E_HOOK
#define RVE_E_HOOK 0
#endif

// Checks
#if (RVE_E_F == 1) && (RVE_E_ZICSR != 1)
#error "Single-Precision Floating-Point needs Control and Status Register Access to be enabled."
#endif

#if (RVE_E_D == 1) && (RVE_E_F != 1)
#error "Double-Precision Floating-Point needs Single-Precision Floating-Point to be enabled."
#endif

#endif