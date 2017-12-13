# C Forth Version

Currently need to build a stack that has the same size used for ints and void pointers, just to make life easier. Should just compare sizeof(void *) and sizeof(int) (and sizeof(float) I guess)  and see which is larger and use that. This allows things like strings and userdata (to use the Lua terminology) to be objects while ints and floats (and bools and chars) can occupy the same space on the stack and not have to be custom allocated.
