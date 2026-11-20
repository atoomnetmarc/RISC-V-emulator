/*
 * Copyright Marc Ketel
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef RiscvEmulatorSnapshot_H_
#define RiscvEmulatorSnapshot_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Snapshot format constants.
 */
#define RVE_SNAPSHOT_MAGIC      0x52564553u
#define RVE_SNAPSHOT_VERSION    1u
#define RVE_SNAPSHOT_RAM_ORIGIN 0x80000000u
#define RVE_SNAPSHOT_RAM_SIZE   0x4000000u
#define RVE_SNAPSHOT_META_SIZE  0x1000u
#define RVE_SNAPSHOT_META_ADDRESS \
    (RVE_SNAPSHOT_RAM_ORIGIN + RVE_SNAPSHOT_RAM_SIZE - RVE_SNAPSHOT_META_SIZE)

/**
 * Restorable state of one virtio-mmio transport.
 */
typedef struct {
    uint32_t base;
    uint32_t size;
    uint32_t memory_base;
    uint32_t memory_size;
    uint32_t device_id;
    uint32_t vendor_id;
    uint32_t host_features;
    uint32_t host_features_sel;
    uint32_t guest_features;
    uint32_t guest_features_sel;
    uint32_t guest_page_size;
    uint32_t queue_sel;
    uint32_t queue_count;
    uint32_t queue_num[2];
    uint32_t queue_align[2];
    uint32_t queue_pfn[2];
    uint16_t last_avail_idx[2];
    uint16_t used_idx[2];
    uint32_t interrupt_status;
    uint32_t status;
} __attribute__((packed)) rve_snapshot_mmio_t;

/**
 * Platform-independent snapshot state. Fields are serialized explicitly, so
 * the in-memory alignment of this struct is not part of the file format.
 */
typedef struct {
    uint32_t magic;
    uint32_t version;
    uint64_t instructions;
    uint32_t ram_origin;
    uint32_t ram_size;
    uint32_t isa_m;
    uint32_t isa_a;
    uint32_t isa_c;
    uint32_t isa_zicsr;
    uint32_t isa_zicntr;
    uint32_t isa_sv32;
    uint32_t isa_zalrsc;

    uint32_t cpu_pc;
    uint32_t cpu_next_pc;
    uint32_t cpu_instruction;
    uint8_t cpu_instruction_executed;
    uint8_t cpu_privilege;
    uint8_t cpu_waiting;
    uint8_t cpu_wfi_timeout;
    uint8_t cpu_counter_written;
    uint8_t cpu_reservation_valid;
    uint8_t cpu_instruction_handled;
    uint32_t cpu_reservation_address;
    uint8_t trap_pending;
    uint8_t trap_cause;
    uint32_t trap_tval;
    uint32_t reg[32];

    uint32_t csr_mhartid;
    uint32_t csr_marchid;
    uint32_t csr_mstatus;
    uint32_t csr_misa;
    uint32_t csr_medeleg;
    uint32_t csr_mideleg;
    uint32_t csr_mie;
    uint32_t csr_mtvec;
    uint32_t csr_mstatush;
    uint32_t csr_menvcfg;
    uint32_t csr_mscratch;
    uint32_t csr_mepc;
    uint32_t csr_mcause;
    uint32_t csr_mtval;
    uint32_t csr_mip;
    uint32_t csr_mip_platform;
    uint32_t csr_mip_platform_software;
    uint32_t csr_pmpcfg0;
    uint32_t csr_pmpaddr0;
    uint32_t csr_mnstatus;
    uint32_t csr_stvec;
    uint32_t csr_scounteren;
    uint32_t csr_senvcfg;
    uint32_t csr_sscratch;
    uint32_t csr_sepc;
    uint32_t csr_scause;
    uint32_t csr_stval;
    uint32_t csr_satp;
    uint32_t csr_zeroreg;
    uint32_t csr_mcounteren;
    uint64_t csr_cycle;
    uint64_t csr_time;
    uint64_t csr_instret;
    uint64_t csr_mhpmcounter3;

    uint64_t clint_mtime;
    uint64_t clint_mtimecmp;
    uint64_t clint_last_us;
    uint32_t clint_msip;
    uint32_t clint_mtime_high_latched;
    bool clint_mtime_high_latch_valid;

    uint32_t plic_priority[32];
    uint32_t plic_pending;
    uint32_t plic_enable[2];
    uint32_t plic_threshold[2];

    uint8_t uart_ier;
    uint8_t uart_fcr;
    uint8_t uart_lcr;
    uint8_t uart_mcr;
    uint8_t uart_scr;
    uint8_t uart_dll;
    uint8_t uart_dlm;
    uint8_t uart_fifo[16];
    uint8_t uart_fifo_read;
    uint8_t uart_fifo_write;
    uint8_t uart_fifo_count;
    bool uart_overrun;
    bool uart_thre_pending;

    rve_snapshot_mmio_t virtio[4];
    uint64_t virtio_blk_capacity;
    uint8_t virtio_console_rx_fifo[16];
    uint8_t virtio_console_rx_read;
    uint8_t virtio_console_rx_write;
    uint8_t virtio_console_rx_count;
    bool virtio_console_rx_overrun;

    uint64_t rtc_base_ns;
    uint64_t rtc_anchor_ns;
    uint32_t rtc_latch_high;
    uint32_t rtc_alarm_low;
    uint32_t rtc_alarm_high;
    uint32_t rtc_irq_enabled;
    uint32_t rtc_alarm_status;
    uint32_t sig_pending;
} __attribute__((packed)) rve_snapshot_t;

