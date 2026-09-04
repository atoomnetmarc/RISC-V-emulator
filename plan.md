# Plan: RV64I + Zicsr support (dual XLEN)

Decisions made during the design interview, in order. Each decision was
verified against the RISC-V ISA manual (`/home/marc/Downloads/riscv-isa-manual/`).

## 1. Dual XLEN via compile-time flag

The emulator will support both RV32 and RV64, selected at compile time.
No runtime XLEN switching. Existing RV32 users must not break.

- Rationale: the codebase is RV32-hardcoded everywhere (`uint32_t` registers
  in `RiscvEmulatorTypeRegister.h`, `uint32_t programcounter` in
  `RiscvEmulatorTypeEmulator.h`, CSR bitfields like `uint32_t base : 30`).
  A compile-time flag fits the existing `RVE_E_*` configuration pattern
  (`RiscvEmulatorConfig.h`, same style as `RVE_E_ZALRSC`).
- Rejected: RV64-only (breaks existing users), runtime XLEN (huge cost,
  near-zero practical value for a bare-metal M-mode emulator).

## 2. Type abstraction: `rve_xlen_t` typedef

A typedef pair in `RiscvEmulatorDefine.h`:

```c
#if RVE_XLEN == 64
typedef uint64_t rve_xlen_t;
typedef int64_t  rve_sxlen_t;
#else
typedef uint32_t rve_xlen_t;
typedef int32_t  rve_sxlen_t;
#endif
```

Register storage itself must widen: every `uint32_t`/`int32_t` field in
`RiscvRegister_u` (both the symbolic struct members `Zero`, `ra`, ... and the
`x[32]` array) becomes `rve_xlen_t`/`rve_sxlen_t`. This is what makes the
operand casts sound — casting a 4-byte field through `rve_xlen_t*` would
read/write 8 bytes and corrupt adjacent registers. The `void *rd`/`void *rs1`/
`void *rs2` plumbing in `RiscvEmulatorLoop` stays `void *` (width-agnostic);
only the dereference casts inside each instruction change.

All instruction implementations cast operands via this type instead of raw
`uint32_t*`/`int32_t*`. Most conversions are mechanical; sign-extend-sensitive
instructions need manual review. The full review set is: SLT, SRA, SRAI, LW,
LWU, LD, JAL, JALR, LUI, AUIPC, ADDI, SLTI, ANDI, ORI, XORI, and branches.
Two confirmed bugs in the current code: `RiscvEmulatorLUI` writes
`*(uint32_t *)rd = imm` (only sets low 32 bits, no sign-extension in RV64);
`RiscvEmulatorAUIPC` does `programcounter + (uint32_t)imm` (the `uint32_t`
cast zero-extends, so a negative immediate becomes a huge positive offset).
Both must sign-extend the 32-bit immediate via `(rve_sxlen_t)(int32_t)imm`
before writing/adding.

- Rejected: operation macros per XLEN (obscure), 64-bit registers with
  masking in RV32 mode (slow, error-prone).

## 3. Scope: strictly RV64I + Zicsr first

First implementation includes only:
- RV64I base instructions (existing I-extension instructions widened via
  `rve_xlen_t`, plus new RV64I instructions: LD, LWU, SD, ADDIW, SLLIW,
  SRLIW, SRAIW, ADDW, SUBW, SLLW, SRLW, SRAW).
