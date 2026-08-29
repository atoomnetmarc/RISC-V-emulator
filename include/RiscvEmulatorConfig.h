/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorConfig_H_
#define RiscvEmulatorConfig_H_

// Sort extension and subsets the same as: https://gcc.gnu.org/onlinedocs/gcc/RISC-V-Options.html

// Integer Multiplication and Division extension.
#ifndef RVE_E_M
#define RVE_E_M 0
#endif

// M implies Zmmul.
#if (RVE_E_M == 1)
#define RVE_E_ZMMUL 1
#endif

// Atomics extension.
#ifndef RVE_E_A
#define RVE_E_A 0
#endif

// Enable all Atomics extension subsets.
#if (RVE_E_A == 1)
#define RVE_E_ZAAMO  1
#define RVE_E_ZALRSC 1
#endif

// Atomic Memory Operations extension.
#ifndef RVE_E_ZAAMO
#define RVE_E_ZAAMO 0
#endif

// Load-Reserved/Store-Conditional extension.
#ifndef RVE_E_ZALRSC
#define RVE_E_ZALRSC 0
#endif

// Compressed instructions extension.
#ifndef RVE_E_C
#define RVE_E_C 0
#endif

// Bit-Manipulation extension.
#ifndef RVE_E_B
#define RVE_E_B 0
#endif

// Zbkb shares the andn, orn, xnor, rol, ror, rori, rev8, sext.b and sext.h
// instructions with Zbb.
#if (RVE_E_ZBKB == 1)
#define RVE_E_ZBB 1
#endif

// Enable all Bit-Manipulation extension subsets.
#if (RVE_E_B == 1)
#define RVE_E_ZBA 1
#define RVE_E_ZBB 1
#define RVE_E_ZBC 1
#define RVE_E_ZBS 1
#define RVE_E_ZBKB 1
#define RVE_E_ZBKX 1
#endif

// Control and Status Register Access extension.
#ifndef RVE_E_ZICSR
#define RVE_E_ZICSR 0
#endif

// Support misaligned loads and stores.
// When enabled, misaligned loads and stores are executed instead of
// raising a load-address-misaligned or store-address-misaligned trap.
// Instruction address misalignment always traps, regardless of this option.
#ifndef RVE_E_MISALIGNED
#define RVE_E_MISALIGNED 0
#endif

// Instruction-Fetch Fence extension.
#ifndef RVE_E_ZIFENCEI
#define RVE_E_ZIFENCEI 0
#endif

// Bit-Manipulation extension: Address calculation subset.
#ifndef RVE_E_ZBA
#define RVE_E_ZBA 0
#endif

// Bit-Manipulation extension: Basic bit manipulation subset.
#ifndef RVE_E_ZBB
#define RVE_E_ZBB 0
#endif

// Bit-Manipulation extension: Carry-less multiplication subset.
#ifndef RVE_E_ZBC
#define RVE_E_ZBC 0
#endif

// Zbc implies Zbkc.
#if (RVE_E_ZBC == 1)
#define RVE_E_ZBKC 1
#endif

// Scalar crypto extension: Carry-less multiplication subset.
#ifndef RVE_E_ZBKC
#define RVE_E_ZBKC 0
#endif

// Bit-Manipulation extension: Single-bit operation subset.
#ifndef RVE_E_ZBS
#define RVE_E_ZBS 0
#endif

// Zcb implies C.
#if (RVE_E_ZCB == 1)
#define RVE_E_C 1
#endif

// Zicntr implies Zicsr.
#if (RVE_E_ZICNTR == 1)
#define RVE_E_ZICSR 1
#endif


// Scalar crypto extension: Byte and bit permutation subset.
#ifndef RVE_E_ZBKX
#define RVE_E_ZBKX 0
#endif

// Scalar crypto extension: Bit manipulation for cryptography subset.
#ifndef RVE_E_ZBKB
#define RVE_E_ZBKB 0
#endif

// Compressed instructions extension: additional compressed instructions subset.
#ifndef RVE_E_ZCB
#define RVE_E_ZCB 0
#endif

// Compressed May-Be-Operations extension.
#ifndef RVE_E_ZCMOP
#define RVE_E_ZCMOP 0
#endif

// Integer Conditional Operations extension.
#ifndef RVE_E_ZICOND
#define RVE_E_ZICOND 0
#endif

// Counter extension.
#ifndef RVE_E_ZICNTR
#define RVE_E_ZICNTR 0
#endif

// Non-Temporal Locality Hint extension.
#ifndef RVE_E_ZIHINTNTL
#define RVE_E_ZIHINTNTL 0
#endif

// Pause Hint extension.
#ifndef RVE_E_ZIHINTPAUSE
#define RVE_E_ZIHINTPAUSE 0
#endif

// May-Be-Operations extension.
#ifndef RVE_E_ZIMOP
#define RVE_E_ZIMOP 0
#endif

// Multiplication subset of the M extension.
#ifndef RVE_E_ZMMUL
#define RVE_E_ZMMUL 0
#endif

// Weak function hook.
#ifndef RVE_E_HOOK
#define RVE_E_HOOK 0
#endif

#endif