#ifndef RVE_SNAPSHOT_BLOB_ONLY

/**
 * Emits the key of the next flat JSON field.
 */
static inline void rve_json_key(FILE *file, bool *first, const char *key) {
    if (!*first) {
        fputc(',', file);
    }
    *first = false;
    fprintf(file, "\"%s\":", key);
}

/**
 * Writes an 8-bit JSON field.
 */
static inline void rve_json_u8(FILE *file, bool *first, const char *key, uint8_t value) {
    rve_json_key(file, first, key);
    fprintf(file, "%u", (unsigned)value);
}

/**
 * Writes a 32-bit JSON field.
 */
static inline void rve_json_u32(FILE *file, bool *first, const char *key, uint32_t value) {
    rve_json_key(file, first, key);
    fprintf(file, "%u", (unsigned)value);
}

/**
 * Writes a 64-bit JSON field as a decimal string.
 */
static inline void rve_json_u64(FILE *file, bool *first, const char *key, uint64_t value) {
    rve_json_key(file, first, key);
    fprintf(file, "\"%llu\"", (unsigned long long)value);
}

/**
 * Writes a boolean JSON field.
 */
static inline void rve_json_bool(FILE *file, bool *first, const char *key, bool value) {
    rve_json_key(file, first, key);
    fputs(value ? "true" : "false", file);
}

/**
 * Writes a byte array as a JSON array of numbers.
 */
static inline void rve_json_bytes(FILE *file, bool *first, const char *key, const uint8_t *bytes, size_t length) {
    rve_json_key(file, first, key);
    fputc('[', file);
    for (size_t i = 0; i < length; i++) {
        if (i != 0) {
            fputc(',', file);
        }
        fprintf(file, "%u", (unsigned)bytes[i]);
    }
    fputc(']', file);
}

/**
 * Returns the value text that follows a flat JSON key.
 */
static inline const char *rve_json_value(const char *json, const char *key) {
    char pattern[128];
    int length = snprintf(pattern, sizeof(pattern), "\"%s\":", key);
    if (length < 0 || (size_t)length >= sizeof(pattern)) {
        return NULL;
    }
    const char *found = strstr(json, pattern);
    if (found == NULL) {
        return NULL;
    }
    return found + strlen(pattern);
}

/**
 * Reads a decimal 8-bit JSON field.
 */
