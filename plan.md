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
#if RVE_E_64
typedef uint64_t rve_xlen_t;
typedef int64_t  rve_sxlen_t;
#else
typedef uint32_t rve_xlen_t;
typedef int32_t  rve_sxlen_t;
#endif
```

All instruction implementations cast operands via this type instead of raw
`uint32_t*`/`int32_t*`. Most conversions are mechanical; sign-extend-sensitive
instructions (SLT, SRA, LW, JALR) need manual review.

- Rejected: operation macros per XLEN (obscure), 64-bit registers with
  masking in RV32 mode (slow, error-prone).

## 3. Scope: strictly RV64I + Zicsr first

First implementation includes only:
- RV64I base instructions (existing I-extension instructions widened via
  `rve_xlen_t`, plus new RV64I instructions: LD, LWU, SD, ADDIW, SLLIW,
  SRLIW, SRAIW, ADDW, SUBW, SLLW, SRLW, SRAW).
- Zicsr (CSR instructions) adapted for 64-bit CSRs.

Explicitly out of scope for the first pass: M, A, C (compressed), B/Zbb/etc.
Note for later: RV64C encodings fundamentally differ from RV32C
(C_LD/C_SD vs C_LW/C_SW, C_ADDIW, C_SUBW), and M needs W-variants (MULW, DIVW...).

## 4. CSR structs: symbolic structs + mask accessors

Keep the readable symbolic CSR structs. Changes:
- CSRs that the spec defines as MXLEN-bit (mtvec, mepc, mtval, satp, misa)
  get XLEN-dependent storage guarded by `#if RVE_E_64`.
- `mtvec.base` becomes a mask-based accessor instead of a C bitfield
  (spec: mtvec is MXLEN-bit; base occupies bits XLEN-1:2 — a variable-width
  bitfield is not portable C).
- Fields that are 32-bit regardless of XLEN (e.g. mhartid) stay unchanged.
- Reference: `src/priv/machine.adoc`, mtvec section: "The mtvec register is
  an MXLEN-bit WARL read/write register... the CSR contains only bits
  XLEN-1 through 2 of the base".

## 5. Addresses follow XLEN

`programcounter`, `programcounternext`, the address parameter of the
externally implemented `RiscvEmulatorLoad/Store` (EEI), and `ram_length`
/ `RAM_ORIGIN` / `ROM_ORIGIN` become `rve_xlen_t`.

- Spec basis: registers are XLEN bits wide (`rv32.adoc`, norm
  `rv32i_xreg_sz`); effective address = XLEN-bit rs1 + sign-extended offset
  (norm `ldst_ea`); pc is XLEN-bit. The physical memory interface itself is
  left to the EEI by the spec, so user-side EEI implementations (AVR,
  Native) may truncate addresses to 32-bit — that is explicitly allowed.
- RV32 builds see no interface change.

## 6. misa/MXL hard-coded per build

misa's MXL field is hard-coded per build: MXL=1 (bits [31:30]) for RV32,
MXL=2 (bits [63:62]) for RV64. Writes to MXL are ignored (WARL).

- Spec basis: "MXL encodes the native base integer ISA width" and is WARL
  (`src/priv/machine.adoc`, misa section). A build compiled as RV64 *is*
  an RV64 implementation.
- Rejected: runtime-writable MXL (dynamic XLEN) — massive runtime cost in
  every ALU op, and no practical use in a bare-metal M-mode emulator.

## 7. Config flag (pending — final open question)

Recommended and to be confirmed: `RVE_E_64`, boolean, default 0 (RV32),
in `RiscvEmulatorConfig.h`, following the existing `RVE_E_*` boolean pattern
and GCC ISA naming (`rv64...`). Backwards compatible: existing builds
compile unchanged.

Alternative considered: `RVE_XLEN` value flag (32/64) — rejected for
inconsistency with the existing boolean pattern.

## Encoding notes for implementation

- Two new opcode defines needed in `RiscvEmulatorDefineOpcode.h`:
  `OPCODE32_OP_IMM_32` = 0b0011011 (ADDIW, SLLIW, SRLIW, SRAIW) and
  `OPCODE32_OP_32` = 0b0111011 (ADDW, SUBW, SLLW, SRLW, SRAW).
- New LOAD funct3 values: 011 = LD, 110 = LWU. New STORE funct3: 011 = SD.
- Dispatch: new opcode cases in `RiscvEmulatorLoop` / `RiscvEmulatorExtensionI.h`
  guarded by `#if RVE_E_64`.
- LUI/AUIPC in RV64 sign-extend the 32-bit immediate to XLEN (spec, rv64.adoc).
