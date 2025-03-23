/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeRegister_H_
#define RiscvEmulatorTypeRegister_H_

#include <stdint.h>

#include "RiscvEmulatorDefine.h"

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
#if (RVE_E_F == 1)
        /**
         * Floating-point register f0.
         */
        RiscvFloatType f0;

        /**
         * Floating-point register f1.
         */
        RiscvFloatType f1;

        /**
         * Floating-point register f2.
         */
        RiscvFloatType f2;

        /**
         * Floating-point register f3.
         */
        RiscvFloatType f3;

        /**
         * Floating-point register f4.
         */
        RiscvFloatType f4;

        /**
         * Floating-point register f5.
         */
        RiscvFloatType f5;

        /**
         * Floating-point register f6.
         */
        RiscvFloatType f6;

        /**
         * Floating-point register f7.
         */
        RiscvFloatType f7;

        /**
         * Floating-point register f8.
         */
        RiscvFloatType f8;

        /**
         * Floating-point register f9.
         */
        RiscvFloatType f9;

        /**
         * Floating-point register f10.
         */
        RiscvFloatType f10;

        /**
         * Floating-point register f11.
         */
        RiscvFloatType f11;

        /**
         * Floating-point register f12.
         */
        RiscvFloatType f12;

        /**
         * Floating-point register f13.
         */
        RiscvFloatType f13;

        /**
         * Floating-point register f14.
         */
        RiscvFloatType f14;

        /**
         * Floating-point register f15.
         */
        RiscvFloatType f15;

        /**
         * Floating-point register f16.
         */
        RiscvFloatType f16;

        /**
         * Floating-point register f17.
         */
        RiscvFloatType f17;

        /**
         * Floating-point register f18.
         */
        RiscvFloatType f18;

        /**
         * Floating-point register f19.
         */
        RiscvFloatType f19;

        /**
         * Floating-point register f20.
         */
        RiscvFloatType f20;

        /**
         * Floating-point register f21.
         */
        RiscvFloatType f21;

        /**
         * Floating-point register f22.
         */
        RiscvFloatType f22;

        /**
         * Floating-point register f23.
         */
        RiscvFloatType f23;

        /**
         * Floating-point register f24.
         */
        RiscvFloatType f24;

        /**
         * Floating-point register f25.
         */
        RiscvFloatType f25;

        /**
         * Floating-point register f26.
         */
        RiscvFloatType f26;

        /**
         * Floating-point register f27.
         */
        RiscvFloatType f27;

        /**
         * Floating-point register f28.
         */
        RiscvFloatType f28;

        /**
         * Floating-point register f29.
         */
        RiscvFloatType f29;

        /**
         * Floating-point register f30.
         */
        RiscvFloatType f30;

        /**
         * Floating-point register f31.
         */
        RiscvFloatType f31;
#endif
    };

    /**
     * Named registers.
     */
    uint32_t x[32];
#if (RVE_E_F == 1)
    RiscvFloatType f[32];
#endif
} RiscvRegister_u;

#endif
