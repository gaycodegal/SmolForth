#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdarg.h>
#include "words.h"

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
