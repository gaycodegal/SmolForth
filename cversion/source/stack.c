#include "stack.h"

stack *new_stack(size_t size){
  stack *s = NEW(stack);
  s->data = MALLOC(char, size);
  s->index = 0;
  s->length = size;
}

void put_stack(stack *s, void *p, size_t size){
  if(size + s->index > s->length){
    return;
  }
  memcpy(s->data + s->index, p, size);
  s->index += size;
  printf("index: %ld\n", (s->index));
}

void *pop_stack(stack *s, size_t size){
  if(s->index - size < 0){
    return NULL;
  }
  s->index -= size;
  return s->data + s->index;
}

void *get_stack(stack *s){
  printf("index: %ld\n", (s->index));
  if(s->index >= s->length)
    return NULL;
  return s->data + s->index;
}

int stack_main(){
  stack *s = new_stack(30);
  memset(s->data, 0, 30);
  int x = 32;
  PUTINT(s, &x);
  int pop = *POPINT(s);
  int get = *GETINT(s);
  printf("'%s' %i %i \n", s->data, get, pop);
  return 0;
}
