# Description

This my try at a RV32 RISC-V cpu emulator. I wrote it to better understand RISC-V instructions.

The emulator has been written to be primarily run on 8-bit AVR microcontrollers and be as small and somewhat fast as possible. It contains no dependencies on any architecture. To prove independence I also got it to run on a 32-bit ATSAMD21G18A and [x86-64 Linux](https://github.com/atoomnetmarc/RISC-V-emulator-Native).

## Extensions

Overview of all the RV32 contained in this emulator. Some are work-in-progress™ and do not work yet. Others are confirmed working.

Extensions are confirmed working when all instructions of that extension pass [riscv-arch-test](https://github.com/riscv-non-isa/riscv-arch-test) made by the RISC-V Foundation Architecture Test SIG.

| Subset                              | Name  | Working?                |
| --                                  | --    | --                      |
| Integer                             | I     | :ballot_box_with_check: |
| Integer Multiplication and Division | M     | :ballot_box_with_check: |
| Atomics                             | A     | :ballot_box_with_check: |
| Control and Status Register Access  | Zicsr | :x:                     |
| Address calculation                 | Zba   | :ballot_box_with_check: |
| Basic bit manipulation              | Zbb   | :ballot_box_with_check: |
| Carry-less multiplication           | Zbc   | :x:                     |
| Single-bit operation                | Zbs   | :x:                     |

The emulator can be configured the enable an extension. The list of defines and their default values are located in [include/RiscvEmulatorConfig.h](include/RiscvEmulatorConfig.h).

To enable a feature compile with `-DRVE_E_M=1` to enable the `M` extension.

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
 * @param destination The destination address to copy the data to.
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
inline void RiscvEmulatorUnknownInstruction(RiscvEmulatorState_t *state)
{
}

/**
 * Handles a fault where the CSR is not recognized.
*/
inline void RiscvEmulatorUnknownCSR(RiscvEmulatorState_t *state)
{
}

/**
 * Handles an ECALL.
 */
inline void RiscvEmulatorHandleECALL(RiscvEmulatorState_t *state)
{
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

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
