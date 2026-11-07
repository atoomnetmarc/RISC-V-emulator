# Documentation Index

Rule files describing how code in this project is written and judged.

- **[decode-once.md](decode-once.md)** — Decode-once principle: every instruction decoder field is read exactly once per executed instruction
- **[funnel-strategy.md](funnel-strategy.md)** — Funnel strategy: merging semantically identical instruction variants into one shared handler
- **[no-bit-magic.md](no-bit-magic.md)** — No bit-magic rule: express operations in natural C form, signedness through casts
- **[no-lookup-tables.md](no-lookup-tables.md)** — No lookup tables on AVR: `static const` costs RAM, use a `switch()` or `PROGMEM`
- **[measure-once-cut-twice.md](measure-once-cut-twice.md)** — Measure once, cut twice: size changes are judged on a per-environment delta table, never a single build
- **[noinline-cold-path.md](noinline-cold-path.md)** — Noinline on cold paths: when a function may be marked `__attribute__((noinline))`
