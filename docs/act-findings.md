# ACT findings

Emulator bugs and configuration gaps found while running the RISC-V
Architectural Certification Tests. These are not Sv32-specific; the Sv32
plan keeps only the Sv32 findings.

## Zcmop encoding

`c.mop.n` is encoded as `c.lui xN, 0` with an odd `N` (1, 3, ..., 15), not as
`c.lui x0, n`. The decoder matched with mask/pattern `0xF1FF`/`0x6181`, which
compares bit 8, part of the `rd` field. Only `N` in {3, 7, 11, 15} matched; the
other variants fell through to `c.lui` and wrote zero to `xN`.

The mask is now `0xF07F`/`0x6001` (quadrant, funct3, bit 12 and bits 6:2) and
the decoder additionally requires an odd `rd`.

## Zbc implies Zbkc

UDB requires `Zbkc` whenever `Zbc` is present. The emulator already enables
`Zbkc` whenever `Zbc` is enabled (`RiscvEmulatorConfig.h`). The ACT
`gen_core.py` `B` expansion and the standalone `Zbc` subset must list both.

## RV32I UDB parameters

UDB only defines these parameters when the configuration implements the
extensions that define them:

- `MEI_INTR_IMPL`, `MSI_INTR_IMPL`, `MTI_INTR_IMPL`: `Zicsr` + `Sm`
- `SEI_INTR_IMPL`, `SSI_INTR_IMPL`, `STI_INTR_IMPL`: `Zicsr` + `S`
- `MEDELEG_DELEGATABLE`, `MIDELEG_DELEGATABLE`: `S`

The ACT `core.yaml` template must not carry them unconditionally. `gen_core.py`
emits the M-level interrupt flags when the config has `Zicsr`, and the
delegation arrays plus the S-level interrupt flags when it has `Sv32`.
