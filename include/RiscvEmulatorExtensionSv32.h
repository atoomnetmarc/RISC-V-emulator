/*
 *
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 *
 */

#ifndef RiscvEmulatorExtensionSv32_H_
#define RiscvEmulatorExtensionSv32_H_

#include <stdbool.h>
#include <stdint.h>

#include "RiscvEmulatorConfig.h"

#include <RiscvEmulatorImplementationSpecific.h>

#include "RiscvEmulatorDefine.h"
#include "RiscvEmulatorTrapFlag.h"
#include "RiscvEmulatorType.h"

/**
 * Access type for a virtual memory access.
 */
typedef enum {
    RVE_SV32_ACCESS_FETCH = 0,
    RVE_SV32_ACCESS_LOAD = 1,
    RVE_SV32_ACCESS_STORE = 2,
} RiscvEmulatorSv32Access_t;

#if (RVE_E_SV32 == 1)

/**
 * Sv32 page-table entry bits.
 */
#define RVE_SV32_PTE_V         (1u << 0)
#define RVE_SV32_PTE_R         (1u << 1)
#define RVE_SV32_PTE_W         (1u << 2)
#define RVE_SV32_PTE_X         (1u << 3)
#define RVE_SV32_PTE_U         (1u << 4)
#define RVE_SV32_PTE_G         (1u << 5)
#define RVE_SV32_PTE_A         (1u << 6)
#define RVE_SV32_PTE_D         (1u << 7)
#define RVE_SV32_PTE_PPN_SHIFT 10
#define RVE_SV32_PTE_PPN_MASK  0xFFFFFC00u

/**
 * Sv32 page geometry.
 */
#define RVE_SV32_PAGE_SHIFT 12
#define RVE_SV32_PAGE_SIZE  0x1000u
#define RVE_SV32_PAGE_MASK  0xFFFu
#define RVE_SV32_VPN0_SHIFT 12
#define RVE_SV32_VPN1_SHIFT 22
#define RVE_SV32_VPN_MASK   0x3FFu

/**
 * Returns the page-fault cause for an access type.
 */
static inline uint8_t RiscvEmulatorSv32PageFaultCause(RiscvEmulatorSv32Access_t access) {
    switch (access) {
        case RVE_SV32_ACCESS_FETCH:
            return MCAUSE_EXCEPTION_CODE_INSTRUCTION_PAGE_FAULT;
        case RVE_SV32_ACCESS_LOAD:
            return MCAUSE_EXCEPTION_CODE_LOAD_PAGE_FAULT;
        default:
            return MCAUSE_EXCEPTION_CODE_STORE_PAGE_FAULT;
    }
}

/**
 * Returns the access-fault cause for an access type.
 */
static inline uint8_t RiscvEmulatorSv32AccessFaultCause(RiscvEmulatorSv32Access_t access) {
    switch (access) {
        case RVE_SV32_ACCESS_FETCH:
            return MCAUSE_EXCEPTION_CODE_INSTRUCTION_ACCESS_FAULT;
        case RVE_SV32_ACCESS_LOAD:
            return MCAUSE_EXCEPTION_CODE_LOAD_ACCESS_FAULT;
        default:
            return MCAUSE_EXCEPTION_CODE_STORE_ACCESS_FAULT;
    }
}

/**
 * Returns non-zero when the PTE permits the access.
 */
static inline uint8_t RiscvEmulatorSv32Permitted(RiscvEmulatorState_t *state, uint32_t pte, RiscvEmulatorSv32Access_t access) {
    uint8_t user = (uint8_t)((pte & RVE_SV32_PTE_U) != 0);

    // U-mode requires U=1.
    if ((state->privilege == 0) && (user == 0)) {
        return 0;
    }

    // S-mode requires U=0, except for load and store when SUM is set.
    if ((state->privilege == 1) && (user != 0)) {
        if ((access == RVE_SV32_ACCESS_FETCH) || (state->csr.mstatus.sum == 0)) {
            return 0;
        }
    }

    switch (access) {
        case RVE_SV32_ACCESS_FETCH:
            return (uint8_t)((pte & RVE_SV32_PTE_X) != 0);
        case RVE_SV32_ACCESS_LOAD:
            return (uint8_t)(((pte & RVE_SV32_PTE_R) != 0) || ((state->csr.mstatus.mxr != 0) && ((pte & RVE_SV32_PTE_X) != 0)));
        default:
            return (uint8_t)((pte & RVE_SV32_PTE_W) != 0);
    }
}

/**
 * Translate a virtual address to a physical address.
 *
 * M-mode and Bare translation return the virtual address unchanged. A failed
 * walk raises a page fault or access fault and returns false.
 */
