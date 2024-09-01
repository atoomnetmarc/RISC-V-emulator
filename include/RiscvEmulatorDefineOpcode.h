/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorDefineOpcode_H_
#define RiscvEmulatorDefineOpcode_H_

#include "RiscvEmulatorConfig.h"

#if (RVE_E_C == 1)

// 16-bit opcodes when RV32. Bits [15:13][1:0].

#define OPCODE16_ADDI4SPN     0b00000
#define OPCODE16_FLD          0b00100
#define OPCODE16_LW           0b01000
#define OPCODE16_FLW          0b01100
#define OPCODE16_FSD          0b10100
#define OPCODE16_SW           0b11000
#define OPCODE16_FSW          0b11100
#define OPCODE16_ADDI         0b00001
#define OPCODE16_JAL          0b00101
#define OPCODE16_LI           0b01001
#define OPCODE16_LUI_ADDI16SP 0b01101
#define OPCODE16_MISCALU      0b10001
#define OPCODE16_J            0b10101
#define OPCODE16_BEQZ         0b11001
#define OPCODE16_BNEZ         0b11101
#define OPCODE16_SLLI         0b00010
#define OPCODE16_FLDSP        0b00110
#define OPCODE16_LWSP         0b01010
#define OPCODE16_FLWSP        0b01110
#define OPCODE16_JALR_MV_ADD  0b10010
#define OPCODE16_FSDSP        0b10110

#endif

// 32-bit opcodes. Bits [6:0].

#define OPCODE32_LOAD                  0b0000011
#define OPCODE32_LOADFP                0b0000111
#define OPCODE32_CUSTOM0               0b0001011
#define OPCODE32_MISCMEM               0b0001111
#define OPCODE32_IMMEDIATE             0b0010011
#define OPCODE32_ADDUPPERIMMEDIATE2PC  0b0010111
#define OPCODE32_STORE                 0b0100011
#define OPCODE32_STOREFP               0b0100111
#define OPCODE32_CUSTOM1               0b0101011
#define OPCODE32_ATOMICMEMORYOPERATION 0b0101111
#define OPCODE32_OPERATION             0b0110011
#define OPCODE32_LOADUPPERIMMEDIATE    0b0110111
#define OPCODE32_OPERATIONFPADD        0b1000011
#define OPCODE32_OPERATIONFPSUB        0b1000111
#define OPCODE32_OPERATIONFPFNMSUB     0b1001011
#define OPCODE32_OPERATIONFPFNMADD     0b1001111
#define OPCODE32_OPERATIONFP           0b1010011
#define OPCODE32_OPERATIONVECTOR       0b1010111
#define OPCODE32_CUSTOM2               0b1011011
#define OPCODE32_BRANCH                0b1100011
#define OPCODE32_JUMPANDLINKREGISTER   0b1100111
#define OPCODE32_RESERVED_6B           0b1101011
#define OPCODE32_JUMPANDLINK           0b1101111
#define OPCODE32_SYSTEM                0b1110011
#define OPCODE32_PACKED_SIMD           0b1110111
#define OPCODE32_CUSTOM3               0b1111011
#define OPCODE32_ILLEGAL               0b1111111

#endif