/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorDefine_H_
#define RiscvEmulatorDefine_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_C == 1)
    #define IALIGN 16
#else
    #define IALIGN 32
#endif

#define IO_ORIGIN  0x02000000
#define ROM_ORIGIN 0x20000000
#define RAM_ORIGIN 0x80000000

#include "RiscvEmulatorDefineBType.h"
#include "RiscvEmulatorDefineCSRMachineTrapHandling.h"
#include "RiscvEmulatorDefineIType.h"
#include "RiscvEmulatorDefineOpcode.h"
#include "RiscvEmulatorDefineRType.h"
#include "RiscvEmulatorDefineSType.h"

#endif