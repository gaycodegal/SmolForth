#ifndef _INTERP_H_
#define _INTERP_H_
#include "words.h"

#define MAX(L, R) (L) > (R) ? (L) : (R)

/**
   splits a string (on spaces) into a NULL terminated
   array of strings.
 */
char **split_words(const char *source);

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
   Interprets code. Only response is printed output.
 */
void interpret(const char * words);

/**
   read and interpret code. only response is printed output.
 */
void repl();

#endif
