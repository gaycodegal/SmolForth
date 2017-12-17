#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include "stack.h"
#include <stdarg.h>
#include "hashmap.h"

enum ForthByteCode{SYMHALT = 0, SYMRUCALL, SYMRCALL, SYMCUCALL, SYMCCALL, SYMRET, SYMINT, SYMSTR, SYMRBRANCH, SYMRJUMP};
size_t STACK_ITEM_SIZE;

/**
   a compile-time Forth function 
 */
typedef void (*compfn)(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);

/**
   a run-time Forth function
 */
typedef void (*runfn)(stack *stk, stack *links);

/**
   adds the top two ints on the stack
   possibly might want to make it add
   any top two things
 */
void rAdd(stack *stk, stack *links);

/**
   prints the top (int) of the stack.
 */
void rDot(stack *stk, stack *links);

/**
   makes a pointer to an integer
 */
int *make_int_ptr(int i);

/**
   splits a string (on spaces) into a NULL terminated
   array of strings.
 */
char **split_words(char *source);

/**
   frees a list of words generated
   by split_words
 */
void free_words(char **words);

/**
   starts function generation process
 */
void cColon(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);

/**
   ends function generation process
 */
void cSemiColon(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);

/**
   makes a pointer to a compile-time function
 */
compfn *make_comp_ptr(compfn fn);

/**
   gets an int from a map
 */
int *get_int(map_t *map, char *key);

/**
   takes in a iterable which returns the separated words
   and compiles it into bytecode
*/
stack *compile(char ** source);

/**
   meant to do things like enable recursion
   and handle strings
 */
stack *secondpass(stack *dataspace, map_t *rmap);

/**
   Takes a bytecode compiled forth program definition <dataspace>
   forth program definition structure described in the README
*/
void interp(stack *dataspace);

/**
   computes the max of multiple ints. I was just too lazy so this happened.
 */
int max_int(int n, ...);

#endif
