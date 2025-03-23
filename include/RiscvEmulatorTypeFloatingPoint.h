/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeFloatingPoint_H_
#define RiscvEmulatorTypeFloatingPoint_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_F == 1)
#if (RVE_E_D == 1)
typedef uint64_t RiscvFloatType;
#else
typedef uint32_t RiscvFloatType;
#endif
#endif

#endif