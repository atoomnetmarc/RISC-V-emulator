# Status

Done. It works for me. Goal reached. Good enough™. Feeling satisfied with the results.

Maybe I will add some more extensions when I feel like it. No, I am not going to add floating point support. I tried, then disposed of the non-working code.
> Does this spark joy? If it does, keep it. If not, dispose of it.
>
> [Marie Kondō (近藤麻理恵)](https://en.wikipedia.org/wiki/Marie_Kondo)

# Description

This is my attempt at an RV32 RISC-V CPU emulator. I wrote it to better understand RISC-V instructions.

The emulator has been written primarily to run on 8-bit AVR microcontrollers and to compile with GCC (GNU Compiler Collection), with the goal of being as small and somewhat fast as possible. It contains no dependencies on any specific CPU architecture. To prove independence, I also got the emulator running on a 32-bit ATSAMD21G18A as well as on [x86-64 Linux and Windows](https://github.com/atoomnetmarc/RISC-V-emulator-Native).

# Goal

Implement [RV32I](https://en.wikipedia.org/wiki/RISC-V#ISA_base_and_extensions) and a bunch of extensions. All while simultaneously learning how it all works.

Some extensions are work in progress and do not work yet. Others are confirmed working.

Extensions are confirmed working when all instructions of that extension pass [riscv-arch-test](https://github.com/riscv-non-isa/riscv-arch-test) made by the RISC-V Foundation Architecture Test SIG using [atoomnetmarc/RISC-V-emulator-RISCOF](https://github.com/atoomnetmarc/RISC-V-emulator-RISCOF).

| Description                         | Extension | Working?                |
| :--                                 | :--       | :--                     |
| Base Integer Instruction Set 32-bit | RV32I     | :ballot_box_with_check: |
| Integer Multiplication and Division | M         | :ballot_box_with_check: |
| Atomics                             | A         | :ballot_box_with_check: |
| Compressed Instructions             | C         | :ballot_box_with_check: |
| Control and Status Register Access  | Zicsr     | :ballot_box_with_check: |
| Instruction-Fetch Fence             | Zifencei  | :ballot_box_with_check: |
| Address calculation                 | Zba       | :ballot_box_with_check: |
| Basic bit manipulation              | Zbb       | :ballot_box_with_check: |
| Carry-less multiplication           | Zbc       | :ballot_box_with_check: |
| Single-bit operation                | Zbs       | :ballot_box_with_check: |

The emulator can be configured to enable specific extensions. The list of defines and their default values are located in [include/RiscvEmulatorConfig.h](include/RiscvEmulatorConfig.h).

For example, to enable the `M` extension compile with `-DRVE_E_M=1`.

> [!NOTE]
> At the time of writing this emulator needs the C-extension to pass the riscv-arch-test for Zicsr. See https://github.com/riscv-non-isa/riscv-arch-test/issues/445

# Testing

Typically only the configuration with all extensions enabled is tested using the mentioned riscv-arch-test during development of this emulator.

As a sanity check, all combinations of extensions will be compiled. At the time of writing this sentence there are 512 combinations of 9 extensions, see [platformio_isa-extension-combination_env.ini](https://github.com/atoomnetmarc/RISC-V-emulator-Native/blob/main/platformio_isa-extension-combination_env.ini). Compilation of these 512 combinations takes about 5 minutes on my fastest machine.

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

Your own program should provide some RAM and should initialize the emulator. Then keep calling `RiscvEmulatorLoop()`. For inspiration:

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

I do not know if this library will remain in its current shape or form.

I used this library in Microchip Studio to be able to debug using debugWIRE and JTAG on AVR.

Use in PlatformIO, `platformio.ini`:
```ini
[env:myenv]
lib_deps =
  https://github.com/atoomnetmarc/RISC-V-emulator.git
```

# Hooks

Enabling hook `-D RVE_E_HOOK=1` creates the possibility to tap into the inner workings of the emulator. I added this functionality for use in [RISC-V-emulator-Native](https://github.com/atoomnetmarc/RISC-V-emulator-Native) for debugging.

Implement your own non-weak function to start using the hook. For example:

```c
void RiscvEmulatorHook(
    const RiscvEmulatorState_t *state,
    const RiscvEmulatorHookContext_t *context) {
}
```

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
