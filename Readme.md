# OCARM - A functionnal Ocaml VM on Nucleo

## Objectives
OCARM project aims to provide the more complete implementation of OCaml on Nucleo STM32. The actual version is working on boards, and allow to run complete OCaml program, without modules which are not available on Nucleos.
The modules which are not available are :
* Unix

## How to use
In the state of art, OCARM works, but need to be automated. However, with hand-compiling, everything works. It needs to compile the VM, compile the OCaml sources, and compile the rest to target the Nucleo.

## Interoperability with C
As OCaml allows to use C for certain functions, OCARM can also use C functions provided by the user. It allows the user to use the full potential of the Nucleo with low-level code and use it into his program in OCaml.
