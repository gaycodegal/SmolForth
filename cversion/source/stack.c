#include "stack.h"

stack *new_stack(size_t item_size, size_t size){
  stack *s = NEW(stack);
  s->data = MALLOC(char, (size) * item_size);
  memset(s->data, 0, (size) * item_size);
  s->index = 0;
  s->length = size * item_size;
  s->item_size = item_size;
  return s;
}

void free_stack(stack *s){
  free(s->data);
  free(s);
}

void put_stack(stack *s, void *p, size_t size){
  if(size > s->item_size || s->item_size + s->index > s->length){
    return;
  }
  memcpy(s->data + s->index, p, size);
  s->index += s->item_size;
  //printf("index: %ld\n", (s->index));
}

void stack_copy(stack *dst, stack *src, size_t n){
  size_t disize;
  size_t sisize;
  size_t minsize;
  size_t times;
  char *sdata;
  char *ddata;
  sisize = src->item_size;
  disize = dst->item_size;
  if(src->index + n * sisize > src->length || dst->index + n * disize > dst->length){
    return;
  }
  minsize = sisize;
  if(sisize > disize)
    minsize = disize;
  ddata = dst->data + dst->index;
  sdata = src->data + src->index;
  times = n;
  while(times--){
    memcpy(ddata, sdata, minsize);
    ddata += disize;
    sdata += sisize;
  }
  src->index += n * sisize;
  dst->index += n * disize;
}

void *pop_stack(stack *s){
  if(s->index < s->item_size){
    return NULL;
  }
  s->index -= s->item_size;
  return s->data + s->index;
}

void print_stack(stack *s){
  for(size_t i = 0; i < s->length; i += s->item_size){
    printf("%i ", *(int *)(s->data + i));
  }
  printf("\n");
}

void *get_stack(stack *s){
  //printf("index: %ld\n", (s->index));
  if(s->index >= s->length)
    return NULL;
  s->index += s->item_size;
  return s->data + s->index - s->item_size;
}

void shift_stack(stack *stk, long len, long amt) { //smaller of 2 shift
  size_t d, s, left, check, k, mx;
  char t;
  char * ary = stk->data;
  size_t start = stk->index;
  if(stk->index >= stk->length)
    return;
  len *= stk->item_size;
  if(len > stk->length - stk->index){
    len = stk->length - stk->index;
  }
  amt *= stk->item_size;
  amt = ((amt % len) + len) % len;
  if (amt == 0)
    return;
  d = len - amt;
  s = 0;
  while (s < d) {
    left = d - s;
    check = amt > left;
    if (check) {
      amt = left;
    }
    for (k = d, mx = amt + d; k < mx; ++k) {
      //swapping
      t = ary[start + s];
      ary[start + s] = ary[start + k];
      ary[start + k] = t;
      ++s;
    }
    if (check) {
      d += amt;
      amt = len - d;
    }
  }
  return;
}

int stack_main(){
  stack *s = new_stack(sizeof(int), 30);
  memset(s->data, 0, 30);
  int x = 32;
  PUTINT(s, &x);
  int pop = *POPINT(s);
  int get = *GETINT(s);
  printf("'%s' %i %i \n", s->data, get, pop);
  return 0;
}
