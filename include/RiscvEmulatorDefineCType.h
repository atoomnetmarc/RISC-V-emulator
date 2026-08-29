/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorDefineCType_H_
#define RiscvEmulatorDefineCType_H_

#if (RVE_E_C == 1)

// Compressed Register.

#define FUNCT4_MV  0b1000
#define FUNCT4_ADD 0b1001

// Compressed Arithmetic.

#define FUNCT6_FUNCT2_SUB 0b10001100
#define FUNCT6_FUNCT2_XOR 0b10001101
#define FUNCT6_FUNCT2_OR  0b10001110
#define FUNCT6_FUNCT2_AND 0b10001111

// Zcb.

#define FUNCT6_ZCB       0b100111
#define FUNCT2_ZCB_GROUP 0b11
#define FUNCT2_ZCB_MUL   0b10
#define RS2P_ZCB_ZEXTB   0b000
#define RS2P_ZCB_SEXTB   0b001
#define RS2P_ZCB_ZEXTH   0b010
#define RS2P_ZCB_SEXTH   0b011
#define RS2P_ZCB_NOT     0b101

// The imm5_3 field of the CL-type instruction selects the Zcb load or store.
#define FUNCT3_ZCB_LBU 0b000
#define FUNCT3_ZCB_LH  0b001
#define FUNCT3_ZCB_SB  0b010
#define FUNCT3_ZCB_SH  0b011

// Zcmop. c.mop.n instructions are encoded in the reserved c.lui xN, 0 space.
// The n field selects the instruction variant but all variants behave the
// same. The mask matches on the fixed bits only.
#if ((RVE_E_ZCMOP == 1) && (RVE_E_C == 1))
#define MASK_ZCMOP_CMOP    0xF1FF
#define PATTERN_ZCMOP_CMOP 0x6181
#endif

// Compressed Branch.

#define FUNCT3_FUNCT2_SRLI 0b10000
#define FUNCT3_FUNCT2_SRAI 0b10001
#define FUNCT3_FUNCT2_ANDI 0b10010

#endif

#endif