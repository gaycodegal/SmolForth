#include "main.h"

runfn *rundefs;

struct s_runfn {
  runfn fn;
  char *name;
};

void rLT(stack *stk, stack *links){
  int a = *POPINT(stk);
  a = *POPINT(stk) < a;
  PUTINT(stk, &a);
}

void rGT(stack *stk, stack *links){
  int a = *POPINT(stk);
  a = *POPINT(stk) > a;
  PUTINT(stk, &a);
}

void rLE(stack *stk, stack *links){
  int a = *POPINT(stk);
  a = *POPINT(stk) <= a;
  PUTINT(stk, &a);
}

void rGE(stack *stk, stack *links){
  int a = *POPINT(stk);
  a = *POPINT(stk) >= a;
  PUTINT(stk, &a);
}

void rEQ(stack *stk, stack *links){
  int a = *POPINT(stk);
  a = *POPINT(stk) == a;
  PUTINT(stk, &a);
}

void rSub(stack *stk, stack *links){
  int a = *POPINT(stk);
  a = *POPINT(stk) - a;
  PUTINT(stk, &a);
}

void rDup(stack *stk, stack *links){
  int v;
  stk->index -= stk->item_size;
  v = *GETINT(stk);
  PUTINT(stk, &v);
}

void rNRot(stack *stk, stack *links){
  int length, delta;
  size_t ind = stk->index;
  delta = *POPINT(stk);
  length = *POPINT(stk);
  stk->index -= length * stk->item_size;
  shift_stack(stk, length, delta);
  stk->index = ind;
}

void rRot(stack *stk, stack *links){
  int v1, v2;
  stk->index -= stk->item_size * 2;
  v1 = *GETINT(stk);
  v2 = *GETINT(stk);
  stk->index -= stk->item_size * 2;
  PUTINT(stk, &v2);
  PUTINT(stk, &v1);
}

void rPop(stack *stk, stack *links){
  POPINT(stk);
}

void rNPop(stack *stk, stack *links){
  long amt = (long)*POPINT(stk);
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
  printf("stack (%ld): ", stk->index / stk->item_size), print_stack(stk);
}

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

char **split_words(char *source){
  int num_words = 0, end_words;
  size_t len;
  char *p = source, *t;
  char c;
  char **result, **resp;
  while((c = *(p++))){
    if(c == ' '){
      ++num_words;
    }
  }
  ++num_words; // for instance "asdf" generates one word
  end_words = num_words;
  result = MALLOC(char *, num_words + 1);
  resp = result;
  p = source;
  while(num_words--){
    t = p;
    c = *t;
    while(c && c != ' '){
      ++t;
      c = *t;
    }
    len = (size_t)(t - p);
    //printf("len: %ld\n", len);
    t = MALLOC(char, len + 1);
    memcpy(t, p, len);
    t[len] = 0;
    //printf("word: '%s'\n", t);
    *resp = t;
    ++resp;
    p += len + 1;
  }
  result[end_words] = NULL;
  return result;
}

void free_words(char **words){
  char **t = words;
  char *word;
  while((word = *(t++))){
    free(word);
  }
  free(words);
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
  int top = stk->index;
  int mid = *POPINT(links);
  int bottom = *POPINT(links);
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
  size_t top = stk->index;
  size_t bottom = *POPINT(links);
  size_t blocks = (top - bottom)/(stk->item_size);
  stk->index = bottom;
  if(blocks < 2)
    return;
  if(*GETINT(stk) != SYM_STR){
    stk->index = bottom;
    return;
  }
  char *name = *GETSTR(stk);
  hashmap_put(rmap, name, make_int_ptr(defs->index));
  //printf("new fn named: %s\n", name);
  stack_copy(defs, stk, blocks - 2);
  int retsym = SYM_RET;
  PUTINT(defs, &retsym);
  stk->index = bottom;
  //memset(stk->data + stk->index, 0, stk->item_size * blocks);
}

void rAdd(stack *stk, stack *links){
  int a = *POPINT(stk);
  a = *POPINT(stk) + a;
  PUTINT(stk, &a);
}

