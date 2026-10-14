# Funnel strategy

A **funnel** is a single `static inline` handler that merges semantically identical instruction variants (for example an R-type instruction and its I-type immediate variant) into one body, dispatching on named decoder constants such as `FUNCT3_*`.

## Why

Without a funnel, the body of each variant is duplicated in source and, after inlining, also duplicated in the generated code.

A funnel halves this duplication and guarantees that paired variants cannot drift apart semantically.

## Rules for writing a funnel

1. **One operand, computed once.** Determine the shared operand (register value or immediate) a single time, before dispatching. Sign/zero-extension must happen exactly once, at the right place.
2. **Dispatch on named constants.** Use the existing `FUNCT3_*` / `FUNCT7_*` constants, never raw bit values.
3. **Hooks and disassembly live inside the funnel.** Select the instruction name with a small switch (CSR-funnel style). Pseudoinstruction detection (e.g. `nop`, `mv`, `not`, `seqz`) is deterministic from the decoded fields and belongs in the funnel too.
4. **Respect decode-once.** The funnel receives already-decoded values (fields, pointers) as arguments and never reads `state->instruction.*` itself. See [decode-once.md](decode-once.md).
5. **Report recognition.** The funnel returns whether it handled the instruction, so the caller keeps ownership of the `illegalinstruction` trap.
6. **Measure first.** Record flash size per build environment before and after (e.g. `avr-size` per PlatformIO env). A funnel that regresses size or speed is reverted.
7. **Inspect the assembly.** Check the generated `.lss` listing to see what the compiler actually made of the funnel: whether reads were CSE'd, whether the dispatch became a jump table, and whether anything was inlined or duplicated unexpectedly. Size numbers alone do not explain *why* a change won or lost.

## Proven examples

- `RiscvEmulatorCSR()` (`include/RiscvEmulatorExtensionZicsr.h`) — the original funnel; ~900 bytes saved per environment.
- Operation funnel for the R-type/I-type pairs `add/addi`, `slt/slti`, `sltu/sltiu`, `xor/xori`, `or/ori`, `and/andi` (`include/RiscvEmulatorExtensionI.h`).

## When not to funnel

Variants that only share `funct3` but differ by `funct7` (e.g. `sub` vs `add`, `sra` vs `srl`) or use a different encoding (shifts with `shamt`) do not fit the "same funct3 = same behaviour" idea and stay outside the funnel.
