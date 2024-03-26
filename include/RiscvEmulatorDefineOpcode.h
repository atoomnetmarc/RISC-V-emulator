/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorDefineOpcode_H_
#define RiscvEmulatorDefineOpcode_H_

// 32-bit opcodes
#define OPCODE_LOAD                  0b0000011
#define OPCODE_LOADFP                0b0000111
#define OPCODE_CUSTOM0               0b0001011
#define OPCODE_MISCMEM               0b0001111
#define OPCODE_IMMEDIATE             0b0010011
#define OPCODE_ADDUPPERIMMEDIATE2PC  0b0010111
#define OPCODE_STORE                 0b0100011
#define OPCODE_STOREFP               0b0100111
#define OPCODE_CUSTOM1               0b0101011
#define OPCODE_ATOMICMEMORYOPERATION 0b0101111
#define OPCODE_OPERATION             0b0110011
#define OPCODE_LOADUPPERIMMEDIATE    0b0110111
#define OPCODE_OPERATIONFPADD        0b1000011
#define OPCODE_OPERATIONFPSUB        0b1000111
#define OPCODE_OPERATIONFPFNMSUB     0b1001011
#define OPCODE_OPERATIONFPFNMADD     0b1001111
#define OPCODE_OPERATIONFP           0b1010011
#define OPCODE_OPERATIONVECTOR       0b1010111
#define OPCODE_CUSTOM2               0b1011011
#define OPCODE_BRANCH                0b1100011
#define OPCODE_JUMPANDLINKREGISTER   0b1100111
#define OPCODE_RESERVED_6B           0b1101011
#define OPCODE_JUMPANDLINK           0b1101111
#define OPCODE_SYSTEM                0b1110011
#define OPCODE_PACKED_SIMD           0b1110111
#define OPCODE_CUSTOM3               0b1111011
#define OPCODE_ILLEGAL               0b1111111

#endif