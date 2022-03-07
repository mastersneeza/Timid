## Timid

Timid is to be a maths language I guess... <br/>
it might do other things like Python or it might not...<br/>
but first thing will be maths because maths is annoying to manually do.<br/>

## Features
Timid is currently a general expression evaluator.<br/>
Timid supports numbers, booleans and null values.<br/>
Timid also supports arithmetic, comparisons and equality ```+ - * / ^ % == != < <= > >= !``` (try guess which ones they do!)<br/>
It is able to do simple arithmetic, comparisons, equality, and more.<br/>
Timid compiles your expression down into a custom bytecode for an interpreter written in C, called TimidRuntime.<br/>

## Syntax
Timid's syntax currently follows C's order of operations for expressions.<br/>

## Instructions
Windows users: set ```<Timid Install Location>\Timid``` as your working directory<br/>
then run ```<Timid Install Location>\Timid> py Timid.py``` in the terminal to get started.<br/>
UNIX users: have GCC preinstalled, then run ```$ gcc ~/Timid/tVirtual*.c -o ~/Timid/TimidRuntime```.<br/>
Then, run  ```$ python ~/Timid/Timid.py``` to get started<br/>
Currently, Timid automatically prints the result, and only supports expressions, not full statements.<br/>

## Examples
Go to the ```~Timid/Examples``` directory and run the programs in there and try to mess around.<br/>
For example, after setup, try ```> python ~\Timid\Timid.py ~\Timid\Examples\example.timid``` on Windows or ```$ python ~/Timid/Timid.py ~/Timid/Examples/example.timid``` on UNIX systems<br/>
Try do other random programs or things. Try break Timid because its so bloody bad.<br/>