- Zicsr (CSR instructions) adapted for 64-bit CSRs. The current CSR
  read/write path (`RiscvEmulatorExtensionZicsr.h`) hardcodes `*(uint32_t *)`
  casts for both register and CSR access. In RV64, MXLEN-bit CSRs (mstatus,
  mtvec, mepc, mtval, misa, satp, mscratch, pmpaddr0) must transfer 64 bits.
  This is resolved by replacing the raw pointer + deref with per-CSR
  read/write helper functions (`RiscvEmulatorCSRRead`/`RiscvEmulatorCSRWrite`)
  that handle width internally and apply WARL/masking in one place (MXL
  writes ignored, `mtvec.base` masked, etc.). `RiscvEmulatorGetCSRAddress` is
  retired or kept only for disasm/hook naming.

  CORRECTED during spec review: the plan originally claimed mhartid, mcause,
  mie, mip and pmpcfg0 must stay 32-bit in RV64 — this is wrong. In RV64,
  mhartid is "an MXLEN-bit read-only register" (machine.adoc) and mcause,
  mie, mip are likewise MXLEN-bit, so their storage widens to `rve_xlen_t`
  in RV64 builds like mtvec/mepc/mtval. Policy: always implement exactly what
  the spec mandates per XLEN, configurable between RV32/RV64, no out-of-spec
  shortcuts. The pmpcfg family is the only genuinely special case: in RV64
  each even-numbered pmpcfg CSR (0, 2, ... 14) holds 8 PMP entries — lower 32
  bits match the RV32 layout (entries 0-3 in pmpcfg0[31:0]), upper 32 bits
  hold entries 4-7 (`norm:pmp_cfg_rv64_layout`); odd-numbered pmpcfg CSRs are
  illegal in RV64 (`norm:pmp_cfg_rv64_illegal`). The RV64 pmpcfg0 struct
  models the full 8-entry layout (64-bit), guarded by `#if RVE_XLEN == 64`;
  the RV32 4-entry struct stays.
- Counter CSR high-half variants (0xC80 cycleh, 0xC81 timeh, 0xC82 instreth)
  are RV32I-only (spec rv-32-64g.adoc: "Upper 32 bits of `cycle`, RV32I only";
  machine.adoc: the `*h` access is "When XLEN=32"). The current code
  implements them unconditionally via the upper-half trick
  `(uint32_t *)(&cycle) + 1`. Guard these cases `#if RVE_XLEN == 32`; in RV64
  the base counter CSRs (0xC00/0xC01/0xC02) return the full 64-bit value via
  the `RiscvEmulatorCSRRead` helper, and the `*h` CSRs are absent (access →
  illegal instruction).
- `RiscvEmulatorMRET` references `state->csr.mstatush.mpv` (RV32-only struct
  per the mstatush decision). In RV64 MPV folds into `mstatus` (bit 39), so
  the `mpv = 0` clear becomes `#if RVE_XLEN == 32` / `#else
  state->csr.mstatus.mpv = 0`. The `mepc → pc` restore is fine once both are
  `rve_xlen_t`.

Explicitly out of scope for the first pass: M, A, C (compressed), B/Zbb/etc.
Note for later: RV64C encodings fundamentally differ from RV32C
(C_LD/C_SD vs C_LW/C_SW, C_ADDIW, C_SUBW), and M needs W-variants (MULW, DIVW...).

## 4. CSR structs: symbolic structs + mask accessors

Keep the readable symbolic CSR structs. Changes:
- CSRs that the spec defines as MXLEN-bit (mtvec, mepc, mtval, satp, misa,
  mstatus, pmpaddr0) get XLEN-dependent storage guarded by
  `#if RVE_XLEN == 64`.
- `mtvec.base` becomes a mask-based accessor instead of a C bitfield
  (spec: mtvec is MXLEN-bit; base occupies bits XLEN-1:2 — a variable-width
  bitfield is not portable C).
- mhartid, mcause, mie, mip: MXLEN-bit per spec, widen to `rve_xlen_t` in
  RV64 builds (see corrected note in section 3).
- pmpcfg0: 32-bit in RV32 (4×8-bit configs); in RV64 a full 8-entry 64-bit
  layout (`norm:pmp_cfg_rv64_layout`). Odd pmpcfg CSRs → illegal in RV64.
- `mstatus` is MXLEN-bit with a structurally different RV64 layout
  (spec `mstatusreg` figure vs `mstatusreg-rv321`): in RV64 it is a 64-bit
  struct with SD at bit 63, new SXL/UXL fields at [35:32], and MPV/GVA
  inlined at [39:38] (these live in `mstatush` for RV32). The current
  32-bit struct (SD at bit 31) is the RV32 layout and stays for RV32.
  SXL/UXL are hard-coded to the build XLEN (2 for RV64, 1 for RV32).
