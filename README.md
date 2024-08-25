# Status

In progress.

# Description

This my try at a RV32 RISC-V cpu emulator. I wrote it to better understand RISC-V instructions.

The emulator has been written to be primarily run on 8-bit AVR microcontrollers and compiles with GCC (GNU Compiler Collection) and be as small and somewhat fast as possible. It contains no dependencies on any CPU architecture. To prove independence I also got the emulator to run on a 32-bit ATSAMD21G18A and [x86-64 Linux and Windows](https://github.com/atoomnetmarc/RISC-V-emulator-Native).

# Goal

Try to implement [RV32GC](https://en.wikipedia.org/wiki/RISC-V#ISA_base_and_extensions). That means the base 32-bit integer instruction set RV32I and the extensions M, A, F, D, Zicsr, Zifencei and C.

# Goal progress and bonus extensions

Some extensions are work-in-progress™ and do not work yet. Others are confirmed working.

Extensions are confirmed working when all instructions of that extension pass [riscv-arch-test](https://github.com/riscv-non-isa/riscv-arch-test) made by the RISC-V Foundation Architecture Test SIG using [atoomnetmarc/RISC-V-emulator-RISCOF](https://github.com/atoomnetmarc/RISC-V-emulator-RISCOF).

| Description                         | Extension | Subextension  | Goal  | Working?                |
| :--                                 | :--       | :--           | :--   | :--                     |
| Base Integer Instruction Set 32-bit | RV32I     |               |       | :ballot_box_with_check: |
| Integer Multiplication and Division | M         |               |       | :ballot_box_with_check: |
| Atomics                             | A         |               |       | :ballot_box_with_check: |
| Single-Precision Floating-Point     | F         |               |       | :x:                     |
| Double-Precision Floating-Point     | D         |               |       | :x:                     |
| Control and Status Register Access  | Zicsr     |               |       | :x:                     |
| Instruction-Fetch Fence             | Zifencei  |               |       | :ballot_box_with_check: |
| Compressed Instructions             | C         |               |       | :x:                     |
| Address calculation                 | B         | Zba           | Bonus | :ballot_box_with_check: |
| Basic bit manipulation              | B         | Zbb           | Bonus | :ballot_box_with_check: |
| Carry-less multiplication           | B         | Zbc           | Bonus | :ballot_box_with_check: |
| Single-bit operation                | B         | Zbs           | Bonus | :ballot_box_with_check: |

The emulator can be configured to the enable specific extensions. The list of defines and their default values are located in [include/RiscvEmulatorConfig.h](include/RiscvEmulatorConfig.h).

For example, to enable the `M` extension compile with `-DRVE_E_M=1`.

# Workings

Instruction decoding is done with packed bitfield structs. When bits need to be untangled I used a union of 2 helper structs instead of trying to shift all the bits into the correct places.
In a first pass the opcode of the instruction is processed in a `switch()` located in `RiscvEmulatorLoop()` and roughly split into their instruction groups (like R-Type, I-Type, etc..). When needed in a second nested `switch()` the instruction is decoded and the operation is executed.

# Your implementation

The emulator needs some implementation specific code in a file called `RiscvEmulatorImplementationSpecific.h` that you must program yourself in your own project:

```c
/**
 * Loads a 32-bit RISC-V instruction from the specified address.
 *
 * @param address The address in bytes of where to read the RISC-V instruction.
 * @return 32-bit RISC-V instruction.
 */
inline uint32_t RiscvEmulatorLoadInstruction(uint32_t address)
{
}

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
 * Something wrong happened at the current programcounter.
 * Use a listing file of the risc-v program to better understand the wrong.
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

I do not know if this library will stay in the same shape or form.

I used this library in Microchip Studio to be able debug using debugwire and JTAG on AVR.

Use in PlatformIO, `platformio.ini`:
```ini
[env:myenv]
lib_deps =
  https://github.com/atoomnetmarc/RISC-V-emulator.git
```

# Hooks

Enabling hooks `-D RVE_E_HOOK=1` creates the possibility to tap into the inner workings of the emulator. I added this functionality to be used in [RISC-V-emulator-Native](https://github.com/atoomnetmarc/RISC-V-emulator-Native) for debugging. Enabling hooks will create some overhead because empty weak functions will be called,

In `RiscvEmulatorHook.h` you can find a list of all the supported hooks in the form of weak functions. Implement your own non-weak function to start using the hooks you are interested in. For example:

```c
void RiscvEmulatorCSRRWHookBegin(RiscvEmulatorState_t *state, void *rd, const void *rs1, void *csr) {

    printf("%08x: %08x          csrrw rd: %u, rs1: %u, csr: %u\n",
           state->programcounter,
           state->instruction.value, *(uint32_t *)rd, *(uint32_t *)rs1, *(uint32_t *)csr);
}
```

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
