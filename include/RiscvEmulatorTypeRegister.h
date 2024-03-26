/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeRegister_H_
#define RiscvEmulatorTypeRegister_H_

#include <stdint.h>

/**
 * Named registers.
 */
typedef struct __attribute__((packed)) {
    uint32_t x0;
    uint32_t x1;
    uint32_t x2;
    uint32_t x3;
    uint32_t x4;
    uint32_t x5;
    uint32_t x6;
    uint32_t x7;
    uint32_t x8;
    uint32_t x9;
    uint32_t x10;
    uint32_t x11;
    uint32_t x12;
    uint32_t x13;
    uint32_t x14;
    uint32_t x15;
    uint32_t x16;
    uint32_t x17;
    uint32_t x18;
    uint32_t x19;
    uint32_t x20;
    uint32_t x21;
    uint32_t x22;
    uint32_t x23;
    uint32_t x24;
    uint32_t x25;
    uint32_t x26;
    uint32_t x27;
    uint32_t x28;
    uint32_t x29;
    uint32_t x30;
    uint32_t x31;

} RiscvRegisterName_t;

/**
 * Symbolic registers.
 */
typedef struct __attribute__((packed)) {
    /**
     * Always zero.
     */
    uint32_t Zero;

    /**
     * Return address.
     */
    uint32_t ra;

    /**
     * Stack pointer.
     */
    uint32_t sp;

    /**
     * Global pointer.
     */
    uint32_t gp;

    /**
     * Thread pointer.
     */
    uint32_t tp;

    /**
     * Temporary or alternate link register.
     */
    uint32_t t0;

    /**
     * Temporary register.
     */
    uint32_t t1;

    /**
     * Temporary register.
     */
    uint32_t t2;

    /**
     * Saved register or frame pointer.
     */
    uint32_t s0_fp;

    /**
     * Saved register.
     */
    uint32_t s1;

    /**
     * Function argument or return value.
     */
    uint32_t a0;

    /**
     * Function argument or return value.
     */
    uint32_t a1;

    /**
     * Function argument.
     */
    uint32_t a2;

    /**
     * Function argument.
     */
    uint32_t a3;

    /**
     * Function argument.
     */
    uint32_t a4;

    /**
     * Function argument.
     */
    uint32_t a5;

    /**
     * Function argument.
     */
    uint32_t a6;

    /**
     * Function argument.
     */
    uint32_t a7;

    /**
     * Saved register.
     */
    uint32_t s2;

    /**
     * Saved register.
     */
    uint32_t s3;

    /**
     * Saved register.
     */
    uint32_t s4;

    /**
     * Saved register.
     */
    uint32_t s5;

    /**
     * Saved register.
     */
    uint32_t s6;

    /**
     * Saved register.
     */
    uint32_t s7;

    /**
     * Saved register.
     */
    uint32_t s8;

    /**
     * Saved register.
     */
    uint32_t s9;

    /**
     * Saved register.
     */
    uint32_t s10;

    /**
     * Saved register.
     */
    uint32_t s11;

    /**
     * Temporary register.
     */
    uint32_t t3;

    /**
     * Temporary register.
     */
    uint32_t t4;

    /**
     * Temporary register.
     */
    uint32_t t5;

    /**
     * Temporary register.
     */
    uint32_t t6;
} RiscvRegisterSymbolic_t;

/**
 * Unsigned array of registers.
 */
typedef struct __attribute__((packed)) {
    uint32_t location[32];
} RiscvRegisterArray_t;

/**
 * Signed array of registers.
 */
typedef struct __attribute__((packed)) {
    int32_t location[32];
} RiscvRegisterArraySigned_t;

/**
 * Union of all the ways a register can be accessed.
 */
typedef union {
    RiscvRegisterName_t name;
    RiscvRegisterSymbolic_t symbolic;
    RiscvRegisterArray_t array;
    RiscvRegisterArraySigned_t array_signed;
} RiscvRegister_u;

#endif