- `mstatush` is RV32-only (spec norm `mstatush_sz_acc`: "For RV32 only").
  In RV64 its SBE/MBE/GVA/MPV fields fold into `mstatus`, so `mstatush`
  is guarded `#if RVE_XLEN == 32` (not compiled in RV64).
- `satp` is not just a width change but a field-layout change. The current
  struct is the RV32 Sv32 layout (22-bit PPN, 9-bit ASID, 1-bit MODE = 32
  bits). RV64 uses Sv39/Sv48: 4-bit MODE [63:60], 16-bit ASID [59:44],
  44-bit PPN [43:0]. A separate RV64 `satp` struct is added, guarded by
  `#if RVE_XLEN == 64`; the RV32 Sv32 struct stays. This emulator is M-mode
  bare-metal and likely never enables paging, but the storage layout must be
  correct for CSR read/write fidelity (ACT may probe it).
- `pmpaddr0` widens to `rve_xlen_t` (54 significant bits in RV64; bits
  [55:54] read as 0). `pmpcfg0` is 32-bit in RV32 (4×8-bit configs); in RV64
  it becomes a full 8-entry 64-bit layout per `norm:pmp_cfg_rv64_layout`
  (lower 32 bits match RV32, upper 32 bits hold entries 4-7).
- medeleg, mideleg, mnstatus are also MXLEN-bit per spec and widen to
  `rve_xlen_t` in RV64 builds.
- Central CSR authority decision: the existing address-only switch
  (`RiscvEmulatorGetCSRAddress`) is replaced by `RiscvEmulatorCSRRead` /
  `RiscvEmulatorCSRWrite`, backed by a single compile-time CSR table that is
  the sole authority on (a) which CSR numbers exist in this build
  (`#if RVE_XLEN`-guarded entries; RV32-only CSRs like mstatush and the `*h`
  counters are absent in RV64 → illegal instruction), and (b) read-only vs
  read-write permission — the current code has no read-only enforcement at
  all (writes to mhartid/cycle currently succeed), which must be added per
  the Zicsr spec.
- Reference: `src/priv/machine.adoc`, mtvec section: "The mtvec register is
  an MXLEN-bit WARL read/write register... the CSR contains only bits
  XLEN-1 through 2 of the base".
- Trap handler updates (`RiscvEmulatorTrap`): the `(uint32_t)` casts on the
  `mtvec.base << 2` and vectored-mode lines truncate in RV64 and become
  `rve_xlen_t`; `mtvec.base` is read via the new mask accessor. `mepc` and
  `mtval` storage in `RiscvCSR_t` widens to `rve_xlen_t` (both are MXLEN-bit).
  The pre-existing vectored-mode bug is fixed as part of this work: vectored
  mode must set `pc = base*4 + 4*exceptioncode`, not `4*exceptioncode`
  alone (the current code discards the base).

## 5. Addresses follow XLEN

`programcounter`, `programcounternext`, the address parameter of the
externally implemented `RiscvEmulatorLoad/Store` (EEI), and `ram_length`
/ `RAM_ORIGIN` / `ROM_ORIGIN` become `rve_xlen_t`.

- Spec basis: registers are XLEN bits wide (`rv32.adoc`, norm
  `rv32i_xreg_sz`); effective address = XLEN-bit rs1 + sign-extended offset
  (norm `ldst_ea`); pc is XLEN-bit. The physical memory interface itself is
  left to the EEI by the spec, so user-side EEI implementations (AVR,
  Native) may truncate addresses to 32-bit — that is explicitly allowed.
- No ABI change for RV32: `rve_xlen_t` resolves to `uint32_t`, so the calling
  convention is identical. The `RiscvEmulatorLoad`/`RiscvEmulatorStore`
  *source declarations* in all three backends (Native, AVR, SAM) are updated
  to `rve_xlen_t address` in the same change for type consistency with the
  core's call sites (which now pass `rve_xlen_t`). AVR/SAM stay RV32-only for
  now, so they pay no 64-bit cost.

