#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include "stack.h"
#include <stdarg.h>
#include "hashmap.h"

enum ForthByteCode{SYMHALT = 0, SYMRUCALL, SYMRCALL, SYMCUCALL, SYMCCALL, SYMRET, SYMINT, SYMSTR, SYMRBRANCH, SYMRJUMP};
size_t STACK_ITEM_SIZE;

typedef void (*compfn)(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);

typedef void (*runfn)(stack *stk, stack *links);

void rAdd(stack *stk, stack *links);
int *make_int_ptr(int i);

#endif
