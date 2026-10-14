# Decode-once principle

Every field of the instruction decoder union (`state->instruction.*`) must be
read **exactly once** per executed instruction.

## Why

`state->instruction` lives in RAM and is loaded once per instruction via
`RiscvEmulatorLoad()`. Every subsequent field read is a bitfield extraction
from RAM. On AVR each 32-bit read costs multiple `ld` plus shift/mask
instructions. The compiler cannot reliably CSE these reads across call
boundaries, because `state` is a pointer and callees may alias it. Repeated
field reads therefore become real, repeated decodes in the generated
assembly.

## The rule

1. Decode each needed field once into a local variable.
2. Pass the decoded value down to callees as a function argument.
3. Never re-read `state->instruction.*` inside instruction handlers.

The main loop follows this rule: `rd`, `rs1`, `rs2`, `funct3` and `funct7`
are decoded once in `RiscvEmulatorStep()` (`include/RiscvEmulator.h`) and
passed down to the opcode handlers.

