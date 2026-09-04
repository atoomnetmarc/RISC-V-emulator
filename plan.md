# Plan: RV64I + Zicsr support (dual XLEN)

## Goal

The emulator will support RV64I + Zicsr alongside RV32, selected via a
compile-time flag. Every decision in this plan has been verified against the
RISC-V ISA manual (`/home/marc/Downloads/riscv-isa-manual/`).

## Decision criteria

- **In-spec, always.** Everything the spec mandates per XLEN is implemented
  exactly as specified: no out-of-spec intermediate solutions, no masking
  shortcuts. Encodings the spec marks as *reserved* are handled
  deterministically as illegal-instruction (this matches Sail, and hence
  ACT).
- **Compile-time ifs are free.** `#if RVE_XLEN == 64` branching costs nothing
  at run time; we use it freely where needed. Run-time XLEN switching does
  not exist in this plan.
- **Configurable, not forked.** One codebase; RV32 and RV64 are build options
  of the same source, not diverging copies.
- **Compliance wins over backwards compatibility.** Where a behavior change
  is necessary for spec compliance, it is made — e.g. the misa MXL field
  removal and the vectored-mtvec base fix. RV32 ABI/config compatibility is
  preserved where compliance allows it.

## 1. Dual XLEN via compile-time flag `RVE_XLEN`

`RVE_XLEN` is a value flag (32 or 64) in `RiscvEmulatorConfig.h`. There is
no default: the user must always make a deliberate choice. If `RVE_XLEN` is
not defined, a helpful `#error` message tells the user to define it (e.g.
`#define RVE_XLEN 32` or `#define RVE_XLEN 64`).

- Rationale: the codebase is RV32-hardcoded everywhere (`uint32_t` registers
  in `RiscvEmulatorTypeRegister.h`, `uint32_t programcounter` in
  `RiscvEmulatorTypeEmulator.h`, CSR bitfields like `uint32_t base : 30`).
  A compile-time flag fits the existing `RVE_E_*` configuration pattern
  (`RiscvEmulatorConfig.h`, same style as `RVE_E_ZALRSC`).
- An `#error` guard sits at the top of the config: any value other than 32
  or 64 fails the build. A silent fallback to RV32 on a typo would produce a
  silently wrong build; the typedef branch is the single source of truth.
- A value flag (rather than a boolean) is more descriptive and extensible
  toward a future RV128 — when that step happens, the guard and typedef
  change in the same change.

## 2. Type abstraction: `rve_xlen_t` typedef

In `RiscvEmulatorDefine.h`:

```c
#if RVE_XLEN == 64
typedef uint64_t rve_xlen_t;
typedef int64_t  rve_sxlen_t;
#else
typedef uint32_t rve_xlen_t;
typedef int32_t  rve_sxlen_t;
#endif
```

Register storage widens along with it: every `uint32_t`/`int32_t` field in
`RiscvRegister_u` (both the symbolic struct members `Zero`, `ra`, ... and the
`x[32]` array) becomes `rve_xlen_t`/`rve_sxlen_t`. This makes the operand
casts sound — casting a 4-byte field through `rve_xlen_t*` would otherwise
read/write 8 bytes and corrupt adjacent registers. The `void *rd`/`void *rs1`/
`void *rs2` plumbing in `RiscvEmulatorLoop` stays `void *` (width-agnostic);
only the dereference casts inside each instruction change.

All instruction implementations cast operands via this type instead of raw
`uint32_t*`/`int32_t*`. Most conversions are mechanical; sign-extend-sensitive
instructions get manual review. The full review set is: SLT, SRA, SRAI, LW,
LWU, LD, JAL, JALR, LUI, AUIPC, ADDI, SLTI, ANDI, ORI, XORI, and branches.

Two known bugs in the current code are fixed by this work:

- `RiscvEmulatorLUI` writes `*(uint32_t *)rd = imm` — in RV64 the 32-bit
  immediate must be sign-extended to XLEN (spec rv64.adoc,
  `norm:lui_op_rv64i`).
- `RiscvEmulatorAUIPC` does `programcounter + (uint32_t)imm` — the
  `uint32_t` cast zero-extends; the immediate must be sign-extended
  (spec rv64.adoc, `norm:auipc_op_rv64i`).

Both sign-extend via `(rve_sxlen_t)(int32_t)imm` before writing/adding.

