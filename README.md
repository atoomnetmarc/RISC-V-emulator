# Status

Done. It works for me. Goal reached. Good enough™. Feeling satisfied with the results.

Maybe I will add some more extensions when I feel like it. No, I am not going to add floating point support. I tried, then disposed of the non-working code.

# Description

This is my attempt at an RV32 RISC-V CPU emulator. I wrote it to better understand RISC-V instructions.

The emulator has been written primarily to run on 8-bit AVR microcontrollers and to compile with GCC (GNU Compiler Collection), with the goal of being as small and somewhat fast as possible. It contains no dependencies on any specific CPU architecture. To prove independence, I also got the emulator running on a 32-bit ATSAMD21G18A as well as on [x86-64 Linux and Windows](https://github.com/atoomnetmarc/RISC-V-emulator-Native).

# Goal

Implement [RV32I](https://en.wikipedia.org/wiki/RISC-V#ISA_base_and_extensions) and a bunch of extensions. All while simultaneously learning how it all works.

Some extensions are work in progress and do not work yet. Others are confirmed working.

Extensions are confirmed working when all instructions of that extension pass the [riscv-arch-test](https://github.com/riscv/riscv-arch-test), made by the RISC-V Foundation Architecture Test SIG, run via [atoomnetmarc/RISC-V-emulator-ACT](https://github.com/atoomnetmarc/RISC-V-emulator-ACT).

| Description                                  | Extension   | Subset | Working?                |
| :------------------------------------------- | :---------- | ------ | :---------------------- |
| Base Integer Instruction Set 32-bit          | RV32I       |        | :ballot_box_with_check: |
| Integer Multiplication and Division          | M           |        | :ballot_box_with_check: |
| Multiplication subset                        | M           | Zmmul  | :ballot_box_with_check: |
| Atomic Memory Operations                     | A           | Zaamo  | :ballot_box_with_check: |
| Load-Reserved/Store-Conditional Instructions | A           | Zalrsc | :ballot_box_with_check: |
| Atomic Compare-and-Swap                      | Zacas       |        | :ballot_box_with_check: |
| Compressed Instructions                      | C           |        | :ballot_box_with_check: |
| Address calculation                          | B           | Zba    | :ballot_box_with_check: |
| Basic bit manipulation                       | B           | Zbb    | :ballot_box_with_check: |
| Carry-less multiplication                    | B           | Zbc    | :ballot_box_with_check: |
| Carry-less multiplication for Cryptography   | B           | Zbkc   | :ballot_box_with_check: |
| Bit manipulation for Cryptography            | B           | Zbkb   | :ballot_box_with_check: |
| Byte and bit permutation for Cryptography    | B           | Zbkx   | :ballot_box_with_check: |
| Single-bit operation                         | B           | Zbs    | :ballot_box_with_check: |
| Additional compressed instructions           | C           | Zcb    | :ballot_box_with_check: |
| Compressed May-Be-Operations                 | Zcmop       |        | :ballot_box_with_check: |
| May-Be-Operations                            | Zimop       |        | :ballot_box_with_check: |
| Integer Conditional Operations               | Zicond      |        | :ballot_box_with_check: |
| Counter                                      | Zicntr      |        | :ballot_box_with_check: |
| Non-Temporal Locality Hint                   | Zihintntl   |        | :ballot_box_with_check: |
| Pause Hint                                   | Zihintpause |        | :ballot_box_with_check: |
| Control and Status Register Access           | Zicsr       |        | :ballot_box_with_check: |
| Instruction-Fetch Fence                      | Zifencei    |        | :ballot_box_with_check: |

The emulator can be configured to enable specific extensions and subsets. The list of defines and their default values is located in [include/RiscvEmulatorConfig.h](include/RiscvEmulatorConfig.h).

For example, to enable the `M` extension, compile with `-DRVE_E_M=1`.

# Extension exclusion

These extensions do not make sense in some way to add to the emulator and will not be implemented.

| Extension | Reason                                             |
| :-------- | :------------------------------------------------- |
| Zicbom    | Cache-block management; the emulator has no cache. |
| Zicbop    | Cache prefetch hints; the emulator has no cache.   |
| Zicboz    | Cache-block zeroing; the emulator has no cache.    |
| Zihpm     | Hardware performance monitors.                     |

# Testing

Typically only the configuration with all extensions enabled is tested using the riscv-arch-test during development of this emulator. [Testing all combinations is possible](https://github.com/atoomnetmarc/RISC-V-emulator-ACT/blob/main/scripts/test_all.sh).

As a sanity check, all combinations of extensions and subsets will be compiled. See [platformio_isa-extension-combination_env.ini](https://github.com/atoomnetmarc/RISC-V-emulator-Native/blob/main/platformio_isa-extension-combination_env.ini). Compilation of all these combinations takes about 5 minutes on my fastest machine.

# Workings

Instruction decoding is done with packed bitfield structs. When bits need to be untangled, I use a union of two helper structs instead of trying to shift all the bits into the correct places.
In a first pass, the opcode of the instruction is processed in a `switch()` located in `RiscvEmulatorLoop()` and roughly split into its instruction groups (like R-Type, I-Type, etc.). When needed, in a second nested `switch()`, the instruction is decoded and the operation is executed.

# Your implementation

The emulator needs some implementation specific code in a file called `RiscvEmulatorImplementationSpecific.h` that you must program yourself in your own project:

```c
/**
 * Loads bytes from emulator to RISC-V.
 *
 * @param address The byte address in memory.
 * @param destination The destination address to copy the data to.
 * @param length The length in bytes of the data.
 */
inline void RiscvEmulatorLoad(uint32_t address, void *destination, uint8_t length)
{
}

/**
 * Stores bytes from RISC-V to emulator.
 *
 * @param address The byte address in memory.
 * @param source The source address to copy the data from.
 * @param length The length in bytes of the data.
 */
inline void RiscvEmulatorStore(uint32_t address, const void *source, uint8_t length)
{
}

/**
 * Handles a fault where the instruction is somehow not recognized.
 *
 * Something wrong happened at the current program counter.
 * Use a listing file of the RISC-V program to better understand the issue.
 * The failed machine instruction is found in state.instruction.value.
 */
inline void RiscvEmulatorIllegalInstruction(RiscvEmulatorState_t *state)
{
}

#if (RVE_E_ZICSR == 1)
/**
 * Handles a fault where the CSR is not recognized.
 */
inline void RiscvEmulatorUnknownCSR(RiscvEmulatorState_t *state) {
}
#endif

/**
 * Handles an ECALL.
 */
inline void RiscvEmulatorHandleECALL(RiscvEmulatorState_t *state)
{
}

/**
 * Handles an EBREAK.
 */
inline void RiscvEmulatorHandleEBREAK(RiscvEmulatorState_t *state) {
}
```

Your own program should provide some RAM, initialize the emulator, and then keep calling `RiscvEmulatorLoop()`. For inspiration:

```c
#include <RiscvEmulator.h>

uint8_t memory[RAM_LENGTH];

RiscvEmulatorState_t RiscvEmulatorState;

int main(void)
{
    RiscvEmulatorInit(&RiscvEmulatorState, sizeof(memory));

    for (;;)
    {
        RiscvEmulatorLoop(&RiscvEmulatorState);
    }
}
```

I do not know if this library will remain in its current form.

I used this library in Microchip Studio to be able to debug using debugWIRE and JTAG on AVR.

Use in PlatformIO, `platformio.ini`:

```ini
[env:myenv]
lib_deps =
  https://github.com/atoomnetmarc/RISC-V-emulator.git
```

# Hooks and disassembly

The emulator provides two independent, opt-in features for tapping into its inner workings:

- **`RVE_E_HOOK`** — a generic weak hook called before and after every instruction. Use it for custom side effects: counting instructions, writing the mnemonic to an LCD, logging, etc.
- **`RVE_E_DISASM`** — built-in disassembly rendering. When enabled, the emulator prints a human-readable trace of every instruction (mnemonic, operands, and results) via a weak `RiscvEmulatorDisasmPrintf` function that you override to choose the output channel.

Both default to `0` (disabled). When disabled, none of the associated code is compiled in — zero code-size cost. They are fully independent: you can enable either, both, or neither.

## Generic hook (`RVE_E_HOOK`)

Enable with `-D RVE_E_HOOK=1`. Implement your own non-weak `RiscvEmulatorHook` to receive a `RiscvEmulatorHookContext_t` before (`hook == HOOK_BEGIN`) and after (`hook == HOOK_END`) every instruction:

```c
void RiscvEmulatorHook(
    const RiscvEmulatorState_t *state,
    const RiscvEmulatorHookContext_t *context) {
    // Example: write the mnemonic to an LCD.
    lcd_puts(context->instruction);
}
```

The context contains the instruction name, register numbers/pointers/names, immediate, memory location, and more. You choose how much to use.

## Disassembly rendering (`RVE_E_DISASM`)

Enable with `-D RVE_E_DISASM=1`. The emulator then calls a set of built-in `static inline` rendering functions that format each instruction and emit text via a weak `RiscvEmulatorDisasmPrintf`. Override that function to route the output. Because a weak definition cannot be overridden in the same translation unit, also define `RVE_DISASM_PRINTF_OVERRIDE` before including the library:

```c
#define RVE_DISASM_PRINTF_OVERRIDE 1
#include <RiscvEmulator.h>
#include <stdarg.h>

void RiscvEmulatorDisasmPrintf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}
```

Only the rendering functions for the extensions you have enabled are compiled in; the rest are eliminated as dead code. You pay only for what you use.

### On AVR (keeping format strings in flash)

AVR has little SRAM and string literals normally get copied to RAM at startup. To keep the disassembly format strings in flash (zero RAM cost), define `RVE_DISASM_FMT(s)` as `PSTR(s)` **before** `RiscvEmulatorDisasm.h` is included. Since a `-D` build flag with parentheses does not survive the shell, the cleanest place is the project's `RiscvEmulatorImplementationSpecific.h`, which every library translation unit includes before the disassembly header anyway:

```c
#define RVE_DISASM_FMT(s) PSTR(s)
```

```c
void RiscvEmulatorDisasmPrintf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf_P(stdout, fmt, ap);
    va_end(ap);
}
```

### Combining both

You can enable both `RVE_E_HOOK` and `RVE_E_DISASM`. The generic hook is called first (for your custom side effects), then the disassembly renderer prints the full trace. For example, on AVR you could write the mnemonic to a TFT via the hook while also emitting a full trace to UART via disassembly.

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
