# No bit-magic

**Rule:** Never hand-encode arithmetic identities with bit tricks (`^ 0x80000000`, manual carry chains, mask arithmetic) to make the compiler share code. Express operations in the natural C form and let casts carry the semantics.

## Why forbidden

1. **The compiler already knows these identities.** Optimizing compilers emit equivalent code for a signed compare and a hand-flipped unsigned one.
2. **Correctness becomes invisible.** `a ^= 0x80000000` looks like a bug. The reviewer must mentally execute the two's-complement identity to approve it. `(*(const int32_t *)rs1 < (int32_t)operand2)` is correct at a glance.
3. **It rots against the ISA.** The trick is only valid for 32-bit two's-complement compares. The natural form survives width changes, type changes and refactors without review.
4. **It violates the house style.** This codebase expresses signedness through casts (`RiscvEmulatorExtensionI.h`, `RiscvEmulatorExtensionZcb.h` sign-extension idiom), never through manual bit logic on values. Bit logic on _decoded instruction fields_ is already banned by [decode-once.md](decode-once.md); this extends the ban to bit logic on _operand values_.

## Allowed

- Casts to select signedness: `(int32_t)x < (int32_t)y` vs `(uint32_t)x < y`.
- Sign/zero-extension via the established idiom (`loadedvalue & 0x8000` → `|= 0xFFFF0000`), because that _is_ the ISA semantics, written once, in the extension handler.
- Named decoder constants and masks in `RiscvEmulatorDefine*.h` — encoding knowledge belongs in the decoder, not in operation bodies.
