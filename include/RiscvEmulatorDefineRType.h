/*

Copyright 2023 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorDefineRType_H_
#define RiscvEmulatorDefineRType_H_

// R-type, register register.
#define FUNCT7_3_OPERATION_ADD  0b0000000000
#define FUNCT7_3_OPERATION_SUB  0b0100000000
#define FUNCT7_3_OPERATION_SLL  0b0000000001
#define FUNCT7_3_OPERATION_SLT  0b0000000010
#define FUNCT7_3_OPERATION_SLTU 0b0000000011
#define FUNCT7_3_OPERATION_XOR  0b0000000100
#define FUNCT7_3_OPERATION_SRL  0b0000000101
#define FUNCT7_3_OPERATION_SRA  0b0100000101
#define FUNCT7_3_OPERATION_OR   0b0000000110
#define FUNCT7_3_OPERATION_AND  0b0000000111

#endif