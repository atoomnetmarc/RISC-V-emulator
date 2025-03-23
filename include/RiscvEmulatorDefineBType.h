/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorDefineBType_H_
#define RiscvEmulatorDefineBType_H_

// B-type, branch.

#define FUNCT3_BRANCH_BEQ  0b000
#define FUNCT3_BRANCH_BNE  0b001
#define FUNCT3_BRANCH_BLT  0b100
#define FUNCT3_BRANCH_BGE  0b101
#define FUNCT3_BRANCH_BLTU 0b110
#define FUNCT3_BRANCH_BGEU 0b111

#define BRANCH_YES 0
#define BRANCH_NO  1

#endif