## 3. Scope: strictly RV64I + Zicsr first

The first implementation includes only:

- RV64I base instructions: existing I-extension instructions widened via
  `rve_xlen_t`, plus the new RV64I instructions: LD, LWU, SD, ADDIW, SLLIW,
  SRLIW, SRAIW, ADDW, SUBW, SLLW, SRLW, SRAW.
- Zicsr (CSR instructions), adapted for 64-bit CSRs — see section 4.
- Explicitly out of scope: M, A, C (compressed), B/Zbb, etc. Note for later:
  RV64C encodings fundamentally differ from RV32C (C_LD/C_SD vs C_LW/C_SW,
  C_ADDIW, C_SUBW) and M needs W-variants (MULW, DIVW, ...).

## 4. CSRs: central table + symbolic structs with mask accessors

### Central CSRRead/CSRWrite

The raw pointer + deref in the current CSR path
(`RiscvEmulatorExtensionZicsr.h`, hardcoded `*(uint32_t *)` casts) is
replaced by `RiscvEmulatorCSRRead`/`RiscvEmulatorCSRWrite`. These are backed
by one central compile-time CSR table that is the sole authority for:

1. **Existence per XLEN.** Every CSR entry is `#if RVE_XLEN`-guarded. In
   RV64 these are absent (→ illegal instruction): `mstatush` (0x310),
   `medelegh` (0x312), all `*h` counter CSRs (`mcycleh` 0xB80, `minstreth`
   0xB82, `mhpmcounter*h`, `mhpevent*h`, `cycleh` 0xC80, `timeh` 0xC81,
   `instreth` 0xC82, `hpmcounter*h` 0xC83+), and the odd pmpcfg CSRs
   (pmpcfg1, 3, ..., 15; spec `norm:pmp_cfg_rv64_illegal`). The spec
   mandates: access to a non-existent CSR → illegal instruction.
2. **Permissions.** Read-only vs read-write per CSR; writes to read-only
   CSRs (mhartid, counters, etc.) → illegal instruction (Zicsr spec).
3. **WARL/masking in one place.** MXL writes ignored, `mtvec.base` masked,
   etc.

`RiscvEmulatorGetCSRAddress` is retired; naming for disasm/hooks moves
(optionally) into the table.

### Width classification (per spec)

All MXLEN-bit CSRs get XLEN-dependent storage (`rve_xlen_t` in RV64),
guarded by `#if RVE_XLEN == 64`:

- `mstatus`, `mtvec`, `mepc`, `mtval`, `misa`, `satp`, `mscratch`,
  `pmpaddr0`, `mhartid`, `mcause`, `mie`, `mip`, `medeleg`, `mideleg`,
  `mnstatus`. The spec defines these as MXLEN-bit (machine.adoc: mhartid is
  "an MXLEN-bit read-only register"; mcause/mie/mip/medeleg/mideleg/mnstatus
  likewise).

Exceptions with their own rules:

- **`mstatush`**: RV32-only (spec norm `mstatush_sz_acc`: "For RV32 only").
  In RV64 its SBE/MBE/GVA/MPV fields fold into `mstatus`; `mstatush` is not
  compiled under `#if RVE_XLEN == 32`.
- **`pmpcfg` family**: the one true layout special-case.
  - RV32: `pmpcfg0` is 32-bit (4×8-bit configs, entries 0-3).
  - RV64: each even pmpcfg CSR (0, 2, ..., 14) holds 8 PMP entries — the
    lower 32 bits match the RV32 layout (entries 0-3 in pmpcfg0[31:0]), the
    upper 32 bits hold entries 4-7 (spec `norm:pmp_cfg_rv64_layout`). The
    RV64 `pmpcfg0` struct models this full 8-entry 64-bit layout. Odd
    pmpcfg CSRs are illegal in RV64.
- **`pmpaddr0`**: widens to `rve_xlen_t` (54 significant bits in RV64; bits
  [55:54] read as 0).

### Specific structs

- `mtvec.base` becomes a mask-based accessor instead of a C bitfield (spec:
  mtvec is MXLEN-bit; base occupies bits XLEN-1:2 — a variable-width
  bitfield is not portable C).
