# Chip 8

This project is a WIP implementation of a [Chip 8](https://en.wikipedia.org/wiki/CHIP-8) interpreter.

It contains:
- the core library of the chip 8 interpreter
- a simple linear disassembler
- a SDL application able to run some chip 8 ROMs

## Build

Requirements:
- C++ 20 compiler
- conan 2.x

```console
# on Linux, you might need to add these options:
#   -c tools.system.package_manager:mode=install
#   -c tools.system.package_manager:sudo=True
conan install . --build=missing
conan build .
```
