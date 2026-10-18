# Noinline on cold paths

## Why

GNU89 `extern inline` and the compiler's inlining heuristics duplicate small bodies at every call site. On a hot path that is the intent; on a cold path it wastes flash: the duplicated body costs flash at every decode site while the code runs at most once per trap.

## Rule

A function may be marked `__attribute__((noinline))` when all of these hold:

- The path is cold: reached at most once per exceptional event, never per executed instruction in normal operation.
- Multiple call sites duplicate the body (the candidate-1 break-even still applies: with only a few sites, inlining can be cheaper).
- The body has no timing-critical effect.

`noinline` is never allowed in normal operation: instruction fetch, decode, execute, and the trap check stay inline.

## Proven example

`RiscvEmulatorTrapFlagIllegalInstruction()` (include/RiscvEmulatorTrapFlag.h): the trap-flag read-modify-write was inlined at 21 decode sites (~10 bytes each); as a `noinline` function each site is a 4-byte `call`, −582 bytes total across the 22 AVR environments with zero regressions (lss-duplicate-candidates.md, candidate 2).
