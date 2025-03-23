/*
 *
 * Copyright 2023-2025 Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorTypeCSRMachineTrapSetup_H_
#define RiscvEmulatorTypeCSRMachineTrapSetup_H_

#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#if (RVE_E_ZICSR == 1)

/**
 * Machine status register.
 */
typedef struct __attribute__((packed)) {
    uint8_t : 1;

    /**
     * S-mode Interrupt Enable.
     */
    uint8_t sie : 1;

    uint8_t : 1;

    /**
     * M-mode Interrupt Enable.
     */
    uint8_t mie : 1;

    uint8_t : 1;

    /**
     * S-mode Previous Interrupt Enable.
     *
     * The SPIE bit indicates whether supervisor interrupts were enabled prior to trapping into supervisor mode.
     */
    uint8_t spie : 1;

    /**
     * U-mode Big-Endian Enable.
     */
    uint8_t ube : 1;

    /**
     * M-mode Previous Interrupt Enable.
     */
    uint8_t mpie : 1;

    /**
     * S-mode Previous Privilege.
     */
    uint8_t spp : 1;

    /**
     * Status of the vector extension.
     */
    uint8_t vs : 2;

    /**
     * M-mode Previous Privilege.
     */
    uint8_t mpp : 2;

    /**
     * Status of the floating-point unit.
     */
    uint8_t fs : 2;

    /**
     * Status of additional user-mode extensions and associated state.
     */
    uint8_t xs : 2;

    /**
     * Modify PRiVilege.
     */
    uint8_t mprv : 1;

    /**
     * Permit Supervisor User Memory access.
     */
    uint8_t sum : 1;

    /**
     * Make eXecutable Readable.
     */
    uint8_t mxr : 1;

    /**
     * Trap Virtual Memory.
     */
    uint8_t tvm : 1;

    /**
     * Timeout Wait.
     */
    uint8_t tw : 1;

    /**
     * Trap SRET.
     */
    uint8_t tsr : 1;

    uint8_t : 8;

    /**
     * FS, VS, or XS bits encode a Dirty state.
     */
    uint8_t sd : 1;
} RiscvCSRmstatus_t;

/**
 * Additional machine-mode status register.
 */
typedef struct __attribute__((packed)) {
    uint8_t : 4;

    /**
     * S-mode Big-Endian Enable.
     */
    uint8_t sbe : 1;

    /**
     * M-mode Big-Endian Enable.
     */
    uint8_t mbe : 1;

    /**
     * Guest Virtual Address.
     */
    uint8_t gva : 1;

    /**
     * Machine Previous Virtualization Mode.
     */
    uint8_t mpv : 1;

    uint32_t : 24;
} RiscvCSRmstatush_t;

/**
 * Machine ISA Register with all the extensions.
 */
typedef union {
    struct __attribute__((packed)) {
        uint32_t extensions : 26;
        uint8_t mxlen : 4;
        uint8_t mxl : 2;
    };

    struct __attribute__((packed)) {
        /**
         * Atomic extension.
         */
        uint8_t a : 1;

        /**
         * B extension.
         */
        uint8_t b : 1;

        /**
         * Compressed extension.
         */
        uint8_t c : 1;

        /**
         * Double-precision floating-point extension.
         */
        uint8_t d : 1;

        /**
         * RV32E base ISA.
         */
        uint8_t e : 1;

        /**
         * Single-precision floating-point extension.
         */
        uint8_t f : 1;

        /**
         * Reserved.
         */
        uint8_t g : 1;

        /**
         * Hypervisor extension.
         */
        uint8_t h : 1;

        /**
         * RV32I/64I/128I base ISA.
         */
        uint8_t i : 1;

        /**
         * Reserved.
         */
        uint8_t j : 1;

        /**
         * Reserved.
         */
        uint8_t k : 1;

        /**
         * Reserved.
         */
        uint8_t l : 1;

        /**
         * Integer Multiply/Divide extension.
         */
        uint8_t m : 1;

        /**
         * Tentatively reserved for User-Level Interrupts extension.
         */
        uint8_t n : 1;

        /**
         * Reserved.
         */
        uint8_t o : 1;

        /**
         * Tentatively reserved for Packed-SIMD extension.
         */
        uint8_t p : 1;

        /**
         * Quad-precision floating-point extension.
         */
        uint8_t q : 1;

        /**
         * Reserved.
         */
        uint8_t r : 1;

        /**
         * Supervisor mode implemented.
         */
        uint8_t s : 1;

        /**
         * Reserved.
         */
        uint8_t t : 1;

        /**
         * User mode implemented.
         */
        uint8_t u : 1;

        /**
         *  Vector extension.
         */
        uint8_t v : 1;

        /**
         * Reserved.
         */
        uint8_t w : 1;

        /**
         * Non-standard extensions present.
         */
        uint8_t x : 1;

        /**
         * Reserved.
         */
        uint8_t y : 1;

        /**
         * Reserved.
         */
        uint8_t z : 1;
    };

} RiscvCSRmisa_u;

/**
 * Machine Exception Delegation Register.
 */
typedef struct __attribute__((packed)) {
    uint32_t synchronousexceptions;
} RiscvCSRmedeleg_t;

/**
 * Machine Interrupt Delegation Register.
 */
typedef struct __attribute__((packed)) {
    uint32_t interrupts;
} RiscvCSRmideleg_t;

/**
 * Machine interrupt-enable register.
 */
typedef union {
    struct __attribute__((packed)) {
        uint32_t mie;
    };

    struct __attribute__((packed)) {
        uint8_t : 1;
        uint8_t ssie : 1;
        uint8_t : 1;
        uint8_t msie : 1;
        uint8_t : 1;
        uint8_t stie : 1;
        uint8_t : 1;
        uint8_t mtie : 1;
        uint8_t : 1;
        uint8_t seie : 1;
        uint8_t : 1;
        uint8_t meie : 1;
        uint8_t : 4;
        uint16_t : 16;
    };
} RiscvCSRmie_u;

/**
 * Machine Trap-Vector Base-Address Register.
 */
typedef struct __attribute__((packed)) {
    uint8_t mode : 2;
    uint32_t base : 30;
} RiscvCSRmtvec_t;

#endif

#endif
