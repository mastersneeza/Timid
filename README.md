## Timid

Timid is to be a maths language I guess...
it might do other things like Python or it might not...
but first thing will be maths because maths is annoying to manually do.

## Features
Timid can currently compile your simple maths expression (such as ```1 + 2 * 3```)
down into a custom bytecode for an interpreter written in C.
Timid currently only supports addition, subtraction, multiplication, division, grouping and numbers

## Syntax
Timid's syntax is currently using BIMDAS (or BODMAS, however you call it).
That is, it follows order of operations.
```(1 +-+ 2) * -3``` is valid, while
```(1 2 + 5 /``` is not

## Instructions
Just run the ```main.py``` file with the path to your file(s) and it will compile
them in a file with the same name but with the ```.timb``` extension.