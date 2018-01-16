#ifndef _WORDS_H_
#define _WORDS_H_

#include "stack.h"
#include "hashmap.h"

enum ForthByteCode{SYM_HALT = 0, SYM_RUCALL, SYM_RCALL, SYM_CUCALL, SYM_CCALL, SYM_RET, SYM_INT, SYM_STR, SYM_RBRANCH, SYM_RJUMP};

/**
   a run-time Forth function
 */
typedef void (*runfn)(stack *stk, stack *links);

/**
   a compile-time Forth function 
 */
typedef void (*compfn)(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);

struct s_runfn {
  runfn fn;
  char *name;
};

size_t STACK_ITEM_SIZE;

/**
   returns the list of builtin functions
 */
const struct s_runfn *get_builtins();

/**
   makes a pointer to a compile-time function
 */
compfn *make_comp_ptr(compfn fn);

/**
   gets an int from a map
 */
int *get_int(map_t *map, char *key);

/**
   gets a compile time function from a map
 */
compfn *get_comp(map_t *map, char *key);

/**
   makes a pointer to an integer
 */
int *make_int_ptr(int i);

/**
   adds the top two ints on the stack
   possibly might want to make it add
   any top two things
 */
void rAdd(stack *stk, stack *links);

/**
   preforms operation of <a> and <b>
   args: 
   -1: b
   -2: a
   result:
   -1: a - b
 */
void rSub(stack *stk, stack *links);

/**
   prints the top (int) of the stack.
 */
void rDot(stack *stk, stack *links);

/**
   prints the entire stack and stack index
 */
void rStack(stack *stk, stack *links);

/**
   duplicates the top of the stack
 */
void rDup(stack *stk, stack *links);

/**
   rotates the top two elements
   of the stack
 */
void rRot(stack *stk, stack *links);

/**
   rotates the top <length> elems
   <delta> places.
   args: 
   -1: delta
   -2: length
 */
void rNRot(stack *stk, stack *links);

/**
   pops the top element off the stack
   by decrementing the stack pointer
 */
void rPop(stack *stk, stack *links);

/**
   pops the top <length> elems off the stack
   by decrementing the stack pointer
   args: 
   -1: length
 */
void rNPop(stack *stk, stack *links);

/**
   preforms operation of <a> and <b>
   args: 
   -1: b
   -2: a
   result:
   -1: a == b
 */
void rEQ(stack *stk, stack *links);

/**
   preforms operation of <a> and <b>
   args: 
   -1: b
   -2: a
   result:
   -1: a < b
 */
void rLT(stack *stk, stack *links);

/**
   preforms operation of <a> and <b>
   args: 
   -1: b
   -2: a
   result:
   -1: a > b
 */
void rGT(stack *stk, stack *links);

/**
   preforms operation of <a> and <b>
   args: 
   -1: b
   -2: a
   result:
   -1: a <= b
 */
void rLE(stack *stk, stack *links);

/**
   preforms operation of <a> and <b>
   args: 
   -1: b
   -2: a
   result:
   -1: a >= b
 */
void rGE(stack *stk, stack *links);

/**
   starts function generation process
 */
void cColon(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);

/**
   ends function generation process
 */
void cSemiColon(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);

/**
   Starts an if statement
 */
void cIf(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);

/**
   Signifies split between both code routes
   in an if statement
 */
void cElse(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);

/**
   Compiles the if statement and updates code with
   jump positions and commands
 */
void cThen(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);


#endif