static inline bool rve_json_read_u8(const char *json, const char *key, uint8_t *value) {
    const char *text = rve_json_value(json, key);
    if (text == NULL) {
        return false;
    }
    char *end;
    unsigned long parsed = strtoul(text, &end, 10);
    if (end == text || parsed > UINT8_MAX) {
        return false;
    }
    *value = (uint8_t)parsed;
    return true;
}

/**
 * Reads a decimal 32-bit JSON field.
 */
static inline bool rve_json_read_u32(const char *json, const char *key, uint32_t *value) {
    const char *text = rve_json_value(json, key);
    if (text == NULL) {
        return false;
    }
    char *end;
    unsigned long parsed = strtoul(text, &end, 10);
    if (end == text) {
        return false;
    }
    *value = (uint32_t)parsed;
    return true;
}

/**
 * Reads a 64-bit JSON field stored as a decimal string.
 */
static inline bool rve_json_read_u64(const char *json, const char *key, uint64_t *value) {
    const char *text = rve_json_value(json, key);
    if (text == NULL || *text != '"') {
        return false;
    }
    char *end;
    unsigned long long parsed = strtoull(text + 1, &end, 10);
    if (end == text + 1 || *end != '"') {
        return false;
    }
    *value = (uint64_t)parsed;
    return true;
}

/**
 * Reads a boolean JSON field.
 */
static inline bool rve_json_read_bool(const char *json, const char *key, bool *value) {
    const char *text = rve_json_value(json, key);
    if (text == NULL) {
        return false;
    }
    if (strncmp(text, "true", 4) == 0) {
        *value = true;
        return true;
    }
    if (strncmp(text, "false", 5) == 0) {
        *value = false;
        return true;
    }
    return false;
}

/**
 * Reads a JSON array of byte values.
 */
static inline bool rve_json_read_bytes(const char *json, const char *key, uint8_t *bytes, size_t length) {
    const char *text = rve_json_value(json, key);
    if (text == NULL || *text != '[') {
        return false;
    }
    text++;
    for (size_t i = 0; i < length; i++) {
        while (*text == ' ' || *text == ',') {
            text++;
        }
        char *end;
        unsigned long parsed = strtoul(text, &end, 10);
        if (end == text || parsed > UINT8_MAX) {
            return false;
        }
        bytes[i] = (uint8_t)parsed;
        text = end;
    }
    return true;
}

/**
 * Writes one virtio-mmio transport to flat JSON keys.
 */
