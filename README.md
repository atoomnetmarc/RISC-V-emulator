This my try at a RISC-V cpu emulator. I wrote it to better understand RISC-V instructions.

It can emulate rv32i and is known to be incomplete, however it seems to be able to execute machine code generated by gcc for a variety of machine instructions.

The emulator has been written to be primarily run on 8-bit AVR microcontrollers and be as small and somewhat fast as possible. It contain no dependencies on any architecture. To prove independence I also got it to run on a 32-bit ATSAMD21G18A.

Instruction decoding is done with packed bitfield structs. When bits need to be untangled I used a union of 2 helper structs instead of trying to shift all the bits into the correct places.
In a first pass the opcode of the instruction is processed in a `switch()`. When needed in a second nested `switch()` the instruction is decoded and the operation is executed.

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

I used this library in Microchip Studio to be able debug using debugwire and JTAG and PlatformIO.

platformio.ini:
```ini
[env:myenv]
lib_deps =
  https://github.com/atoomnetmarc/RISC-V-emulator.git
```

[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