- `mstatus` has a structurally different RV64 layout (spec `mstatusreg`
  figure vs `mstatusreg-rv321`): in RV64 a 64-bit struct with SD at bit 63,
  SXL/UXL at [35:32], and MPV/GVA inlined at [39:38] (these live in
  `mstatush` for RV32). The 32-bit struct (SD at bit 31) is the RV32 layout
  and stays for RV32. SXL/UXL are hard-coded to the build XLEN (2 for RV64,
  1 for RV32) — this is compliant with the WARL definition: a build
  compiled as RV64 *is* an RV64 implementation.
- `satp` is not just a width change but a field-layout change. RV32: Sv32
  layout (22-bit PPN, 9-bit ASID, 1-bit MODE = 32 bits). RV64: Sv39/Sv48 —
  4-bit MODE [63:60], 16-bit ASID [59:44], 44-bit PPN [43:0]. A separate
  RV64 `satp` struct is added, guarded by `#if RVE_XLEN == 64`; the RV32
  Sv32 struct stays. This M-mode bare-metal emulator likely never enables
  paging, but the storage layout must be correct for CSR read/write
  fidelity (ACT may probe it).
- `misa`: MXL is WARL and hard-coded per build — MXL=1 (bits [31:30]) for
  RV32, MXL=2 (bits [63:62]) for RV64; writes to MXL are ignored.
  `RiscvCSRmisa_u` widens to `rve_xlen_t` so the union is 64 bits in RV64
  and MXL can sit at bits [63:62]. The `extensions` bitfield becomes
  `rve_xlen_t extensions : 26` (XLEN-wide backing). The spec defines only
  MXL (2 bits) and extensions (26 bits) — there is no additional MXLEN
  field.

### Trap handling

- `RiscvEmulatorTrap`: the `(uint32_t)` casts on the `mtvec.base << 2` and
  vectored-mode lines become `rve_xlen_t`; `mtvec.base` is read via the new
  mask accessor. `mepc` and `mtval` storage in `RiscvCSR_t` widen to
  `rve_xlen_t` (both are MXLEN-bit).
- Vectored-mode semantics are per spec: `pc = base*4 + 4*exceptioncode`.
- `RiscvEmulatorMRET`: only the XLEN-dependent parts change — MPV lives in
  RV64 in `mstatus` (bit 39), so the `mpv = 0` clear becomes
  `#if RVE_XLEN == 32` (via `mstatush`) / `#else` (via `mstatus`). The
  `mepc → pc` restore is correct once both are `rve_xlen_t`. The
  privilege-mode TODO around MPP remains explicitly out of scope for this
  plan.

## 5. Addresses follow XLEN

`programcounter`, `programcounternext`, the address parameter of the
externally implemented `RiscvEmulatorLoad/Store` (EEI), and `ram_length` /
`RAM_ORIGIN` / `ROM_ORIGIN` become `rve_xlen_t`.

- Spec basis: registers are XLEN bits wide (`rv32.adoc`, norm
  `rv32i_xreg_sz`); effective address = XLEN-bit rs1 + sign-extended offset
  (norm `ldst_ea`); pc is XLEN-bit. The physical memory interface is left to
  the EEI by the spec, so user-side EEI implementations (AVR, Native) may
  truncate addresses to 32-bit — that is explicitly allowed.
- No ABI change for RV32: `rve_xlen_t` resolves to `uint32_t`, so the
  calling convention is identical. The `RiscvEmulatorLoad`/`RiscvEmulatorStore`
  declarations in all three backends (Native, AVR, SAM) are updated to
  `rve_xlen_t address` in the same change for type consistency with the
  core's call sites. AVR/SAM stay RV32-only for now, so they pay no 64-bit
  cost.

## Encoding notes for implementation

- Two new opcode defines needed in `RiscvEmulatorDefineOpcode.h`:
  `OPCODE32_OP_IMM_32` = 0b0011011 (ADDIW, SLLIW, SRLIW, SRAIW) and
  `OPCODE32_OP_32` = 0b0111011 (ADDW, SUBW, SLLW, SRLW, SRAW).
- New LOAD funct3 values: 011 = LD, 110 = LWU. New STORE funct3: 011 = SD.
- Dispatch: new opcode cases in `RiscvEmulatorLoop` / `RiscvEmulatorExtensionI.h`
  guarded by `#if RVE_XLEN == 64`.
