# Interrupts and delegation

## mip and mie

`mip` and `mie` gain bits 1 (SSIP), 3 (MSIP), 5 (STIP), 7 (MTIP), 9 (SEIP) and 11 (MEIP). `mip.SSIP`, `mip.STIP` and `mip.SEIP` are writable. `mip.MSIP`, `mip.MTIP` and `mip.MEIP` are read-only. `mip.MSIP` is set by the CLINT `MSIP` register, `mip.MTIP` by the CLINT `mtimecmp` register and `mip.MEIP` by the platform interrupt controller.

`mip.SEIP` is split into a software-writable bit and a platform signal. A read returns the OR and a write only changes the software bit. `sip.SEIP` is read-only.

## Platform sources

The platform interrupt controller is a custom CSR pair: `0x8C0` sets and `0x8C1` clears the software-set platform bits in `mip_platform_software`. The device tick writes the device-driven bits in `mip_platform`, so the two sources do not overwrite each other. A `mip` read and the interrupt selection use the OR of `mip`, `mip_platform` and `mip_platform_software`.

The board also has a memory-mapped simple interrupt generator at `0x0C400000` for the same purpose. Bit 31 selects set; the low bits are the `mip` bits.

## Delegation

`mideleg` accepts bits 1, 5 and 9. `medeleg` accepts the S-mode exception bits: 0 to 8, 12, 13, 15, 18 and 19. Bit 9 (ECALL from S-mode) and bit 11 (ECALL from M-mode) are not delegated.

## Priority and selection

The interrupt selection checks the non-delegated interrupts first, then the delegated ones, because interrupts to M-mode take priority over interrupts to S-mode. Within each group the priority is MEI, MSI, MTI, SEI, SSI, STI. A delegated interrupt enters S-mode through `stvec`, `scause`, `sepc` and `sstatus` only when the active privilege is S-mode with `sstatus.SIE` set or U-mode; it never enters M-mode.

## ACT macros

The ACT `RVMODEL_SET_SEXT_INT`, `RVMODEL_CLR_SEXT_INT` and `RVMODEL_SET_SSW_INT` macros are injected through `# act-define` in the `.ini` generator. The ACT template stays generic and the emulator `.ini` owns the platform macro. The macros use `csrw` to the platform CSRs, so the M-, S- and U-mode tests raise MEI and SEI without T-SBI or the identity map.
