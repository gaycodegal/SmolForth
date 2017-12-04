# SmolForth

A little forth interpreter in python. Will soon move to C or C++

## Forth Bytecode Structure

1. An int, indicating the length of the function definition section
2. The funciton definition section
3. The main program body

### Forth Bytecode Commands

SYMHALT
- Ends program execution

SYMRUCALL
- Calls a user defined function

SYMRCALL
- Calls a system function

SYMCUCALL
- Calls a user defined compile time function

SYMCCALL
- Calls a system compile time function

SYMRET
- Return from a function

SYMINT
- An integer

SYMSTR
- A string

SYMRBRANCH
- relative branch if zero

SYMRJUMP
- relative jump if zero
