# Sv32 ACT environments

The Sv32 environments run the S-mode and Sv32 coverpoints of `riscv-arch-test` on the emulator. The ISA behaviour they check is in [privilege.md](privilege.md), [sv32.md](sv32.md) and [interrupts.md](interrupts.md); this document records the harness and the configuration.

## Environments

| Environment | ISA |
| --- | --- |
| `RV32IZicntr_Zicsr_Sv32` | I, Zicntr, Zicsr, Sv32 |
| `RV32IMAZicntr_Zicsr_Zifencei_Sv32` | I, M, A, Zicntr, Zicsr, Zifencei, Sv32 |
| `RV32IMACBZicntr_Zicsr_Zifencei_Sv32` | I, M, A, C, B, Zicntr, Zicsr, Zifencei, Sv32 |

The coverpoints are `coverpoints/priv/S_coverage.svh`, `coverpoints/priv/Svade_coverage.svh` and `coverpoints/priv/Sstvala_coverage.svh`.

## Harness

- `act4` commit `1c3e9f2a` with the per-chunk trap signature counts and one temporary trap-handler-width patch. A global `TRAP_SIGUPD_COUNT` cannot serve both the small interrupt tests and the large `medeleg` walk; upstream #2505 sizes the region per test chunk.
- The container applies only the trap-handler patch. The patch fixes a framework bug that affects tests outside the excluded suites.
- `link.ld` places `.text.rvmodel` directly after `.text.rvtest`, ahead of `.rodata` and `.data`. The signature in `.data` pushes `.text.rvmodel` beyond the `R_RISCV_JAL` range for `InterruptsS-00`; the reorder keeps the call in range without an ACT source patch.
- `gen_core.py` sets `fs_legal_states` and `vs_legal_states` to `ExtContext_Off` when F and V are absent, so the reference config does not declare features the DUT lacks.
- The `-t` trap trace is the primary diagnostic for the `Sv_Exceptions` group: it logs every trap with `scause`, `sepc` and `stval`, so the first unexpected trap is visible without the console.

## The U-mode exclusion

The boot identity map is not user accessible. Tests that run U-mode code without `begin_sv_test` trap-loop in the reference model. The Sv32 environments exclude these tests through `# act-exclude-extensions` in the `.ini`:

- `Exceptions` (the whole suite)
- `Sstvala_s` and `Sstvala_u`
- `U` (the whole suite)
- `S_Zicntr`
- `Sv_sv32_satp_access_Smode`

`Sv_sv32_satp_access_Smode` is excluded because `_t_satp_access` uses `begin_test_chunk` and does not map the U-mode alias.

## Configuration-driven behaviour

| Behaviour | Rule |
| --- | --- |
| Misaligned AMO | The config declares `MISALIGNED_AMO: false`; the manual makes the priority relative to page faults implementation-defined, and the config uses low priority. |
| Misaligned LR/SC | The config declares `LRSC_MISALIGNED_BEHAVIOR: always raise access fault`; the check precedes the page walk so the access fault wins over a page fault. |
| `menvcfg.FIOM` | Writable, because `satp` is implemented. |
| `marchid` | Reads 1, as the generated config declares `archid = 1`. |
| `mhpmcounter3` | Writable, as the generated config declares. |
| RV32 upper-half CSRs | `0x312`-`0x314`, `0x318`-`0x319` and `0x31C`-`0x31F` are read-only zero: `medelegh`, `midelegh`, `mieh`, `mvienh`, `mviph` and `mstateen0h`-`mstateen3h` are zero on RV32. |
| `misa` C bit | Read-only set; `cp_misa_clear_c` expects the bit to stay set when software clears it. |
| CLINT `mtime` | Advances by the host-clock delta, so a software write survives the next tick; `cp_mtime_write` writes `mtime` and reads the `time` CSR. |
| `mcycle` and `minstret` write | The written value is the final counter value and the writing instruction does not increment the counter; `cp_minstret_wraparound` expects it. |
| Trapping instruction | Does not increment `minstret`; `cp_instret_ecall` expects the ecall not to count as a retired instruction. |
| Zicfiss | Outside the emulator's scope; the Zicfiss tests are excluded. |

## Open items

- Run the Svade and Sstvala coverages on the three Sv32 environments.