static inline bool RiscvEmulatorTranslate(RiscvEmulatorState_t *state, uint32_t virtualaddress, RiscvEmulatorSv32Access_t access, uint32_t *physicaladdress) {
    if ((state->privilege == 3) || (state->csr.satp.mode == 0)) {
        *physicaladdress = virtualaddress;
        return true;
    }

    uint32_t vpn[2];
    vpn[0] = (virtualaddress >> RVE_SV32_VPN0_SHIFT) & RVE_SV32_VPN_MASK;
    vpn[1] = (virtualaddress >> RVE_SV32_VPN1_SHIFT) & RVE_SV32_VPN_MASK;

    uint32_t table = (uint32_t)state->csr.satp.ppn << RVE_SV32_PAGE_SHIFT;

    for (int level = 1; level >= 0; level--) {
        uint32_t pteaddress = table + (vpn[level] * (uint32_t)sizeof(uint32_t));
        uint32_t pte = 0;
        if (!RiscvEmulatorLoad(pteaddress, &pte, sizeof(pte))) {
            RiscvEmulatorRaiseException(state, RiscvEmulatorSv32AccessFaultCause(access), virtualaddress);
            return false;
        }

        // Invalid PTE, or the reserved W=1/R=0 encoding.
        if (((pte & RVE_SV32_PTE_V) == 0) || (((pte & RVE_SV32_PTE_R) == 0) && ((pte & RVE_SV32_PTE_W) != 0))) {
            RiscvEmulatorRaiseException(state, RiscvEmulatorSv32PageFaultCause(access), virtualaddress);
            return false;
        }

        // A leaf PTE has R or X set.
        if ((pte & (RVE_SV32_PTE_R | RVE_SV32_PTE_X)) != 0) {
            // A level-1 leaf is a 4 MiB superpage, which is not supported.
            if (level != 0) {
                RiscvEmulatorRaiseException(state, RiscvEmulatorSv32PageFaultCause(access), virtualaddress);
                return false;
            }

            if (RiscvEmulatorSv32Permitted(state, pte, access) == 0) {
                RiscvEmulatorRaiseException(state, RiscvEmulatorSv32PageFaultCause(access), virtualaddress);
                return false;
            }

            // Svade: A must be set, and D must be set for a store.
            if (((pte & RVE_SV32_PTE_A) == 0) || ((access == RVE_SV32_ACCESS_STORE) && ((pte & RVE_SV32_PTE_D) == 0))) {
                RiscvEmulatorRaiseException(state, RiscvEmulatorSv32PageFaultCause(access), virtualaddress);
                return false;
            }

            uint32_t ppn = (pte & RVE_SV32_PTE_PPN_MASK) >> RVE_SV32_PTE_PPN_SHIFT;
            *physicaladdress = (ppn << RVE_SV32_PAGE_SHIFT) | (virtualaddress & RVE_SV32_PAGE_MASK);
            return true;
        }

        // A pointer to the next level.
        table = ((pte & RVE_SV32_PTE_PPN_MASK) >> RVE_SV32_PTE_PPN_SHIFT) << RVE_SV32_PAGE_SHIFT;
    }

    RiscvEmulatorRaiseException(state, RiscvEmulatorSv32PageFaultCause(access), virtualaddress);
    return false;
}

#endif

/**
 * Load bytes from a virtual address.
 *
 * A page-crossing access is split into two chunks. A failed translation or
 * physical access raises a fault and returns false.
 */
static inline bool RiscvEmulatorLoadVirtual(RiscvEmulatorState_t *state, uint32_t virtualaddress, void *destination, uint8_t length, RiscvEmulatorSv32Access_t access) {
#if (RVE_E_SV32 == 1)
    uint32_t offset = virtualaddress & RVE_SV32_PAGE_MASK;
    uint8_t firstlength = length;

    if ((offset + length) > RVE_SV32_PAGE_SIZE) {
        firstlength = (uint8_t)(RVE_SV32_PAGE_SIZE - offset);
    }

    uint32_t physicaladdress = 0;
    if (!RiscvEmulatorTranslate(state, virtualaddress, access, &physicaladdress)) {
        return false;
    }
    if (!RiscvEmulatorLoad(physicaladdress, destination, firstlength)) {
        RiscvEmulatorRaiseException(state, RiscvEmulatorSv32AccessFaultCause(access), virtualaddress);
        return false;
    }

    if (firstlength == length) {
        return true;
    }

    uint32_t secondvirtualaddress = virtualaddress + firstlength;
    if (!RiscvEmulatorTranslate(state, secondvirtualaddress, access, &physicaladdress)) {
        return false;
    }
    if (!RiscvEmulatorLoad(physicaladdress, (uint8_t *)destination + firstlength, (uint8_t)(length - firstlength))) {
        RiscvEmulatorRaiseException(state, RiscvEmulatorSv32AccessFaultCause(access), secondvirtualaddress);
        return false;
    }
    return true;
#else
    (void)state;
    (void)access;
    return RiscvEmulatorLoad(virtualaddress, destination, length);
#endif
}

/**
 * Store bytes to a virtual address.
 *
 * A page-crossing access is split into two chunks. A failed translation or
 * physical access raises a fault and returns false.
 */
static inline bool RiscvEmulatorStoreVirtual(RiscvEmulatorState_t *state, uint32_t virtualaddress, const void *source, uint8_t length, RiscvEmulatorSv32Access_t access) {
#if (RVE_E_SV32 == 1)
    uint32_t offset = virtualaddress & RVE_SV32_PAGE_MASK;
    uint8_t firstlength = length;

    if ((offset + length) > RVE_SV32_PAGE_SIZE) {
        firstlength = (uint8_t)(RVE_SV32_PAGE_SIZE - offset);
    }

    uint32_t physicaladdress = 0;
    if (!RiscvEmulatorTranslate(state, virtualaddress, access, &physicaladdress)) {
        return false;
    }
    if (!RiscvEmulatorStore(physicaladdress, source, firstlength)) {
        RiscvEmulatorRaiseException(state, RiscvEmulatorSv32AccessFaultCause(access), virtualaddress);
        return false;
    }

    if (firstlength == length) {
        return true;
    }

    uint32_t secondvirtualaddress = virtualaddress + firstlength;
    if (!RiscvEmulatorTranslate(state, secondvirtualaddress, access, &physicaladdress)) {
        return false;
    }
    if (!RiscvEmulatorStore(physicaladdress, (const uint8_t *)source + firstlength, (uint8_t)(length - firstlength))) {
        RiscvEmulatorRaiseException(state, RiscvEmulatorSv32AccessFaultCause(access), secondvirtualaddress);
        return false;
    }
    return true;
#else
    (void)state;
    (void)access;
    return RiscvEmulatorStore(virtualaddress, source, length);
#endif
}

#endif
