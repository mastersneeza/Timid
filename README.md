## Timid

Timid is to be a maths language I guess...
it might do other things like Python or it might not...
but first thing will be maths because maths is annoying to manually do.

## Features
Timid is currently a general expression evaluator.
Timid supports numbers, booleans and null values.
It is able to do simple arithmetic, comparisons, equality, and more.
Timid compiles your expression down into a custom bytecode for an interpreter written in C, called TimidRuntime.
Timid currently only supports addition, subtraction, multiplication, division, exponents, modulus, grouping and numbers (+, -, *, /, ^, %)

## Syntax
Timid's syntax is currently using BIMDAS (or BODMAS, however you call it).
That is, it follows order of operations.
```(1 +-+ 2) * -3``` is valid, while
```(1 2 + 5 /``` is not

## Instructions
Windows users: run ```> py Timid.py``` in the terminal to get started
UNIX users: have GCC preinstalled, then go to the base directory and run ```$ gcc tVirtualMachine\*.c -o TimidRuntime```, then you can run ```$ py Timid.py``` from the base directory
Currently, Timid automatically prints the result, and only supports expressions, not full statements.

## Examples
Go to the ```Timid\Examples``` directory and run the programs in there and try to mess around.
Try do other random programs or things. Try break Timid because its so bloody bad