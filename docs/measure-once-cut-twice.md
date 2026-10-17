# Measure once, cut twice

**Rule:** Never judge a size change from a single build. Record a baseline of every environment before touching code, re-measure all environments after, and decide on the per-environment delta table — never on the total alone.

## Why

1. **The total hides regressions.** A −500 total can hide a +200 regression in one environment. Only the per-env table shows who pays and who wins.
2. **The compiler redistributes, it does not just shrink.** A change in one handler shifts register allocation across the whole translation unit; unrelated environments can grow or shrink. Only a full re-measure makes that visible (see the ACT_DISASM allocation cascade in the C-funnel session).
3. **A baseline goes stale.** Register allocation cascades mean a baseline measured before unrelated commits is worthless. Record a fresh baseline at the start of every session and per candidate when the tree moved.

## How

1. **Baseline once per session**, before the first edit: build every environment and record the size table. Save it in `/tmp/baseline_<session>.txt`. For the AVR repo:

   ```
   cd ../RISC-V-emulator-AVR && pio run
   python3 - <<'EOF' > /tmp/baseline_<session>.txt
   import glob
   total = 0
   for f in sorted(glob.glob('hex/*.hex')):
       # Sum the Intel HEX data records: flash bytes actually used.
       size = sum(int(l[1:3], 16) for l in open(f) if l[7:9] == '00')
       name = f.split('/')[-1][:-4]
       print(f'{size:8d}  {name}'); total += size
   print(f'TOTAL {total}')
   EOF
   ```
2. **One candidate at a time.** Implement, rebuild all environments, diff against the baseline per environment and in total. Save the delta table in `/tmp/candidateN_<name>.txt`.
3. **Accept only with zero regressions** in the decision set. Environments that are debug-only or not representative (e.g. ACT_DISASM) are still measured but excluded from the decision — record their delta anyway, with the reason for exclusion.
4. **Trust numbers only after inspecting the assembly.** Check the `.lss` of a representative plain and a debug environment: is the shared body emitted once, is there one dispatch chain, no double dispatch? A win with the wrong shape (e.g. duplicated loads inside switch cases) will not survive the next compiler update.
5. **Revert on regression.** A candidate that regresses any decision-set environment is reverted, not argued away. Save the delta table as the record of why.

## Pitfalls

- **Layout shift is not growth.** Comparing `.lss` addresses between builds diverges at the first moved function; diff on landmarks (string literals, call sites), not on addresses.
- **Debug environments amplify allocation cascades.** A funnel that wins −100 in plain envs can cost +1000 in a disassembly build because `main()` is register-starved there. That is acceptable only when the environment is explicitly out of the decision set.
- **Measure the same thing.** Same parser, same environment set, same build flags — a baseline from a different configuration is not a baseline.
