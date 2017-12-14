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
  return s->data + s->index;
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