## 6. misa/MXL hard-coded per build

misa's MXL field is hard-coded per build: MXL=1 (bits [31:30]) for RV32,
MXL=2 (bits [63:62]) for RV64. Writes to MXL are ignored (WARL).

`RiscvCSRmisa_u` storage must widen to `rve_xlen_t` so that in RV64 the union
is 64 bits and MXL can sit at bits [63:62]. The `extensions` bitfield becomes
`rve_xlen_t extensions : 26` (XLEN-wide backing) so the union is XLEN-wide.
The non-spec `mxlen : 4` field in the current struct is removed — `misa` has
only MXL (2 bits) and extensions (26 bits); there is no 4-bit MXLEN field in
the RISC-V spec. This is a pre-existing bug; removing it is a behavior change
to existing RV32 builds, but anything reading `mxlen` was already
non-compliant.

- Spec basis: "MXL encodes the native base integer ISA width" and is WARL
  (`src/priv/machine.adoc`, misa section). A build compiled as RV64 *is*
  an RV64 implementation.
- Rejected: runtime-writable MXL (dynamic XLEN) — massive runtime cost in
  every ALU op, and no practical use in a bare-metal M-mode emulator.

## 7. Config flag: `RVE_XLEN` value flag

Decision: `RVE_XLEN`, value flag (32 or 64), default 32, in
`RiscvEmulatorConfig.h` with the usual `#ifndef` pattern. Backwards
compatible: existing builds compile unchanged.

Usage: `#if RVE_XLEN == 64` for RV64 paths.

Alternative considered: `RVE_E_64` boolean — rejected; a value flag is
more descriptive and extensible toward a future RV128.

An `#error` guard is added for unsupported `RVE_XLEN` values (anything other
than 32 or 64) in `RiscvEmulatorConfig.h`. Decision during review: a silent
typedef fallback to RV32 for a typo like `RVE_XLEN=86` would produce a
silently wrong build; compile-time checks are free at runtime. When RV128
arrives, the guard is updated in the same change as the typedef.

MRET/MPP scope note: the existing TODOs around privilege-mode restoration in
`RiscvEmulatorMRET` are explicitly out of scope for this plan. Only the
XLEN-dependent changes are made (mpv bit placement, widened mepc). MPP WARL
semantics for an M-mode-only hart (MPP hardwired to 3) are a candidate for a
future plan.

## Encoding notes for implementation

- Two new opcode defines needed in `RiscvEmulatorDefineOpcode.h`:
  `OPCODE32_OP_IMM_32` = 0b0011011 (ADDIW, SLLIW, SRLIW, SRAIW) and
  `OPCODE32_OP_32` = 0b0111011 (ADDW, SUBW, SLLW, SRLW, SRAW).
- New LOAD funct3 values: 011 = LD, 110 = LWU. New STORE funct3: 011 = SD.
- Dispatch: new opcode cases in `RiscvEmulatorLoop` / `RiscvEmulatorExtensionI.h`
  guarded by `#if RVE_XLEN == 64`.
