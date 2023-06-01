
# HAL For nRF52833 SoC

This project is a c-language static library intended to be used as a
HAL (Hardware Abstraction Layer) for the [nRF52833](https://infocenter.nordicsemi.com/pdf/nRF52833_OPS_v0.7.pdf)
Sistem on Chip.

## Components

Each public header of the library exposes functionalities to control different
memory mapped peripherals available to the SoC.

## Build

You can build the library from the source code by creating a build folder inside
the souce tree, cd into it, and then running
```
cmake .. && cmake --build .
```
If you want to install the library in custom destination you can run (from the
build directory)
```
cmake --install . --prefix <custom-destination-path>
```
The default destination path relative to the build directory is `../install`.
In alternative you can also download the release install package.

