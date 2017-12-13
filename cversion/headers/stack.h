#ifndef STACK_H
#define STACK_H
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PUTINT(STACK, VAL) put_stack((STACK), (VAL), sizeof(int))
#define PUTCHAR(STACK, VAL) put_stack((STACK), (VAL), sizeof(char))
#define GETINT(STACK) ((int *)get_stack((STACK)))
#define POPINT(STACK) ((int *)pop_stack((STACK), sizeof(int)))

typedef struct s_stack {
  char *data;
  size_t index;
  size_t length;
} stack;

/**
   creates a new stack with data of length <size>
*/
stack *new_stack(size_t size);

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
void *pop_stack(stack *s, size_t size);

/**
   gets some data from the stack
   index remains static

   returns null if index == length
 */
void *get_stack(stack *s);

/**
   some little tests
 */
int stack_main();


#endif
