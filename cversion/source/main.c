#include "main.h"
#include "hashmap.h"

typedef struct s_fnptr {
  
} fnptr;

/*
  mains_map = hashmap_new();
  hashmap_put(mains_map, "battle", make_mptr(&battle_main));
  hashmap_put(mains_map, "graphics", make_mptr(&graphics_main));
  hashmap_put(mains_map, "filereader", make_mptr(&filereader_main));
  hashmap_put(mains_map, "attacks", make_mptr(&attacks_main));
  hashmap_put(mains_map, "prompt", make_mptr(&prompt_main));
  if(argc == 2)
    status = hashmap_get(mains_map, argv[1], p);
  if(status != MAP_OK){
    printf("usage: %s <mode>\n", *argv);
    printf("modes: battle, graphics, filereader, attacks, prompt\n");
    return 1;
  }else{
    f = *(mainfn *)*p;
    status = f();
  }
  hashmap_iterate(mains_map, &free_mptr, NULL);
  hashmap_free(mains_map);

*/

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

char * free_words(char **words){
  char **t = words;
  char *word;
  while((word = *(t++))){
    printf("free: '%s'\n", word);
    free(word);
  }
  free(words);
}

void cColon(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap){
  int top = (stk->index);
  PUTINT(links, &top);
  printf("HIHI\n");
}

void cSemiColon(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap){
  size_t top = stk->index;
  size_t bottom = *POPINT(links);
  size_t blocks = (top - bottom)/(stk->item_size);
  stk->index = bottom;
  printf("blocks: %ld\n", blocks);
  stack_copy(defs, stk, blocks);
  int retsym = SYMRET;
  PUTINT(defs, &retsym);
  stk->index = bottom;
  printf("Byebye %ld %ld\n",top, bottom);
}


typedef void (*compfn)(stack *stk, stack *links, stack *defs, map_t *cmap, map_t *rmap);

compfn *make_comp_ptr(compfn fn){
  compfn *p = NEW(compfn);
  *p = fn;
  return p;
}

compfn *get_comp(map_t *map, char *key){
  any_t p[1];
  int status = hashmap_get(map, key, p);
  if(status != MAP_OK){
    return NULL;
  }
  return (compfn *)*p;
}

/**
   takes in a iterable which returns the separated words
   and compiles it into bytecode
*/
stack *compile(char ** source){
  char * word;
  compfn *cVal;
  stack *stk = new_stack(STACK_ITEM_SIZE, 1 << 10);
  stack *links = new_stack(STACK_ITEM_SIZE, 1 << 10);
  stack *defs = new_stack(STACK_ITEM_SIZE, 1 << 10);
  map_t *cmap = hashmap_new();
  map_t *rmap = hashmap_new();
  printf("start compile\n");
  hashmap_put(cmap, ":", make_comp_ptr(&cColon));
  hashmap_put(cmap, ";", make_comp_ptr(&cSemiColon));

  while((word = *(source++)) != NULL){
    printf("compiling: %s\n", word);
    cVal = get_comp(cmap, word);
    if(cVal != NULL){
      (*cVal)(stk, links, defs, cmap, cmap);
    }
    //cVal = cWords->get(word, NULL);
    //rVal = rWords->get(word, NULL);
    /*
    if(cVal != NULL){
      if(cVal == int){
	interpC(stack, links, defs, cWords, rWords);
      }else{
	cVal(stack, links, defs, cWords, rWords);
      }
    }else if(rVal != NULL){
	
      if(rVal == int){
	stack.append(SYMRUCALL);
	stack.append(rVal);        
      }else{
	stack.append(SYMRCALL);
	stack.append(rVal[0]);
      }
    }else{
      int len = strlen(word);
      if(len >= 2 && word[0] == '0' && (word[1] < '0' || word[1] > '9')){
	stack.append(SYMINT);
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
	stack.append(SYMINT);
	stack.append(int(word));
      }else{
	stack.append(SYMSTR);
	stack.append(word);
      }
      }*/
  }
  /*stack *dataspace = new_stack(stack->len + defs->len + 1);
  *(int *)(dataspace->data) = defs->len;
  memcpy(dataspace->data + sizeof(int), defs->data, defs->len);
  memcpy(dataspace->data + sizeof(int) + defs->len, stack->data, stack->len);*/
  free_stack(defs);
  free_stack(stk);
  free_stack(links);
  hashmap_free(cmap);
  hashmap_free(rmap);
    
  //return dataspace;
  return NULL;
}

/**
   Takes a bytecode compiled forth program definition <dataspace>
   forth program definition structure described in the README
*/
void interp(stack *dataspace){
  /*stack *stk = new_stack();
  stack *links = new_stack();
  int i = *(int *)(dataspace->data);
  int DATASTART = 1;
  int ldata = dataspace->len;
  unsigned char sym;
  int runnning = true;
  int ind;
  char *str;
  i += DATASTART;

  while(i < ldata && running){
    sym = dataspace[i++];
    switch(sym){
    case SYMHALT:
      running = false;
      break;
      
    case SYMRUCALL:
      ind = stack_getInt(dataspace);
      stack_putInt(links, i);
      i = ind + DATASTART;
      break;
      
    case SYMRCALL:
      ind = stack_getInt(dataspace);
      rWordDefs[fun](stack, links);
      break;
      
    case SYMCUCALL:
      break;
    case SYMCCALL:
      break;
      
    case SYMINT:
      d = stack_getInt(dataspace);
      stack_putInt(stk, d);
      break;
      
    case SYMSTR:
      str = stack_getStrCpy(dataspace);
      stack_putStr(stk, str);
      break;
      
    case SYMRET:
      i = stack_popInt(links);
      break;
      
    case SYMRBRANCH:
      jmp = stack_getInt(dataspace);
      cond = stack_popInt(stk);
      if (cond == 0){
	i += jmp;
      }
      break;
    case SYMRJUMP:
      jmp = stack_getInt(dataspace);
      i += jmp;
      break;
      
    default:
      printf("err\n");
      break;
    }
  }
  free_stack(stack);
  free_stack(links);
  */
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
  STACK_ITEM_SIZE = max_int(1, sizeof(int));
  printf("hi\n");
  char **words = split_words(": THREE getInt if NOTZERO else ZERO then ; THREE .");
  stack *compiled = compile(words);//words//#sys.stdin
  interp(compiled);
  stack_main();
  //free_stack(compiled);
  printf("bye\n");
  free_words(words);
  return 0;
}
