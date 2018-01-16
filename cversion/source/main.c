#include "main.h"

runfn *rundefs;

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
  struct s_runfn *builtin = (struct s_runfn*)get_builtins();
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
  int *tpop;
  char **stpop;
  int DATASTART = 1 * dataspace->item_size;
  dataspace->index = DATASTART;
  int ldata = dataspace->length;
  unsigned char sym;
  char *word;
  int *rVal, v, t;
  while(dataspace->index < ldata){
    tpop = GETINT(dataspace);
    if(tpop == NULL)
      break;
    sym = *tpop;
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
      stpop = GETSTR(dataspace);
      if(stpop == NULL)
	break;
      word = *stpop;
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
  int *tpop;
  stack *stk = new_stack(STACK_ITEM_SIZE, 30);
  stack *links = new_stack(STACK_ITEM_SIZE, 30);
  dataspace->index = 0;
  tpop = GETINT(dataspace);
  if(tpop == NULL)
    return;
  dataspace->index = *tpop * dataspace->item_size;
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
    tpop = GETINT(dataspace);
    if(tpop == NULL)
      break;
    sym = *tpop;
    //printf("sym: %i\n", sym);
    switch(sym){
    case SYM_HALT:
      running = 0;
      break;
      
    case SYM_RUCALL:
      tpop = GETINT(dataspace);
      if(tpop == NULL)
	break;
      ind = *tpop;
      d = dataspace->index;
      PUTINT(links, &d);
      dataspace->index = ind + DATASTART;
      break;
      
    case SYM_RCALL:
      tpop = GETINT(dataspace);
      if(tpop == NULL)
	break;
      ind = *tpop;
      rundefs[ind](stk, links);
      break;
      
    case SYM_CUCALL:
      break;
    case SYM_CCALL:
      break;
      
    case SYM_INT:
      tpop = GETINT(dataspace);
      if(tpop == NULL)
	break;
      d = *tpop;
      PUTINT(stk, &d);
      break;
      
    case SYM_STR:
      tpop = GETINT(dataspace);
      if(tpop == NULL)
	break;
      str = *tpop;
      //stack_putStr(stk, str);
      break;
      
    case SYM_RET:
      tpop = POPINT(links);
      if(tpop == NULL)
	break;
      dataspace->index = *tpop;
      break;
      
    case SYM_RBRANCH:
      tpop = GETINT(dataspace);
      if(tpop == NULL)
	break;
      jmp = *tpop * stk->item_size;
      tpop = POPINT(stk);
      if(tpop == NULL)
	break;
      cond = *tpop;
      if (cond == 0){
	dataspace->index += jmp;
      }
      break;
    case SYM_RJUMP:
      tpop = GETINT(dataspace);
      if(tpop == NULL)
	break;
      jmp = *tpop * stk->item_size;
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
  struct s_runfn *builtin = (struct s_runfn*)get_builtins();
  int t = 0;
  while(builtin->name != NULL){
    t++;
    builtin++;
  }
  builtin = (struct s_runfn*)get_builtins();
  rundefs = MALLOC(runfn, t);
  t = 0;
  while(builtin->name != NULL){
    rundefs[t] = (builtin->fn);
    t++;
    builtin++;
  }
  //char **words = split_words("1 pop pop stack");
  //char **words = split_words(": range dup 0 > if dup 1 - range else then ; 5 range 6 1 stack  nrot stack");
  char **words;
  stack *compiled;
  char *first = ": FIB dup 1 <= if else dup 1 - FIB rot 2 - FIB + then ; 6 FIB .";
  char *input;
  input = strdup(first);
  int len = strlen(input);
  while(1){
    if(input && len > 0){
      printf("CODE: %s\n", input);
      words = split_words(input);
      //
      //": TESTIF 0 + if 30 else 40 then ; 1 TESTIF 0 TESTIF + .");
      //": THREE 1 2 + ; THREE THREE + .");
      //": THREE 1 2 + if NOTZERO else ZERO then ; THREE .");
      compiled = compile(words);//words//#sys.stdin
      //print_stack(compiled);
      interp(compiled);
      free_stack(compiled);
      printf("ok\n");
      free_words(words);
      free(input);
      input = NULL;
      size_t size;
      printf("> ");
      getline(&input, &size, stdin);
    }
    if(input){
      len = strlen(input);
      if(input[len - 1] == '\n'){
	input[len - 1] = 0;
	--len;
      }
    }
  }
  printf("done\n");
  return 0;
}
