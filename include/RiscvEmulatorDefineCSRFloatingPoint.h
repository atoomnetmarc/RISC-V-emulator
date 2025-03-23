/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorDefineCSRFloatingPoint_H_
#define RiscvEmulatorDefineCSRFloatingPoint_H_

#if (RVE_E_F == 1)

// Floating-point rounding modes (frm field of fcsr).
#define FCSR_ROUNDING_MODE_RNE       0b000 // Round to Nearest, ties to Even
#define FCSR_ROUNDING_MODE_RTZ       0b001 // Round towards Zero
#define FCSR_ROUNDING_MODE_RDN       0b010 // Round Down (towards -∞)
#define FCSR_ROUNDING_MODE_RUP       0b011 // Round Up (towards +∞)
#define FCSR_ROUNDING_MODE_RMM       0b100 // Round to Nearest, ties to Max Magnitude
#define FCSR_ROUNDING_MODE_RESERVED5 0b101 // Reserved for future use.
#define FCSR_ROUNDING_MODE_RESERVED6 0b110 // Reserved for future use.
#define FCSR_ROUNDING_MODE_DYN       0b111 // Dynamic Rounding Mode (from fcsr)

#endif

#endif