- LUI/AUIPC in RV64 sign-extend the 32-bit immediate to XLEN (spec, rv64.adoc).
- Shift-amount width (RV64 correctness):
  - Variable shifts SLL/SRL/SRA: mask the shift amount with `& 0x3f`
    (6 bits) under `#if RVE_XLEN == 64`; keep `& 0x1f` (5 bits) for RV32.
  - Immediate shifts SLLI/SRLI/SRAI: the encoding changes in RV64 — `shamt`
    is 6 bits [25:20] and bit [30] (not the full funct7) distinguishes SRAI
    from SRLI/SLLI. Add a separate RV64 instruction-type struct with
    `shamt : 6` and a bit-30-based decode; the existing RV32 struct
    (`shamt : 5` + `imm11_5 : 7`) stays unchanged. Both guarded by
    `#if RVE_XLEN == 64`.
  - The `shamt` function parameter stays `uint8_t` (holds 6 bits fine); only
    the internal masks and the decode struct change.
  - W-variant shifts (SLLW/SRLW/SRAW, SLLIW/SRLIW/SRAIW) keep a 5-bit shift
    amount even in RV64 (spec rv64.adoc: rs2[4:0]; imm[5] != 0 is reserved).
    They must NOT pick up the `& 0x3f` rule. The immediate W-shifts reuse
    the existing 5-bit-shamt struct, with an `imm[5] != 0` →
    illegal-instruction check. Note: the spec marks these encodings
    *reserved* (not illegal) as of the current manual
    (rv64.adoc, `norm:slliw_srliw_sraiw_imm5_rsv`); we choose to raise
    illegal-instruction for deterministic behavior, matching Sail/ACT.
    Same policy for reserved encodings of RV64 SLLI/SRLI/SRAI
    (shamt[5] != 0 for SLLI/SRLI, or invalid bit-30/funct6 combinations).
- Hook/disasm context (`RiscvEmulatorHookContext_t`): `memorylocation`
 becomes `rve_xlen_t` (it is an address and would truncate in RV64).
 `imm`/`upperimmediate` stay `uint32_t` (encoding width is 32-bit); the
 disasm printer sign-extends for display where appropriate. The
 `rd`/`rs1`/`rs2`/`csr` operand pointers are already `void *` and need no
 change. In scope for the first pass.

## 8. Validation: RISC-V Architectural Certification Tests (ACT4)

Validation happens through the existing `../RISC-V-emulator-ACT/` framework
(ACT4 + Sail reference model + Native emulator backend), by adding an RV64
config — not by writing new ad-hoc tests. The ACT framework, configs,
backends and reporting infrastructure already exist and were proven for
RV32 (`rve-rv32i`, `rve-rv32imacb_zicsr_zifencei`, AVR backend, etc.).

Required work for RV64 validation:
- A `RVE_XLEN=64` environment in
  `RISC-V-emulator-Native/platformio_isa-extension-combination_env.ini`
  (new env, e.g. `RV64IZicsr`).
- A new ACT config (e.g. `rve-rv64i_zicsr`). This is **not** a trivial config
  addition: `scripts/gen_core.py` is hardcoded around the `rv32i` prefix
  (literal string replacements like `"rv32i variant"`, `"name: rve-rv32i"`,
  `"udb_config: rve-rv32i.yaml"`) with no `rv64` path. `gen_core.py` itself
  must be made rv64-aware before it can emit a valid rv64 config.
- The ACT rv64i test targets run against the Sail golden model. The Sail
  model already supports RV64 (no work there).
- A riscv64-capable cross toolchain is a **hard prerequisite**. The current
  `install.sh` installs only `riscv32-unknown-elf-gcc`, which cannot emit
  rv64 code. `install.sh` needs a riscv64 (or multilib) toolchain install
  step.

Acceptance criteria, decided during review:
- Regression: all existing RV32 ACT suites (rve-rv32i,
  rve-rv32imacb_zicsr_zifencei, etc.) must still pass on the Native backend,
  modulo the two deliberate behavior fixes (misa `mxlen` field removal,
  vectored-mtvec base fix). If any suite encoded the old buggy vectored
  behavior, its expectations are corrected in the same change with a note.
- Two new RV64 suites are added: `rve-rv64i` and `rve-rv64i_zicsr`
  (not just the Zicsr variant).

Alternatives rejected: hand-written PlatformIO unit tests only (weak
coverage, own expectations may be wrong); demo program only (weak coverage).
PlatformIO unit tests may still be added later for CI regression.

Backend decision: Native backend only for the first RV64I+Zicsr delivery.
The AVR/simavr backend (ATmega1284P) is deferred until later extensions
(C, M, A) land — an RV64 run on an 8-bit MCU would be extremely slow for
little added coverage, since ACT on Native already validates the core
logic.
