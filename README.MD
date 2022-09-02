# Chip 8

This project is a WIP implementation of a [Chip 8](https://en.wikipedia.org/wiki/CHIP-8) interpreter.

It contains:
- the core library of the chip 8 interpreter
- a simple linear disassembler
- a SDL application able to run some chip 8 ROMs

## Build

Requirements:
- C and C++ compiler
- Cmake v3.20 or later

```console
cmake -B build
cmake --build build
```