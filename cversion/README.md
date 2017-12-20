# C Forth Version

## Example Programs

Programs are currently injected into the main function in main.c for testing purposes.

Basic if statement testing - `: TESTIF 0 + if 30 else 40 then ; 1 TESTIF 0 TESTIF + .` yields the output:

    70
    ok

Basic recursion test - `: FIB dup 1 <= if else dup 1 - FIB rot 2 - FIB + then ; 6 FIB .` yields the output:

    8
    ok



## Compilation

I've been working in linux (Ubuntu xenial). To compile simply run `make` in the cversion directory.

## Running

Once the program is compiled, it can be run with the command `./main` in the cversion folder.

## Notes

Currently need to make the compiled output stable enough to be possibly saved to a file. File extension will likely by `.c4` for compiled Forth. `.c4` files will be specific to the platform they are compiled on, and thus should never be included in a build.

In order to make this Forth implementation usable, we need to do something about the pointers I leave lying around to unassigned words. For now, we can safely ignore SYMSTR because they will never end up being used in runtime - only for compile time function naming conventions.

The intended purpose for this Forth implementation is for using in formatting content. The only forseeable usage of strings would be for dealing with image file names, but I'm comfortable ignoring those, as in the desired system, Lua/C++ should be the one handling string content. Perhaps however, SYMSTRs should simply be thrown in after the defs section of the code just on the off chance that they make life easier.

I can't see why for-loops would be useful, but I should include them just in case.

## Error Handling (Future)

Include an active error type (raised) and a (passive) error type. When a function is called, the user can decide to call it with errors on, and should an error occur, the function will skip its ret value and jump to the error handler. Error handlers, however, would need to be cleared before ret is called so that the program would not jump to the error handler immediately. I suppose that alternatively, we could have the ret jump to the error handler, and the error handler's first instruction would be to check for an active error raised and if no active errors, call ret itself. So something like a sym-ret-if-no-error SYMNERET.

## Future Work

Features needed to make this a useful implementation include:
- Stack swapping.
- Duplication:
    - Duplicate top element.
    - Duplicate indexed element.
    - Duplicate element range?
- Userdata (a simple pointer type).
- Some meaningful API to specify additional runtime functions and compile based on those.
- Some concept of constants.
- Anonymous functions (useful for having multiple execution starting points, as will happen with the intended usecase).

## Done
- Stack rotation (rotate n elements by m places).

