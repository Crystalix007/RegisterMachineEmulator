# Register Machine Emulator

## What is a register machine?

A register machine is a computing model capable of performing all algorithms possible on modern machines (i.e. it is Turing-complete).

It works by having a finite number of registers, which can store an integer of any length.
By using a [valid encoding scheme](https://en.wikipedia.org/wiki/G%C3%B6del_numbering), this number can be thought of representing either an instruction, or a value, or a list, or all at once.

## How do you encode programs?

Well, assuming we have a way to encode lists, then we can encode programs as lists of instructions.
To encode an instruction, we deliberately limit the available number of instructions:

* `0` = `HALT` - stops the program
* `R+ -> L` - increments register `R`, and subsequently jumps to label `L`
* `R- -> L1, L2` - decrements register `R` if greater-than `0`, jumping to `L1`. Otherwise (if `R` == `0`), jump to `L2`.

To convert these instructions into numbers, we first convert the instructions into tuples:

* `R+ -> L` ==> <`R`, `L`>
* `R- -> L1, L2` ==> <`R`, <`L1`, `L2`>>

Here, `R+` instructions are encoded as a pair, and `R-` instructions are encoded as a pair of an integer and a pair.

To distinguish between incrementing and decrementing instructions, we then encode the `R+` instruction register as an even number (2 * `R`).
And `R-` instructions have an odd-numbered register (2 * `R` + 1).

This all relies on tuples having an encoding, so for further details, see the code.

# Building this emulator.

This requires a working \*nix installation. That should include Linux, the BSDs and MacOS.

Ensure you have [`Tup`](https://github.com/gittup/tup) installed, or use the included `build.sh` script.

You will also require `gcc`, `gmp` and `gmpxx`. `gmpxx` is probably included with `gmp` on your distribution.

## Using `Tup`.

`tup init`
`tup`

## Using the build script.

`./build.sh`

## Running it.

You should now be able to run the `./Emulator` executable.

Run `./Emulator`, and enter a number at the prompt for a program (no calculation allowed).
