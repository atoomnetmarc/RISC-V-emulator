/*

Copyright 2023-2024 Marc Ketel
SPDX-License-Identifier: Apache-2.0

*/

#ifndef RiscvEmulatorTypeCSRMachineTrapSetup_H_
#define RiscvEmulatorTypeCSRMachineTrapSetup_H_

#include <stdint.h>

/**
 * Machine status register
 */
typedef struct __attribute__((packed)) {
    uint8_t : 1;
    uint8_t sie : 1;
    uint8_t : 1;
    uint8_t mie : 1;
    uint8_t : 1;
    uint8_t spie : 1;
    uint8_t ube : 1;
    uint8_t mpie : 1;
    uint8_t spp : 1;
    uint8_t vs : 2;
    uint8_t mpp : 2;
    uint8_t fs : 2;
    uint8_t xs : 2;
    uint8_t mprv : 1;
    uint8_t sum : 1;
    uint8_t mxr : 1;
    uint8_t tvm : 1;
    uint8_t tw : 1;
    uint8_t tsr : 1;
    uint8_t : 8;
    uint8_t sd : 1;
} RiscvCSRmstatus_t;

/**
 * Additional machine-mode status register
 */
typedef struct __attribute__((packed)) {
    uint8_t : 4;
    uint8_t sbe : 1;
    uint8_t mbe : 1;
    uint8_t gva : 1;
    uint8_t mpv : 1;
    uint32_t : 24;
} RiscvCSRmstatush_t;

/**
 * Machine ISA Register
 */
typedef struct __attribute__((packed)) {
    uint32_t extensions : 26;
    uint8_t mxlen : 4;
    uint8_t mxl : 2;
} RiscvCSRmisa_t;

/**
 * Machine ISA Register with all the extensions
 */
typedef struct __attribute__((packed)) {
    /**
     * Atomic extension
     */
    uint8_t a : 1;

    /**
     * Reserved
     */
    uint8_t b : 1;

    /**
     * Compressed extension
     */
    uint8_t c : 1;

    /**
     * Double-precision floating-point extension
     */
    uint8_t d : 1;

    /**
     * RV32E base ISA
     */
    uint8_t e : 1;

    /**
     * Single-precision floating-point extension
     */
    uint8_t f : 1;

    /**
     * Reserved
     */
    uint8_t g : 1;

    /**
     * Hypervisor extension
     */
    uint8_t h : 1;

    /**
     * RV32I/64I/128I base ISA
     */
    uint8_t i : 1;

    /**
     * Reserved
     */
    uint8_t j : 1;

    /**
     * Reserved
     */
    uint8_t k : 1;

    /**
     * Reserved
     */
    uint8_t l : 1;

    /**
     * Integer Multiply/Divide extension
     */
    uint8_t m : 1;

    /**
     * Tentatively reserved for User-Level Interrupts extension
     */
    uint8_t n : 1;

    /**
     * Reserved
     */
    uint8_t o : 1;

    /**
     * Tentatively reserved for Packed-SIMD extension
     */
    uint8_t p : 1;

    /**
     * Quad-precision floating-point extension
     */
    uint8_t q : 1;

    /**
     * Reserved
     */
    uint8_t r : 1;

    /**
     * Supervisor mode implemented
     */
    uint8_t s : 1;

    /**
     * Reserved
     */
    uint8_t t : 1;

    /**
     * User mode implemented
     */
    uint8_t u : 1;

    /**
     * "V" Vector extension implemented
     */
    uint8_t v : 1;

    /**
     * Reserved
     */
    uint8_t w : 1;

    /**
     * Non-standard extensions present
     */
    uint8_t x : 1;

    /**
     * Reserved
     */
    uint8_t y : 1;

    /**
     * Reserved
     */
    uint8_t z : 1;

    uint8_t mxlen : 4;
    uint8_t mxl : 2;
} RiscvCSRmisaExtensions_t;

/**
 * Machine ISA Register union
 */
typedef union {
    RiscvCSRmisa_t misa;
    RiscvCSRmisaExtensions_t misaExtensions;
} RiscvCSRmisa_u;

/**
 * Machine Exception Delegation Register
 */
typedef struct __attribute__((packed)) {
    uint32_t synchronousexceptions;
} RiscvCSRmedeleg_t;

/**
 * Machine Interrupt Delegation Register
 */
typedef struct __attribute__((packed)) {
    uint32_t interrupts;
} RiscvCSRmideleg_t;

/**
 * Machine interrupt-enable register
 */
typedef struct __attribute__((packed)) {
    uint32_t interrupts;
} RiscvCSRmie_t;

typedef struct __attribute__((packed)) {
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
} RiscvCSRmiebits_t;

typedef union {
    RiscvCSRmie_t mie;
    RiscvCSRmiebits_t bits;
} RiscvCSRmie_u;

/**
 * Machine Trap-Vector Base-Address Register
 */
typedef struct __attribute__((packed)) {
    uint8_t mode : 2;
    uint32_t base : 30;
} RiscvCSRmtvec_t;

#endif
