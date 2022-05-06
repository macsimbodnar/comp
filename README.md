# NASTY COMPILER

* Get the assembly with gcc: `gcc -S -O3 -fno-asynchronous-unwind-tables <filename>`, then `cat <filename>`
* Invoke the assembler: `gcc assembly.s -o out`
