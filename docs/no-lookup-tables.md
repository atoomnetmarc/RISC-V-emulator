# No lookup tables on AVR

**Rule:** Do not use a `static const` table for dispatch or metadata. Use a `switch()`. If a table is genuinely unavoidable, put it in `PROGMEM`.

## Why

1. **`static const` is not flash on AVR.** avr-gcc places it in `.data`, so the table costs RAM *and* flash (the initializer is copied from flash to RAM at startup). A `switch()` costs flash only.
2. **RAM is the scarce resource.** AVR parts in this project have 256 B to 16 KiB of RAM against 4 KiB to 128 KiB of flash. Spending RAM to save flash is the wrong trade.
3. **The flash saving is not there.** Measured on the CSR lookup: the table cost 130 bytes of RAM in every Zicsr environment, and replacing it with a `switch()` cost +370 bytes of flash over all 23 environments — while three Zicsr environments got *smaller* (−58, −18, −18 bytes).
4. **A table hides the control flow.** A `switch()` shows every case at the call site; a table plus a linear scan hides the same information behind an index.

## Allowed

- `switch()` for dispatch and for per-item metadata, including an out-parameter for flags.
- `static const` tables on targets where `const` lives in flash (native, SAM).
- `PROGMEM` tables on AVR when a table is genuinely needed, read with `pgm_read_byte()` / `pgm_read_word()`.
- Scalar constants (`#define`, `enum`): those become immediates, not tables.

Judge the result with [measure-once-cut-twice.md](measure-once-cut-twice.md): a table that looks smaller in one environment can still cost RAM in all of them.
