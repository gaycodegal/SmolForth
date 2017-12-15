#include "stack.h"

stack *new_stack(size_t item_size, size_t size){
  stack *s = NEW(stack);
  s->data = MALLOC(char, size);
  s->index = 0;
  s->length = size;
  s->item_size = item_size;
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
  printf("index: %ld\n", (s->index));
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
  ddata = dst->data + src->index;
  sdata = src->data + dst->index;
  times = n;
  while(times--){
    memcpy(ddata, sdata, minsize);
    ddata += disize;
    sisize += sisize;
  }
  src->index += n * sisize;
  dst->index += n * disize;
}

void *pop_stack(stack *s){
  if(s->index - s->item_size < 0){
    return NULL;
  }
  s->index -= s->item_size;
  return s->data + s->index;
}

void *get_stack(stack *s){
  printf("index: %ld\n", (s->index));
  if(s->index >= s->length)
    return NULL;
  ++(s->index);
  return s->data + s->index - 1;
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
