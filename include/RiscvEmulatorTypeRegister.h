/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeRegister_H_
#define RiscvEmulatorTypeRegister_H_

#include <stdint.h>

/**
 * Union of all the ways a register can be accessed.
 */
typedef union {
    /**
     * Symbolic registers.
     */
    struct {
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
    };

    /**
     * Named registers.
     */
    uint32_t x[32];
} RiscvRegister_u;

#endif
