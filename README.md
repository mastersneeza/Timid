## Timid

Timid is to be a maths language I guess...
it might do other things like Python or it might not...
but first thing will be maths because maths is annoying to manually do.

## Features
Timid can currently compile your simple maths expression (such as ```1 + 2 * 3```)
down into a custom bytecode for an interpreter written in C.
Timid currently only supports addition, subtraction, multiplication, division, exponents, modulus, grouping and numbers (+, -, *, /, ^, %)

## Syntax
Timid's syntax is currently using BIMDAS (or BODMAS, however you call it).
That is, it follows order of operations.
```(1 +-+ 2) * -3``` is valid, while
```(1 2 + 5 /``` is not

## Instructions
Just run ```> py Timid.py [filename]``` in the Terminal / Command line and it will compile
and run your script, displaying the output in the terminal.
Currently, Timid automatically prints the result, and only supports expressions, not full statements.

## Example
The program, ```(1 + 2) - 3 * 4 / 5 ^ 6 % 7```, stored in file ```example.timid```
will output ```> 2.77722``` when run with ```> py Timid.py example.timid```.
Try do other random programs or things. Try break TImid because its so bloody bad