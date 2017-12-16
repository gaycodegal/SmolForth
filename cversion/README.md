# C Forth Version

Currently need to make the compiled output stable enough to be possibly saved to a file. File extension will likely by `.c4` for compiled Forth. `.c4` files will be specific to the platform they are compiled on, and thus should never be included in a build.

In order to make this Forth implementation usable, we need to do something about the pointers I leave lying around to unassigned words. For now, we can safely ignore SYMSTR because they will never end up being used in runtime - only for compile time function naming conventions.

The intended purpose for this Forth implementation is for using in formatting content. The only forseeable usage of strings would be for dealing with image file names, but I'm comfortable ignoring those, as in the desired system, Lua/C++ should be the one handling string content. Perhaps however, SYMSTRs should simply be thrown in after the defs section of the code just on the off chance that they make life easier.

I can't see why for-loops would be useful, but I should include them just in case.

Thus, features needed to make this a useful implementation include:
- Stack swapping.
- Stack rotation (rotate n elements by m places).
- Duplication:
    - Duplicate top element.
    - Duplicate indexed element.
    - Duplicate element range?
- Userdata (a simple pointer type).
- Some meaningful API to specify additional runtime functions and compile based on those.
- Some concept of constants.
- Anonymous functions (useful for having multiple execution starting points, as will happen with the intended usecase).