#include "words.h"

static const struct s_runfn builtins [] = {
  {rAdd, "+"},
  {rSub, "-"},
  {rDot, "."},
  {rDup, "dup"},
  {rRot, "rot"},
  {rNRot, "nrot"},
  {rPop, "pop"},
  {rNPop, "npop"},
  {rEQ, "=="},
  {rLT, "<"},
  {rGT, ">"},
  {rLE, "<="},
  {rGE, ">="},
  {rStack, "stack"},
  {NULL, NULL}
};

const struct s_runfn *get_builtins(){
  return builtins;
}

compfn *make_comp_ptr(compfn fn){
  compfn *p = NEW(compfn);
  *p = fn;
  return p;
}

int *make_int_ptr(int i){
  int *p = NEW(int);
  *p = i;
  return p;
}

compfn *get_comp(map_t *map, char *key){
  any_t p;
  int status = hashmap_get(map, key, &p);
  if(status != MAP_OK){
    return NULL;
  }
  return (compfn *)p;
}

int *get_int(map_t *map, char *key){
  any_t p;
  int status = hashmap_get(map, key, &p);
  if(status != MAP_OK){
    return NULL;
  }
  return (int *)p;
}

void rLT(stack *stk, stack *links){
  int *ta;
  int a;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta < a;
  PUTINT(stk, &a);
}

void rGT(stack *stk, stack *links){
  int *ta;
  int a;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta > a;
  PUTINT(stk, &a);
}

void rLE(stack *stk, stack *links){
  int *ta;
  int a;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta <= a;
  PUTINT(stk, &a);
}

void rGE(stack *stk, stack *links){
  int *ta;
  int a;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta >= a;
  PUTINT(stk, &a);
}

void rEQ(stack *stk, stack *links){
  int *ta;
  int a;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta == a;
  PUTINT(stk, &a);
}

void rSub(stack *stk, stack *links){
  int *ta;
  int a;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta - a;
  PUTINT(stk, &a);
}

void rDup(stack *stk, stack *links){
  int *tv;
  stk->index -= stk->item_size;
  tv = GETINT(stk);
  if(tv == NULL)
    return;
  PUTINT(stk, tv);
}

void rNRot(stack *stk, stack *links){
  int *t;
  int length, delta;
  size_t ind = stk->index;
  t = POPINT(stk);
  if(t == NULL)
    return;
  delta = *t;
  t = POPINT(stk);
  if(t == NULL)
    return;
  length = *t;
  stk->index -= length * stk->item_size;
  shift_stack(stk, length, delta);
  stk->index = ind;
}

void rRot(stack *stk, stack *links){
  int *t;
  int v1, v2;
  stk->index -= stk->item_size * 2;
  t = GETINT(stk);
  if(t == NULL)
    return;
  v1 = *t;
  t = GETINT(stk);
  if(t == NULL)
    return;
  v2 = *t;
  stk->index -= stk->item_size * 2;
  PUTINT(stk, &v2);
  PUTINT(stk, &v1);
}

void rPop(stack *stk, stack *links){
  POPINT(stk);
}

void rNPop(stack *stk, stack *links){
  int *t;
  long amt;
  t = POPINT(stk);
  if(t == NULL)
    return;
  amt = (long)*t;
  amt = (long)(stk->index)  - amt * (long)(stk->item_size);
  //printf("index now : %i\n", (int)stk->index);
  if(amt < 0)
    stk->index = 0;
  else if(amt > stk->length)
    stk->index = stk->length;
  else
    stk->index = amt;
}

void rStack(stack *stk, stack *links){
  printf("stack (%i): ", (int)(stk->index / stk->item_size)), print_stack(stk);
}
    
void cIf(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap){
  int t = 0;
  PUTINT(stk, &t);
  PUTINT(stk, &t);
  t = stk->index;
  PUTINT(links, &t);
}
    
void cElse(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap){
  int t = 0;
  PUTINT(stk, &t);
  PUTINT(stk, &t);
  t = stk->index;
  PUTINT(links, &t);
}

void cThen(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap){
  int t;
  int *tv;
  int top = stk->index;
  int mid;
  int bottom;
  tv = POPINT(links);
  if(tv == NULL)
    return;
  mid = *tv;
  tv = POPINT(links);
  if(tv == NULL)
    return;
  bottom = *tv;
  stk->index = bottom - 2 * stk->item_size;
  t = SYM_RBRANCH;
  PUTINT(stk, &t);
  t = (mid - bottom) / stk->item_size;
  PUTINT(stk, &t);

  stk->index = mid - 2 * stk->item_size;
  t = SYM_RJUMP;
  PUTINT(stk, &t);
  t = (top - mid) / stk->item_size;
  PUTINT(stk, &t);

  stk->index = top;
}    

void cColon(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap){
  int top = (stk->index);
  PUTINT(links, &top);
}

void cSemiColon(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap){
  int *t;
  size_t top = stk->index;
  size_t bottom;
  size_t blocks;
  char *name;
  char **st;
  t = POPINT(links);
  if(t == NULL)
    return;
  bottom = *t;
  blocks = (top - bottom)/(stk->item_size);
  stk->index = bottom;
  if(blocks < 2){
    stk->index = bottom;
    return;
  }
  t = GETINT(stk);
  if(t == NULL || *t != SYM_STR){
    stk->index = bottom;
    return;
  }
  st = GETSTR(stk);
  if(st == NULL){
    stk->index = bottom;
    return;
  }
  name = *st;
  hashmap_put(rmap, name, make_int_ptr(defs->index));
  stack_copy(defs, stk, blocks - 2);
  int retsym = SYM_RET;
  PUTINT(defs, &retsym);
  stk->index = bottom;
  //memset(stk->data + stk->index, 0, stk->item_size * blocks);
}

void rAdd(stack *stk, stack *links){
  int *ta;
  int a;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta;
  ta = POPINT(stk);
  if(ta == NULL)
    return;
  a = *ta + a;
  PUTINT(stk, &a);
}

void rDot(stack *stk, stack *links){
  int *tv = POPINT(stk);
  if(tv == NULL)
    return;
  int a = *tv;
  stk->index += stk->item_size;  
  printf("%i\n", a);
}
