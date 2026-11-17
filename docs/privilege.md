# Privilege modes and supervisor CSRs

The emulator implements privilege mode 0 (U-mode), 1 (S-mode) and 3 (M-mode). The board starts the hart in M-mode at `0x80000000`, as the manual requires (`machine.adoc`, `m_mode_priv_at_reset`).

## mstatus

`mstatus.MPP` holds two bits and accepts 0, 1 and 3. `mstatus.SIE`, `SPIE`, `SPP`, `SUM`, `MXR`, `TW`, `TSR` and `TVM` are writable. `mstatus.MPRV` makes loads and stores use `MPP` as the effective privilege; instruction fetches always use the real privilege. `MRET` and `SRET` clear `MPRV` when they return to a mode less privileged than M-mode.

A write to `sstatus` preserves every `mstatus` bit outside the sstatus write mask. `senvcfg` is a read/write CSR with only FIOM writable. `sepc` and `mepc` clear the low bits per IALIGN; with IALIGN=32 the two low bits are always zero.

## Delegation

`medeleg` and `mideleg` are writable. A synchronous exception goes to S-mode when the matching `medeleg` bit is set and the active privilege is S-mode or U-mode. An interrupt goes to S-mode when the matching `mideleg` bit is set and the active privilege is S-mode or U-mode. All other traps enter M-mode.

`medeleg` accepts bits 0 to 8, 12, 13, 15, 18 and 19. Bit 9 (ECALL from S-mode) and bit 11 (ECALL from M-mode) are not delegated. `mideleg` accepts bits 1, 5 and 9.

## Supervisor CSRs

| CSR | Address | Notes |
| --- | --- | --- |
| `sstatus` | 0x100 | View on `mstatus`; SIE, SPIE, SPP, FS, XS, SUM, MXR, UBE and SD |
| `sie` | 0x104 | View on `mie`, masked by `mideleg` |
| `stvec` | 0x105 | Direct and vectored mode |
| `scounteren` | 0x106 | Separate 32-bit storage with a `0x7` write mask; Linux writes 0x2 in `head.S` |
| `sscratch` | 0x140 | Full 32-bit storage |
| `sepc` | 0x141 | Low bit reads as zero |
| `scause` | 0x142 | Interrupt bit 31 and cause code |
| `stval` | 0x143 | Fault address or instruction |
| `sip` | 0x144 | View on `mip`, masked by `mideleg` |
| `satp` | 0x180 | MODE bit 31 (WARL: Bare and Sv32), ASID bits 30:22, PPN bits 21:0 |

OpenSBI sets `mcounteren` to 0x7 before it enters S-mode, so S-mode can read `cycle`, `time` and `instret`. `scounteren` is a separate CSR with a `0x7` write mask that controls U-mode counter access. The user counters `0xC00`-`0xC82` are read-only shadows of the machine counters; a `csrrw` to them raises an illegal instruction.

## SRET

`SRET` restores `SIE` from `SPIE`, sets `SPIE`, clears `SPP`, sets the privilege from `SPP` and jumps to `sepc`. `SRET` raises an illegal-instruction exception when the active privilege is below S-mode, or when `mstatus.TSR` is set and the active privilege is S-mode.

## SFENCE.VMA and satp

`SFENCE.VMA` is decoded and treated as a no-op because the implementation has no address-translation cache. It raises an illegal-instruction exception when `mstatus.TVM` is set and the active privilege is S-mode. `satp` raises the same exception under the same condition.

## WFI

`WFI` parks the hart for one idle iteration and then resumes, in every privilege mode. The implementation completes the wait within the bounded time limit, so `mstatus.TW` does not raise an illegal-instruction exception. The platform declares `WFI_FINITE`, and the ACT `cp_instret_wfi_timeout` test expects the WFI to return.

## ECALL

`ECALL` from U-mode raises cause 8 and follows `medeleg`. `ECALL` from S-mode raises cause 9 and follows `medeleg`; bit 9 is not delegated, so the trap enters M-mode. OpenSBI handles the call there. `ECALL` from M-mode raises cause 11.