void rDot(stack *stk, stack *links){
  int a = *POPINT(stk);
  stk->index += stk->item_size;  
  printf("%i\n", a);
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

stack *compile(char ** source){
  char * word, *str;
  int t, v;
  compfn *cVal;
  int *rVal;
  stack *stk = new_stack(STACK_ITEM_SIZE, 100);
  stack *links = new_stack(STACK_ITEM_SIZE, 100);
  stack *defs = new_stack(STACK_ITEM_SIZE, 100);
  map_t *cmap = hashmap_new();
  map_t *rmap = hashmap_new();
  //printf("start compile\n");
  hashmap_put(cmap, ":", make_comp_ptr(&cColon));
  hashmap_put(cmap, ";", make_comp_ptr(&cSemiColon));
  hashmap_put(cmap, "if", make_comp_ptr(&cIf));
  hashmap_put(cmap, "else", make_comp_ptr(&cElse));
  hashmap_put(cmap, "then", make_comp_ptr(&cThen));
  //use int vs neg int
  struct s_runfn *builtin = (struct s_runfn*)builtins;
  t = -1;
  while(builtin->name != NULL){
    hashmap_put(rmap, (builtin->name), make_int_ptr(t));
    --t;
    builtin++;
  }
  
  while((word = *(source++)) != NULL){
    //printf("compiling: %s\n", word);
    cVal = get_comp(cmap, word);
    rVal = get_int(rmap, word);
    if(cVal != NULL){
      (*cVal)(stk, links, defs, cmap, rmap);
    }else if(rVal != NULL){
      v = *rVal;
      if(v < 0){
	v = ~v; // v = -v + 1;
	t = SYM_RCALL;
	PUTINT(stk, &t);
	PUTINT(stk, &v);
      }else{
	t = SYM_RUCALL;
	PUTINT(stk, &t);
	PUTINT(stk, &v);
      }
      //stack.append(SYM_RUCALL);
      //      stack.append(rVal);
    }else{
      int len = strlen(word);
      /*if(len >= 2 && word[0] == '0' && (word[1] < '0' || word[1] > '9')){
	t = SYM_INT;
	PUTINT(stk, &t);
	switch(word[1]){
	case 'b':
	  stack.append(int(word[2:],2));
	  break;
	case 'o':
	  stack.append(int(word[2:],8));
	  break;
	case 'x':
	  stack.append(int(word[2:],16));
	  break;
	}
	}else*/
      if(len >= 1 && word[0] >= '0' && word[0] <= '9'){
	t = SYM_INT;
	PUTINT(stk, &t);
	v = atoi(word);
	PUTINT(stk, &v);
      }else if(len >= 2 && word[0] == '-' && word[1] >= '0' && word[1] <= '9'){
	t = SYM_INT;
	PUTINT(stk, &t);
	v = atoi(word);
	PUTINT(stk, &v);
      }else{
	t = SYM_STR;
	PUTINT(stk, &t);
	PUTSTR(stk, &word);
	/* 
	   words will have to be later searched 
	   for and given their own data section
	   or ignored entirely.
	   Not allocating a new word means if we ignore them
	   we at least won't have memory leaks
	*/
	
	
      //printf("put str %s top:%ld\n", word, stk->index);
      
      }
    }
    /*      }else{
	stack.append(SYM_RUCALL);
	stack.append(rVal);
      }
    }else{
      int len = strlen(word);
      if(len >= 2 && word[0] == '0' && (word[1] < '0' || word[1] > '9')){
	stack.append(SYM_INT);
	switch(word[1]){
	case 'b':
	  stack.append(int(word[2:],2));
	  break;
	case 'o':
	  stack.append(int(word[2:],8));
	  break;
	case 'x':
	  stack.append(int(word[2:],16));
	  break;
	}
      }else if(len >= 1 && word[0] >= '0' && word[0] <= '9'){
	stack.append(SYM_INT);
	stack.append(int(word));
      }else{
	stack.append(SYM_STR);
	stack.append(word);
      }
      }*/
    //print_stack(stk);
  }
  stack *dataspace = new_stack(STACK_ITEM_SIZE, stk->index/stk->item_size + defs->index/defs->item_size + 1);
  v = (int)(defs->index/defs->item_size);
  PUTINT(dataspace, &v);
  size_t copylen = defs->index;
  defs->index = 0;
  stack_copy(dataspace, defs, copylen/defs->item_size);
  copylen = stk->index;
  stk->index = 0;
  stack_copy(dataspace, stk, copylen/stk->item_size);
  free_stack(defs);
  free_stack(stk);
  free_stack(links);
  dataspace = secondpass(dataspace, rmap);
  hashmap_free(cmap);
  hashmap_free(rmap);
  return dataspace;
}

stack *secondpass(stack *dataspace, map_t *rmap){
  int DATASTART = 1 * dataspace->item_size;
  dataspace->index = DATASTART;
  int ldata = dataspace->length;
  unsigned char sym;
  char *word;
  int *rVal, v, t;
  while(dataspace->index < ldata){
    sym = *GETINT(dataspace);
    //printf("at %ld: %i\n", dataspace->index / dataspace->item_size, sym);
    switch(sym){
    case SYM_HALT:
      break;
      
    case SYM_RUCALL:
      dataspace->index += dataspace->item_size;
      break;
      
    case SYM_RCALL:
      dataspace->index += dataspace->item_size;
      break;
      
    case SYM_CUCALL:
      dataspace->index += dataspace->item_size;
      break;
    case SYM_CCALL:
      dataspace->index += dataspace->item_size;
      break;
      
    case SYM_INT:
      dataspace->index += dataspace->item_size;
      break;
      
    case SYM_STR:
      word = *GETSTR(dataspace);
      rVal = get_int(rmap, word);
      if(rVal == NULL){
	break;
      }
      v = *rVal;
      dataspace->index -= dataspace->item_size * 2;
      if(v < 0){
	v = ~v; // v = -v + 1;
	t = SYM_RCALL;
	PUTINT(dataspace, &t);
	PUTINT(dataspace, &v);
      }else{
	t = SYM_RUCALL;
	PUTINT(dataspace, &t);
	PUTINT(dataspace, &v);
      }
      break;
      
    case SYM_RET:
      break;
      
    case SYM_RBRANCH:
      dataspace->index += dataspace->item_size;
      break;
    case SYM_RJUMP:
      dataspace->index += dataspace->item_size;
      break;
      
    default:
      printf("err\n");
      break;
    }
    
  }
  return dataspace;
}

void interp(stack *dataspace){
  //print_stack(dataspace);

  stack *stk = new_stack(STACK_ITEM_SIZE, 30);
  stack *links = new_stack(STACK_ITEM_SIZE, 30);
  dataspace->index = 0;
  dataspace->index = *GETINT(dataspace) * dataspace->item_size;
  int DATASTART = 1 * dataspace->item_size;
  int ldata = dataspace->length;
  unsigned char sym;
  int running = 1;
  int ind;
  int str;
  int d;
  int cond;
  size_t jmp;
  dataspace->index += DATASTART;
  //printf("i: %i\n", dataspace->index);
  while(dataspace->index < ldata && running){
    sym = *GETINT(dataspace);
    //printf("sym: %i\n", sym);
    switch(sym){
    case SYM_HALT:
      running = 0;
      break;
      
    case SYM_RUCALL:
      ind = *GETINT(dataspace);
      d = dataspace->index;
      PUTINT(links, &d);
      dataspace->index = ind + DATASTART;
      break;
      
    case SYM_RCALL:
      ind = *GETINT(dataspace);
      rundefs[ind](stk, links);
      break;
      
    case SYM_CUCALL:
      break;
    case SYM_CCALL:
      break;
      
    case SYM_INT:
      d = *GETINT(dataspace);
      PUTINT(stk, &d);
      break;
      
    case SYM_STR:
      str = *GETINT(dataspace);
      //stack_putStr(stk, str);
      break;
      
    case SYM_RET:
      dataspace->index = *POPINT(links);
      break;
      
    case SYM_RBRANCH:
      jmp = *GETINT(dataspace) * stk->item_size;
      cond = *POPINT(stk);
      if (cond == 0){
	dataspace->index += jmp;
      }
      break;
    case SYM_RJUMP:
      jmp = *GETINT(dataspace) * stk->item_size;
      dataspace->index += jmp;
      break;
      
    default:
      printf("err\n");
      break;
    }
    //printf("stack:"), print_stack(stk);
    //printf("links:"), print_stack(links);
  }
  free_stack(stk);
  free_stack(links);
  
}


int max_int(int n, ...){
  va_list args;
  int max, t;
  if(n < 1)
    return 0;
  va_start(args, n);
  max = (int)(va_arg(args, int));
  while(--n){
    t = (int)(va_arg(args, int));
    if(t > max)
      max = t;
  }
  return max;
}


int main(int argc, char **argv){
  STACK_ITEM_SIZE = max_int(2, sizeof(int), sizeof(char *));
  struct s_runfn *builtin = (struct s_runfn*)builtins;
  int t = 0;
  while(builtin->name != NULL){
    t++;
    builtin++;
  }
  builtin = (struct s_runfn*)builtins;
  rundefs = MALLOC(runfn, t);
  t = 0;
  while(builtin->name != NULL){
    rundefs[t] = (builtin->fn);
    t++;
    builtin++;
  }
  char **words = split_words(": range dup 0 > if dup 1 - range else then ; 5 range 6 1 stack  nrot stack");
  //char **words = split_words(": FIB dup 1 <= if else dup 1 - FIB rot 2 - FIB + then ; 6 FIB .");
  //
  //": TESTIF 0 + if 30 else 40 then ; 1 TESTIF 0 TESTIF + .");
  //": THREE 1 2 + ; THREE THREE + .");
  //": THREE 1 2 + if NOTZERO else ZERO then ; THREE .");
  stack *compiled = compile(words);//words//#sys.stdin
  //print_stack(compiled);
  interp(compiled);
  free_stack(compiled);
  printf("ok\n");
  free_words(words);
  return 0;
}
 