- LUI/AUIPC sign-extend the 32-bit immediate to XLEN (spec rv64.adoc).
- **Shift-amount width (RV64 correctness):**
  - Variable shifts SLL/SRL/SRA: in RV64I only the low 6 bits of rs2 are
    used as the shift amount (spec rv64.adoc) → mask with `& 0x3f` under
    `#if RVE_XLEN == 64`; RV32 keeps `& 0x1f`.
  - Immediate shifts SLLI/SRLI/SRAI: the encoding changes in RV64 — `shamt`
    is 6 bits [25:20] and bit [30] distinguishes SRAI from SRLI/SLLI. Add a
    separate RV64 instruction-type struct with `shamt : 6` and a bit-30-based
    decode; the RV32 struct (`shamt : 5` + `imm11_5 : 7`) stays unchanged.
    Both guarded by `#if RVE_XLEN == 64`.
  - W-variant shifts (SLLW/SRLW/SRAW, SLLIW/SRLIW/SRAIW) keep a 5-bit shift
    amount even in RV64 (spec rv64.adoc: rs2[4:0]; imm[5] != 0 is reserved).
    They must NOT pick up the `& 0x3f` rule. The immediate W-shifts reuse
    the existing 5-bit-shamt struct; encodings with `imm[5] != 0` are
    reserved per spec and are treated as illegal-instruction
    (deterministic, matches Sail/ACT). Same policy for reserved encodings of
    RV64 SLLI/SRLI/SRAI (shamt[5] != 0 for SLLI/SRLI, or invalid
    bit-30/funct6 combinations).
  - The `shamt` function parameter stays `uint8_t` (holds 6 bits fine);
    only the internal masks and the decode struct change.
- Hook/disasm context (`RiscvEmulatorHookContext_t`): `memorylocation`
  becomes `rve_xlen_t` (it is an address and would truncate in RV64).
  `imm`/`upperimmediate` stay `uint32_t` (encoding width is 32-bit); the
  disasm printer sign-extends for display where appropriate. The
  `rd`/`rs1`/`rs2`/`csr` operand pointers are already `void *` and need no
  change. In scope for the first pass.

## 6. Validation: RISC-V Architectural Certification Tests (ACT4)

Validation happens through the existing `../RISC-V-emulator-ACT/` framework
(ACT4 + Sail reference model + Native emulator backend), by adding RV64
configs — not by writing new ad-hoc tests. The ACT framework, configs,
backends and reporting infrastructure already exist and were proven for
RV32. The ACT framework ships `tests/rv64i` targets that run against the
Sail golden model (Sail already supports RV64; no work there).

Required work for RV64 validation:

- A `RVE_XLEN=64` environment in
  `RISC-V-emulator-Native/platformio_isa-extension-combination_env.ini`
  (new envs, e.g. `RV64I` and `RV64IZicsr`).
- New ACT configs: `rve-rv64i` and `rve-rv64i_zicsr`. This is not a trivial
  config addition: `scripts/gen_core.py` is hardcoded around the `rv32i`
  prefix (literal string replacements like `"rv32i variant"`,
  `"name: rve-rv32i"`, `"udb_config: rve-rv32i.yaml"`, lines 255-320) with
  no `rv64` path. `gen_core.py` must be made rv64-aware before it can emit
  a valid rv64 config.
- A riscv64-capable cross toolchain is a **hard prerequisite**. The current
  `install.sh` installs only `riscv32-unknown-elf-gcc`, which cannot emit
  rv64 code. `install.sh` needs a riscv64 (or multilib) toolchain install
  step.

### Acceptance criteria

- **RV32 regression**: all existing RV32 ACT suites (rve-rv32i,
  rve-rv32imacb_zicsr_zifencei, etc.) still pass on the Native backend,
  modulo the two deliberate compliance fixes (misa MXL field, vectored-mtvec
  base). If any suite encoded the old deviant behavior, its expectations are
  corrected in the same change, with a note in the commit/PR.
- **RV64**: the new suites `rve-rv64i` and `rve-rv64i_zicsr` pass fully
  against the Sail golden model on the Native backend.

### Backend decision

Native backend only for the first RV64I+Zicsr delivery. The AVR/simavr
backend (ATmega1284P) is deferred until later extensions (C, M, A) land —
an RV64 run on an 8-bit MCU would be extremely slow for little added
coverage, since ACT on Native already validates the core logic.

PlatformIO unit tests may still be added later for CI regression.
