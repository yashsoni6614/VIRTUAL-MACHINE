# rurvm — an LC-3 Virtual Machine in C

A from-scratch emulator for the LC-3 (Little Computer 3) architecture, written in C. It implements the full LC-3 instruction set, memory-mapped keyboard I/O, and all six trap routines, and has been tested against real compiled LC-3 programs, including two full games.

This project implements the **emulator** — the virtual CPU that executes LC-3 machine code. It does not include an assembler; see [Writing your own programs](#writing-your-own-programs) below for how to turn your own LC-3 assembly into something this VM can run.

## What is LC-3?

LC-3 is a simplified 16-bit computer architecture designed for teaching how CPUs actually work — how instructions are encoded into bits, how a processor fetches and executes them one at a time, and how a CPU talks to memory and I/O devices. It's the subject of Yale Patt and Sanjay Patel's *Introduction to Computing Systems*, and this project is my implementation of a virtual machine that behaves like real LC-3 hardware, entirely in software.

## Specifications

| | |
|---|---|
| Word size | 16-bit |
| Address space | 65,536 memory locations (64K), each storing 16 bits |
| Registers | 8 general-purpose registers (R0–R7), 1 program counter, 1 condition flag register |
| Condition flags | Positive (P), Zero (Z), Negative (N) — set after every instruction that writes a register |
| Instruction set | 16 opcodes (see below) |
| I/O | Memory-mapped keyboard status/data registers, polled non-blocking input |

### Supported instructions

| Opcode | Name | Description |
|---|---|---|
| `ADD` | Add | Add two registers, or a register and a 5-bit immediate |
| `AND` | Bitwise AND | AND two registers, or a register and a 5-bit immediate |
| `NOT` | Bitwise NOT | Complement a register |
| `BR` | Branch | Conditional branch based on N/Z/P flags |
| `JMP` | Jump | Unconditional jump to address in a register |
| `JSR` / `JSRR` | Jump to Subroutine | Call a subroutine, saving return address in R7 |
| `LD` | Load | Load from a PC-relative address |
| `LDI` | Load Indirect | Load from the address stored at a PC-relative address |
| `LDR` | Load Register | Load from a base register + offset |
| `LEA` | Load Effective Address | Load a computed address into a register |
| `ST` | Store | Store to a PC-relative address |
| `STI` | Store Indirect | Store to the address stored at a PC-relative address |
| `STR` | Store Register | Store to a base register + offset |
| `TRAP` | Trap | Invoke a system call (see below) |

### Trap routines

| Trap | Vector | Description |
|---|---|---|
| `GETC` | `x20` | Read a single character from the keyboard, no echo |
| `OUT` | `x21` | Write a single character to the console |
| `PUTS` | `x22` | Write a null-terminated string to the console |
| `IN` | `x23` | Prompt for and read a character, with echo |
| `PUTSP` | `x24` | Write a null-terminated string packed two characters per word |
| `HALT` | `x25` | Stop execution |

## Installation

Clone the repository, or download it as a ZIP from GitHub and extract it:

```
git clone https://github.com/<yashsoni6614>/rurvm.git
cd rurvm
```

You'll need `gcc` installed. Compile with:

```
gcc -o rurvm rurvm.c platform.c
```

This produces an executable named `rurvm`.

## Usage

Run any LC-3 object file by passing it as an argument:

```
./rurvm object_files/hello.obj
```

### Try the included programs

The `object_files/` folder ships with five pre-compiled `.obj` files you can run immediately, no assembler required:

- **`2048.obj`** and **`rogue.obj`** — two full games, included to demonstrate the VM handling real, non-trivial programs. Screenshots below.
- Three smaller demo programs (`hello.obj`, `echo.obj`, `count.obj`) written specifically for this project to exercise individual parts of the instruction set and trap routines in isolation.

The LC-3 assembly source for the three demo programs is in `asm_files(demo)/`, so you can read exactly what each one does, or use it as a starting point for writing your own.

**2048**
<br>
*<img width="702" height="341" alt="image" src="https://github.com/user-attachments/assets/5dd14b50-4474-44b3-b09e-b84d6b74baef" />
*

**Rogue**
<br>
*<img width="461" height="361" alt="image" src="https://github.com/user-attachments/assets/7d396e58-293e-4ed8-aa94-a11bb77977b9" />
*

## Writing your own programs

This repository does not include an assembler. If you want to write your own LC-3 assembly and generate an `.obj` file the VM can run, I'd recommend the **[LC-3 Assembler](https://github.com/paul-nameless/lc3-asm)**. It requires Python 3.



## Acknowledgements

The reference used while building this emulator was Justin Meiners and Ryan Pendleton's [LC-3 VM tutorial](https://github.com/justinmeiners/lc3-vm). I followed its structure while implementing this project, wrote and debugged the code myself, and later restructured it into the multi-file, cross-platform layout in this repository. I'm noting this openly rather than presenting the project as designed from a blank page.
