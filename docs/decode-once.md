# Decode-once principle

Every field of the instruction decoder union (`state->instruction.*`) must be read **exactly once** per executed instruction.

## Why

`state->instruction` lives in RAM and is loaded once per instruction via `RiscvEmulatorLoad()`.

Every subsequent field read is a bitfield extraction from RAM.

On AVR each 32-bit read costs multiple `ld` plus shift/mask instructions.

The compiler cannot reliably CSE these reads across call boundaries, because `state` is a pointer and callees may alias it.

Repeated field reads therefore become real, repeated decodes in the generated assembly.

## The rule

1. Decode each needed field once into a local variable.
2. Pass the decoded value down to callees as a function argument.
3. Never re-read `state->instruction.*` inside instruction handlers.
4. Always use the fields of the instruction decoder struct/union to extract values. Manual bit logic is forbidden: no `& 0b11111`, no `>> 5`, no re-shifting or re-masking of already decoded fields. If a value is not directly available as a field, add or use the appropriate decoder struct/union (e.g. `RiscvInstructionTypeSDecoderImm_u`, `RiscvInstructionTypeBDecoderImm_u`, `RiscvInstructionTypeUDecoderImm_u`, `RiscvInstructionTypeJDecoderImm_u`, `RiscvInstructionTypeRShift_t`, `RiscvInstructionTypeIShiftByConstant_t`).

   Exception: combining already-decoded fields into a single switch key (e.g. `funct7` + `funct3`, `op` + `funct3`) is done with a named compose helper (`RiscvEmulatorDecodeFunct7Funct3()` in `RiscvEmulatorTypeR.h`, `RiscvEmulatorDecodeOpFunct3()` in `RiscvEmulatorTypeC.h`, etc.) instead of a decoder union. GCC's `-Wconversion` warns for every runtime store of a wider integer into a narrower bitfield — even with a cast or mask — so the union idiom cannot be used for fields that were decoded into plain integers first. The shift/or lives in the decoder layer, written once, not at the call sites.

5. Decode a field in the main loop switch only when multiple cases share it (multi-consumer). A field consumed by exactly one handler is decoded inside that handler, so the handler code looks like it did before the decode-once refactor.

The main loop follows this rule: `rd`, `rs1`, `rs2`, `funct3`, `funct7` and the I-type `imm` are decoded once in `RiscvEmulatorLoop()` (`include/RiscvEmulator.h`) and passed down to the opcode handlers.

Single-consumer fields (S-imm, B-imm, J-imm, U-imm, `funct12`, CSR `imm`, `funct5`, `funct3_shifts`, `shamt`) are decoded inside their owning handler.

## Style for temporary variables

- Name the local after the decoded field, in the type it is used as: `uint8_t rdnum`, `uint8_t funct3`, `uint8_t funct7`, `int16_t imm`, `uint8_t shamt`, `uint16_t funct12`, `uint8_t funct3_shifts`.
- For values decoded via a decoder union, use a dedicated decoder variable named `immdecoder` (or `<field>decoder`), zero-initialized, with the bit fields assigned from the instruction struct and the final value read from the union:

  ```c
  RiscvInstructionTypeSDecoderImm_u immdecoder = {0};
  immdecoder.bit.imm4_0 = state->instruction.stype.imm4_0;
  immdecoder.bit.imm11_5 = state->instruction.stype.imm11_5;
  int16_t offset = immdecoder.imm;
  ```

- When a call site needs a value that is only decoded inside the handler, pass the raw decoder-struct field as the argument and let the handler decode it; do not duplicate the decode at the call site.
- Keep case bodies in the main loop as plain one-line calls; any extra decode needed only by that case goes into a local variable inside the case block.
