# Funnel strategy

A **funnel** is a single `static inline` handler that merges semantically identical instruction variants (for example an R-type instruction and its I-type immediate variant) into one body, dispatching on named decoder constants such as `FUNCT3_*`.

## Why

Without a funnel, the body of each variant is duplicated in source and, after inlining, also duplicated in the generated code.

A funnel halves this duplication and guarantees that paired variants cannot drift apart semantically.

## Rules for writing a funnel

1. **One operand, computed once.** Determine the shared operand (register value or immediate) a single time, before dispatching. Sign/zero-extension must happen exactly once, at the right place.
2. **Hoist the load and the store.** Read the operands into locals and write the destination once from a single `result` variable, outside the dispatch switch. The compiler then shares the scaffolding across all variants; a load or store inside each case duplicates it. Measured on the operation funnel: −3,434 flash bytes over 21 AVR environments, zero regressions.
3. **Dispatch on named constants.** Use the existing `FUNCT3_*` / `FUNCT7_*` constants, never raw bit values.
4. **Hooks and disassembly live inside the funnel.** Select the instruction name with a small switch (CSR-funnel style). Pseudoinstruction detection (e.g. `nop`, `mv`, `not`, `seqz`) is deterministic from the decoded fields and belongs in the funnel too.
5. **Respect decode-once.** The funnel receives already-decoded values (fields, pointers) as arguments and never reads `state->instruction.*` itself. See [decode-once.md](decode-once.md).
6. **Report recognition.** The funnel returns whether it handled the instruction, so the caller keeps ownership of the `illegalinstruction` trap.
7. **Natural style, no bit-magic.** Express signedness with casts, never with bit tricks. See [no-bit-magic.md](no-bit-magic.md).
8. **Measure first.** Record flash size per build environment before and after (e.g. `avr-size` per PlatformIO env). A funnel that regresses size or speed is reverted.
9. **Inspect the assembly.** Check the generated `.lss` listing to see what the compiler actually made of the funnel: whether reads were CSE'd, whether the dispatch became a jump table, and whether anything was inlined or duplicated unexpectedly. Size numbers alone do not explain *why* a change won or lost.

## Proven examples

- `RiscvEmulatorCSR()` (`include/RiscvEmulatorExtensionZicsr.h`) — the original funnel; ~900 bytes saved per environment.
- Operation funnel for the R-type/I-type pairs `add/addi`, `slt/slti`, `sltu/sltiu`, `xor/xori`, `or/ori`, `and/andi` (`include/RiscvEmulatorExtensionI.h`) — hoisted load/store and natural cast compares; −3,434 flash bytes over 21 AVR environments, zero regressions.

## When not to funnel

Variants that only share `funct3` but differ by `funct7` (e.g. `sub` vs `add`) do not fit the "same funct3 = same behaviour" idea and stay outside the funnel.

Variants with a different encoding (shifts with `shamt`, branches) can still be funneled, but the funnel must dispatch on the encoding difference itself (e.g. a shift funnel taking the shift amount and the variant as decoded arguments), not pretend the encodings are the same. Measure first: the byte-loop cost that made a switch-inside-loop lose on the operation funnel applies here too.
