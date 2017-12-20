#ifndef STACK_H
#define STACK_H
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PUTINT(STACK, VAL) put_stack((STACK), (VAL), sizeof(int))
#define PUTCHAR(STACK, VAL) put_stack((STACK), (VAL), sizeof(char))
#define PUTSTR(STACK, VAL) put_stack((STACK), (VAL), sizeof(char *))
#define GETINT(STACK) ((int *)get_stack((STACK)))
#define GETSTR(STACK) ((char **)get_stack((STACK)))
#define POPINT(STACK) ((int *)pop_stack((STACK)))

typedef struct s_stack {
  char *data;
  size_t index;
  size_t length;
  size_t item_size;
} stack;

/**
   creates a new stack with data of length <size>
   where each item in the stack is at max
   <item_size> bytes big.
*/
stack *new_stack(size_t item_size, size_t size);

/**
   frees a stack
*/
void free_stack(stack *s);

/**
   puts some data onto the stack,
   incrementing index
   
   does not do anything if there is no space
*/
void put_stack(stack *s, void *p, size_t size);

/**
   gets some data from the stack,
   decrementing index

   returns null if there's nothing to pop
 */
void *pop_stack(stack *s);

/**
   gets some data from the stack
   index remains static

   returns null if index == length
 */
void *get_stack(stack *s);

/**
   copies <n> many things from <source> into <dest>
   increments both stacks by <n> * stack->item_size
   if item sizes differ, uses smaller as transfer unit.
*/
void stack_copy(stack *dest, stack *source, size_t n);


/**
   circular rotate the stack starting at
   <stk->index> <amt> places to the right
   inside the range <stk->index> (inclusive) to
   <stk->index> + <len> (exclusive)
 */
void shift_stack(stack *stk, long len, long amt);

/**
   prints out the contents of a stack as integers;
 */
void print_stack(stack *s);

/**
   some little tests
 */
int stack_main();


#endif
