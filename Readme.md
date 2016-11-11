# OCARM — A functionnal Ocaml VM on Nucleo

## Context
OCARM project has been written during an internship at both the INRIA and the UPMC. Using [OCaPIC](http://www.algo-prog.info/ocapic/web/index.php?id=ocapic) as an inspiration, OCARM is focusing on Nucleo STM32.

## Objectives
OCARM project aims to provide the more complete implementation of OCaml on Nucleo STM32. The actual version is working on boards, and allow to run complete OCaml program, without modules which are not available on Nucleos.
The modules which are not available are :
* Unix

## Requirements
While Nucleo STM32 uses many commercial IDE working with Windows, we are using the original firmware of the Nucleo and its library (HAL, i.e. High Abstraction Level) with OpenOCD (Open On Chip Debugger) in order to push the code onto the board. The original ARM compiler provided by ARM (ARM GCC) and the Red Hat Newlib are also used to compile the code.

## Choices
The VM is working by turning the OCaml code into bytecode, exporting it into the program, packing it with the VM, and pushing it onto the board. 
- The bytecode is exported into the code as an array of bytes.
- Every data is exactly the same as in the OCaml VM, i.e. 32 bits, with standard OCaml Header. A good solution would be to reduce the size of each data (i.e. on 16 bits) to gain a lot of memory.
- The Garbage Collector is one of our own, i.e. a simpler GC (stop & copy) in order to maximize performances of the VM. It will probably have to be rewrote in the future.
- The HAL modules are implemented to manipulate the Nucleo.

## How to use
In the state of art, OCARM works, but need to be automated. However, with hand-compiling, everything works. It needs to compile the VM, compile the OCaml sources, and compile the rest to target the Nucleo.

The compiling chain follows :
```bash
gcc -c stm32f3xx_hal_ml.c
ocaml -custom stm32.ml stm32f3xx_hal_ml.o my_file.ml *.ml -o my_prog
elf2bc my_prog -o my_prog_bc
ocamlclean my_prog_bc -o my_prog_clean
bin2c my_prog_clean -o bytecode.h
modinc *.ml
make
make program
```

It is possible to find the different files in the correct folders. This schema is an obligatory passage, for the linker to correctly link the different file into one executable. `my_file.ml` and `*.ml` represent the user files written by the user.  
`elf2bc`, `modinc`and `bin2c`are utilities which can be found in the appropriate utils folder. `ocamlclean` is an utility written during the OCaPIC project to clean the bytecode.  
The compiling chain is approximative, because files are dependent each other. However, it is important to compile files in the good order, one after the other.

## Interoperability with C
As OCaml allows to use C for certain functions, OCARM can also use C functions provided by the user. It allows the user to use the full potential of the Nucleo with low-level code and use it into his program in OCaml.

We hope that you will find it useful, because both Darius Mercadier and I will not touch the project anymore. For any questions, do not hesitate to send me a mail, I will help you as much as I can.
