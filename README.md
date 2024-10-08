# TM4C USB-to-TTL adapter
Use a TM4C123GXL eval board as a uart passthru device. Serial data is sent from a host to the TM4C
and the TM4C sends the data out on a separate UART. Similarly, any data received on the separate
UART is forwarded to the host.

`Host <--> TM4C <--> serial device`

## Included Make Recipes
1. `all`: build both an ELF and a flat binary.

2. `clean`: delete build artifacts.

3. `flash`: flash the target with the binary.

## Debugging
The repo includes a script `debug.sh` for debugging the target using `arm-none-eabi-gdb`. 

## Dependencies

[OpenOCD](https://github.com/openocd-org/openocd.git) is necessary for debugging the remote target,
 and [lm4flash](https://github.com/utzig/lm4tools.git) is used to flash it. Both the debug script 
and the makefile are coupled with the `arm-none-eabi` toolchain.