static inline void rve_snapshot_write_mmio(FILE *file, bool *first, const char *prefix, const rve_snapshot_mmio_t *mmio) {
    char key[128];
#define SNAP_MMIO_U32(field)                                 \
    do {                                                     \
        snprintf(key, sizeof(key), "%s.%s", prefix, #field); \
        rve_json_u32(file, first, key, mmio->field);         \
    } while (0)
    SNAP_MMIO_U32(base);
    SNAP_MMIO_U32(size);
    SNAP_MMIO_U32(memory_base);
    SNAP_MMIO_U32(memory_size);
    SNAP_MMIO_U32(device_id);
    SNAP_MMIO_U32(vendor_id);
    SNAP_MMIO_U32(host_features);
    SNAP_MMIO_U32(host_features_sel);
    SNAP_MMIO_U32(guest_features);
    SNAP_MMIO_U32(guest_features_sel);
    SNAP_MMIO_U32(guest_page_size);
    SNAP_MMIO_U32(queue_sel);
    SNAP_MMIO_U32(queue_count);
    for (uint32_t i = 0; i < 2; i++) {
        snprintf(key, sizeof(key), "%s.queue%u.queue_num", prefix, (unsigned)i);
        rve_json_u32(file, first, key, mmio->queue_num[i]);
        snprintf(key, sizeof(key), "%s.queue%u.queue_align", prefix, (unsigned)i);
        rve_json_u32(file, first, key, mmio->queue_align[i]);
        snprintf(key, sizeof(key), "%s.queue%u.queue_pfn", prefix, (unsigned)i);
        rve_json_u32(file, first, key, mmio->queue_pfn[i]);
        snprintf(key, sizeof(key), "%s.queue%u.last_avail_idx", prefix, (unsigned)i);
        rve_json_u32(file, first, key, mmio->last_avail_idx[i]);
        snprintf(key, sizeof(key), "%s.queue%u.used_idx", prefix, (unsigned)i);
        rve_json_u32(file, first, key, mmio->used_idx[i]);
    }
    SNAP_MMIO_U32(interrupt_status);
    SNAP_MMIO_U32(status);
#undef SNAP_MMIO_U32
}

/**
 * Reads one virtio-mmio transport from flat JSON keys.
 */
static inline bool rve_snapshot_read_mmio(const char *json, const char *prefix, rve_snapshot_mmio_t *mmio) {
    char key[128];
    bool ok = true;
#define SNAP_MMIO_U32(field)                                                                   \
    do {                                                                                       \
        uint32_t value;                                                                        \
        snprintf(key, sizeof(key), "%s.%s", prefix, #field);                                   \
        ok = rve_json_read_u32(json, key, &value) && ok;                                       \
        memcpy((uint8_t *)mmio + offsetof(rve_snapshot_mmio_t, field), &value, sizeof(value)); \
    } while (0)
    SNAP_MMIO_U32(base);
    SNAP_MMIO_U32(size);
    SNAP_MMIO_U32(memory_base);
    SNAP_MMIO_U32(memory_size);
    SNAP_MMIO_U32(device_id);
    SNAP_MMIO_U32(vendor_id);
    SNAP_MMIO_U32(host_features);
    SNAP_MMIO_U32(host_features_sel);
    SNAP_MMIO_U32(guest_features);
    SNAP_MMIO_U32(guest_features_sel);
    SNAP_MMIO_U32(guest_page_size);
    SNAP_MMIO_U32(queue_sel);
    SNAP_MMIO_U32(queue_count);
    for (uint32_t i = 0; i < 2; i++) {
        uint32_t value;
        snprintf(key, sizeof(key), "%s.queue%u.queue_num", prefix, (unsigned)i);
        ok = rve_json_read_u32(json, key, &value) && ok;
        memcpy((uint8_t *)mmio + offsetof(rve_snapshot_mmio_t, queue_num) + i * sizeof(uint32_t), &value, sizeof(value));
        snprintf(key, sizeof(key), "%s.queue%u.queue_align", prefix, (unsigned)i);
        ok = rve_json_read_u32(json, key, &value) && ok;
        memcpy((uint8_t *)mmio + offsetof(rve_snapshot_mmio_t, queue_align) + i * sizeof(uint32_t), &value, sizeof(value));
        snprintf(key, sizeof(key), "%s.queue%u.queue_pfn", prefix, (unsigned)i);
        ok = rve_json_read_u32(json, key, &value) && ok;
        memcpy((uint8_t *)mmio + offsetof(rve_snapshot_mmio_t, queue_pfn) + i * sizeof(uint32_t), &value, sizeof(value));
        snprintf(key, sizeof(key), "%s.queue%u.last_avail_idx", prefix, (unsigned)i);
        ok = rve_json_read_u32(json, key, &value) && ok;
        uint16_t small = (uint16_t)value;
        memcpy((uint8_t *)mmio + offsetof(rve_snapshot_mmio_t, last_avail_idx) + i * sizeof(uint16_t), &small, sizeof(small));
        snprintf(key, sizeof(key), "%s.queue%u.used_idx", prefix, (unsigned)i);
        ok = rve_json_read_u32(json, key, &value) && ok;
        small = (uint16_t)value;
        memcpy((uint8_t *)mmio + offsetof(rve_snapshot_mmio_t, used_idx) + i * sizeof(uint16_t), &small, sizeof(small));
    }
    SNAP_MMIO_U32(interrupt_status);
    SNAP_MMIO_U32(status);
#undef SNAP_MMIO_U32
    return ok;
}

/**
 * Writes the complete snapshot state as flat JSON.
 */
static inline void rve_snapshot_json_write(FILE *file, const rve_snapshot_t *state) {
    bool first = true;
    fputc('{', file);
#define J8(key, field)         rve_json_u8(file, &first, key, state->field)
#define J32(key, field)        rve_json_u32(file, &first, key, state->field)
#define J64(key, field)        rve_json_u64(file, &first, key, state->field)
#define JB(key, field)         rve_json_bool(file, &first, key, state->field)
#define JBA(key, field, count) rve_json_bytes(file, &first, key, state->field, count)
    J32("magic", magic);
    J32("version", version);
    J64("instructions", instructions);
    J32("ram_origin", ram_origin);
    J32("ram_size", ram_size);
    J32("isa.m", isa_m);
    J32("isa.a", isa_a);
    J32("isa.c", isa_c);
    J32("isa.zicsr", isa_zicsr);
    J32("isa.zicntr", isa_zicntr);
    J32("isa.sv32", isa_sv32);
    J32("isa.zalrsc", isa_zalrsc);
    J32("cpu.pc", cpu_pc);
    J32("cpu.next_pc", cpu_next_pc);
    J32("cpu.instruction", cpu_instruction);
    J8("cpu.instruction_executed", cpu_instruction_executed);
    J8("cpu.privilege", cpu_privilege);
    J8("cpu.waiting", cpu_waiting);
    J8("cpu.wfi_timeout", cpu_wfi_timeout);
    J8("cpu.counter_written", cpu_counter_written);
    J8("cpu.reservation_valid", cpu_reservation_valid);
    J8("cpu.instruction_handled", cpu_instruction_handled);
    J32("cpu.reservation_address", cpu_reservation_address);
    J8("trap.pending", trap_pending);
    J8("trap.cause", trap_cause);
    J32("trap.tval", trap_tval);
    for (uint32_t i = 0; i < 32; i++) {
        char key[32];
        snprintf(key, sizeof(key), "cpu.x%u", (unsigned)i);
        rve_json_u32(file, &first, key, state->reg[i]);
    }
    J32("csr.mhartid", csr_mhartid);
    J32("csr.marchid", csr_marchid);
    J32("csr.mstatus", csr_mstatus);
    J32("csr.misa", csr_misa);
    J32("csr.medeleg", csr_medeleg);
    J32("csr.mideleg", csr_mideleg);
    J32("csr.mie", csr_mie);
    J32("csr.mtvec", csr_mtvec);
    J32("csr.mstatush", csr_mstatush);
    J32("csr.menvcfg", csr_menvcfg);
    J32("csr.mscratch", csr_mscratch);
    J32("csr.mepc", csr_mepc);
    J32("csr.mcause", csr_mcause);
    J32("csr.mtval", csr_mtval);
    J32("csr.mip", csr_mip);
    J32("csr.mip_platform", csr_mip_platform);
    J32("csr.mip_platform_software", csr_mip_platform_software);
    J32("csr.pmpcfg0", csr_pmpcfg0);
    J32("csr.pmpaddr0", csr_pmpaddr0);
    J32("csr.mnstatus", csr_mnstatus);
    J32("csr.stvec", csr_stvec);
    J32("csr.scounteren", csr_scounteren);
    J32("csr.senvcfg", csr_senvcfg);
    J32("csr.sscratch", csr_sscratch);
    J32("csr.sepc", csr_sepc);
    J32("csr.scause", csr_scause);
    J32("csr.stval", csr_stval);
    J32("csr.satp", csr_satp);
    J32("csr.zeroreg", csr_zeroreg);
    J32("csr.mcounteren", csr_mcounteren);
    J64("csr.cycle", csr_cycle);
    J64("csr.time", csr_time);
    J64("csr.instret", csr_instret);
    J64("csr.mhpmcounter3", csr_mhpmcounter3);
    J64("clint.mtime", clint_mtime);
    J64("clint.mtimecmp", clint_mtimecmp);
    J64("clint.last_us", clint_last_us);
    J32("clint.msip", clint_msip);
    J32("clint.mtime_high_latched", clint_mtime_high_latched);
    JB("clint.mtime_high_latch_valid", clint_mtime_high_latch_valid);
    for (uint32_t i = 0; i < 32; i++) {
        char key[32];
        snprintf(key, sizeof(key), "plic.priority%u", (unsigned)i);
        rve_json_u32(file, &first, key, state->plic_priority[i]);
    }
    J32("plic.pending", plic_pending);
    for (uint32_t i = 0; i < 2; i++) {
        char key[32];
        snprintf(key, sizeof(key), "plic.enable%u", (unsigned)i);
        rve_json_u32(file, &first, key, state->plic_enable[i]);
        snprintf(key, sizeof(key), "plic.threshold%u", (unsigned)i);
        rve_json_u32(file, &first, key, state->plic_threshold[i]);
    }
    J8("uart.ier", uart_ier);
    J8("uart.fcr", uart_fcr);
    J8("uart.lcr", uart_lcr);
    J8("uart.mcr", uart_mcr);
    J8("uart.scr", uart_scr);
    J8("uart.dll", uart_dll);
    J8("uart.dlm", uart_dlm);
    JBA("uart.fifo", uart_fifo, 16);
    J8("uart.fifo_read", uart_fifo_read);
    J8("uart.fifo_write", uart_fifo_write);
    J8("uart.fifo_count", uart_fifo_count);
    JB("uart.overrun", uart_overrun);
    JB("uart.thre_pending", uart_thre_pending);
    rve_snapshot_write_mmio(file, &first, "virtio_blk", &state->virtio[0]);
    J64("virtio_blk.capacity", virtio_blk_capacity);
    rve_snapshot_write_mmio(file, &first, "virtio_console", &state->virtio[1]);
    JBA("virtio_console.rx_fifo", virtio_console_rx_fifo, 16);
    J8("virtio_console.rx_read", virtio_console_rx_read);
    J8("virtio_console.rx_write", virtio_console_rx_write);
    J8("virtio_console.rx_count", virtio_console_rx_count);
    JB("virtio_console.rx_overrun", virtio_console_rx_overrun);
    rve_snapshot_write_mmio(file, &first, "virtio_net", &state->virtio[2]);
    rve_snapshot_write_mmio(file, &first, "virtio_rng", &state->virtio[3]);
    J64("rtc.base_ns", rtc_base_ns);
    J64("rtc.anchor_ns", rtc_anchor_ns);
    J32("rtc.latch_high", rtc_latch_high);
    J32("rtc.alarm_low", rtc_alarm_low);
    J32("rtc.alarm_high", rtc_alarm_high);
    J32("rtc.irq_enabled", rtc_irq_enabled);
    J32("rtc.alarm_status", rtc_alarm_status);
    J32("sig.pending", sig_pending);
#undef JBA
#undef JB
#undef J64
#undef J32
#undef J8
    fputs("}\n", file);
}

/**
 * Reads the complete snapshot state from flat JSON.
 */
static inline bool rve_snapshot_json_read(const char *json, rve_snapshot_t *state) {
    bool ok = true;
#define R8(key, field)                                                                     \
    do {                                                                                   \
        uint8_t value;                                                                     \
        ok = rve_json_read_u8(json, key, &value) && ok;                                    \
        memcpy((uint8_t *)state + offsetof(rve_snapshot_t, field), &value, sizeof(value)); \
    } while (0)
#define R32(key, field)                                                                    \
    do {                                                                                   \
        uint32_t value;                                                                    \
        ok = rve_json_read_u32(json, key, &value) && ok;                                   \
        memcpy((uint8_t *)state + offsetof(rve_snapshot_t, field), &value, sizeof(value)); \
    } while (0)
#define R64(key, field)                                                                    \
    do {                                                                                   \
        uint64_t value;                                                                    \
        ok = rve_json_read_u64(json, key, &value) && ok;                                   \
        memcpy((uint8_t *)state + offsetof(rve_snapshot_t, field), &value, sizeof(value)); \
    } while (0)
#define RB(key, field)                                                                     \
    do {                                                                                   \
        bool value;                                                                        \
        ok = rve_json_read_bool(json, key, &value) && ok;                                  \
        memcpy((uint8_t *)state + offsetof(rve_snapshot_t, field), &value, sizeof(value)); \
    } while (0)
#define RBA(key, field, count)                                                            \
    do {                                                                                  \
        uint8_t value[count];                                                             \
        ok = rve_json_read_bytes(json, key, value, count) && ok;                          \
        memcpy((uint8_t *)state + offsetof(rve_snapshot_t, field), value, sizeof(value)); \
    } while (0)
    R32("magic", magic);
    R32("version", version);
    R64("instructions", instructions);
    R32("ram_origin", ram_origin);
    R32("ram_size", ram_size);
    R32("isa.m", isa_m);
    R32("isa.a", isa_a);
    R32("isa.c", isa_c);
    R32("isa.zicsr", isa_zicsr);
    R32("isa.zicntr", isa_zicntr);
    R32("isa.sv32", isa_sv32);
    R32("isa.zalrsc", isa_zalrsc);
    R32("cpu.pc", cpu_pc);
    R32("cpu.next_pc", cpu_next_pc);
    R32("cpu.instruction", cpu_instruction);
    R8("cpu.instruction_executed", cpu_instruction_executed);
    R8("cpu.privilege", cpu_privilege);
    R8("cpu.waiting", cpu_waiting);
    R8("cpu.wfi_timeout", cpu_wfi_timeout);
    R8("cpu.counter_written", cpu_counter_written);
    R8("cpu.reservation_valid", cpu_reservation_valid);
    R8("cpu.instruction_handled", cpu_instruction_handled);
    R32("cpu.reservation_address", cpu_reservation_address);
    R8("trap.pending", trap_pending);
    R8("trap.cause", trap_cause);
    R32("trap.tval", trap_tval);
    for (uint32_t i = 0; i < 32; i++) {
        char key[32];
        uint32_t value;
        snprintf(key, sizeof(key), "cpu.x%u", (unsigned)i);
        ok = rve_json_read_u32(json, key, &value) && ok;
        memcpy((uint8_t *)state + offsetof(rve_snapshot_t, reg) + i * sizeof(uint32_t), &value, sizeof(value));
    }
    R32("csr.mhartid", csr_mhartid);
    R32("csr.marchid", csr_marchid);
    R32("csr.mstatus", csr_mstatus);
    R32("csr.misa", csr_misa);
    R32("csr.medeleg", csr_medeleg);
    R32("csr.mideleg", csr_mideleg);
    R32("csr.mie", csr_mie);
    R32("csr.mtvec", csr_mtvec);
    R32("csr.mstatush", csr_mstatush);
    R32("csr.menvcfg", csr_menvcfg);
    R32("csr.mscratch", csr_mscratch);
    R32("csr.mepc", csr_mepc);
    R32("csr.mcause", csr_mcause);
    R32("csr.mtval", csr_mtval);
    R32("csr.mip", csr_mip);
    R32("csr.mip_platform", csr_mip_platform);
    R32("csr.mip_platform_software", csr_mip_platform_software);
    R32("csr.pmpcfg0", csr_pmpcfg0);
    R32("csr.pmpaddr0", csr_pmpaddr0);
    R32("csr.mnstatus", csr_mnstatus);
    R32("csr.stvec", csr_stvec);
    R32("csr.scounteren", csr_scounteren);
    R32("csr.senvcfg", csr_senvcfg);
    R32("csr.sscratch", csr_sscratch);
    R32("csr.sepc", csr_sepc);
    R32("csr.scause", csr_scause);
    R32("csr.stval", csr_stval);
    R32("csr.satp", csr_satp);
    R32("csr.zeroreg", csr_zeroreg);
    R32("csr.mcounteren", csr_mcounteren);
    R64("csr.cycle", csr_cycle);
    R64("csr.time", csr_time);
    R64("csr.instret", csr_instret);
    R64("csr.mhpmcounter3", csr_mhpmcounter3);
    R64("clint.mtime", clint_mtime);
    R64("clint.mtimecmp", clint_mtimecmp);
    R64("clint.last_us", clint_last_us);
    R32("clint.msip", clint_msip);
    R32("clint.mtime_high_latched", clint_mtime_high_latched);
    RB("clint.mtime_high_latch_valid", clint_mtime_high_latch_valid);
    for (uint32_t i = 0; i < 32; i++) {
        char key[32];
        uint32_t value;
        snprintf(key, sizeof(key), "plic.priority%u", (unsigned)i);
        ok = rve_json_read_u32(json, key, &value) && ok;
        memcpy((uint8_t *)state + offsetof(rve_snapshot_t, plic_priority) + i * sizeof(uint32_t), &value, sizeof(value));
    }
    R32("plic.pending", plic_pending);
    for (uint32_t i = 0; i < 2; i++) {
        char key[32];
        uint32_t value;
        snprintf(key, sizeof(key), "plic.enable%u", (unsigned)i);
        ok = rve_json_read_u32(json, key, &value) && ok;
        memcpy((uint8_t *)state + offsetof(rve_snapshot_t, plic_enable) + i * sizeof(uint32_t), &value, sizeof(value));
        snprintf(key, sizeof(key), "plic.threshold%u", (unsigned)i);
        ok = rve_json_read_u32(json, key, &value) && ok;
        memcpy((uint8_t *)state + offsetof(rve_snapshot_t, plic_threshold) + i * sizeof(uint32_t), &value, sizeof(value));
    }
    R8("uart.ier", uart_ier);
    R8("uart.fcr", uart_fcr);
    R8("uart.lcr", uart_lcr);
    R8("uart.mcr", uart_mcr);
    R8("uart.scr", uart_scr);
    R8("uart.dll", uart_dll);
    R8("uart.dlm", uart_dlm);
    RBA("uart.fifo", uart_fifo, 16);
    R8("uart.fifo_read", uart_fifo_read);
    R8("uart.fifo_write", uart_fifo_write);
    R8("uart.fifo_count", uart_fifo_count);
    RB("uart.overrun", uart_overrun);
    RB("uart.thre_pending", uart_thre_pending);
    ok = rve_snapshot_read_mmio(json, "virtio_blk", &state->virtio[0]) && ok;
    R64("virtio_blk.capacity", virtio_blk_capacity);
    ok = rve_snapshot_read_mmio(json, "virtio_console", &state->virtio[1]) && ok;
    RBA("virtio_console.rx_fifo", virtio_console_rx_fifo, 16);
    R8("virtio_console.rx_read", virtio_console_rx_read);
    R8("virtio_console.rx_write", virtio_console_rx_write);
    R8("virtio_console.rx_count", virtio_console_rx_count);
    RB("virtio_console.rx_overrun", virtio_console_rx_overrun);
    ok = rve_snapshot_read_mmio(json, "virtio_net", &state->virtio[2]) && ok;
    ok = rve_snapshot_read_mmio(json, "virtio_rng", &state->virtio[3]) && ok;
    R64("rtc.base_ns", rtc_base_ns);
    R64("rtc.anchor_ns", rtc_anchor_ns);
    R32("rtc.latch_high", rtc_latch_high);
    R32("rtc.alarm_low", rtc_alarm_low);
    R32("rtc.alarm_high", rtc_alarm_high);
    R32("rtc.irq_enabled", rtc_irq_enabled);
    R32("rtc.alarm_status", rtc_alarm_status);
    R32("sig.pending", sig_pending);
#undef RBA
#undef RB
#undef R64
#undef R32
#undef R8
    return ok;
}

#endif

#endif
