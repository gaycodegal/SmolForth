#include "main.h"

struct fnptr {

};

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

/**
   takes in a iterable which returns the separated words
   and compiles it into bytecode
*/
stack *compile(char ** source){
  char * word;
  fnptr * cVal;
  stack *stk = new_stack();
  stack *links = new_stack();
  while((word = *(source++)) != NULL){
    cVal = cWords->get(word, NULL);
    rVal = rWords->get(word, NULL);
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
    }
  }
  /*
    #print("stack:", stack)
    #print("defnames:", rWords)
    #print("defs:", defs)
  */
  stack *dataspace = new_stack(stack->len + defs->len + 1);
  *(int *)(dataspace->data) = defs->len;
  memcpy(dataspace->data + sizeof(int), defs->data, defs->len);
  memcpy(dataspace->data + sizeof(int) + defs->len, stack->data, stack->len);
  free_stack(stack);
  free_stack(links);
  free_stack(defs);
  return dataspace;
}

/**
   Takes a bytecode compiled forth program definition <dataspace>
   forth program definition structure described in the README
*/
void interp(stack *dataspace){
  stack *stk = new_stack();
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
}





int main(int argc, char **argv){
  printf("hi\n");
  char **words = splitwords(": THREE getInt if NOTZERO else ZERO then ; THREE .");
  stack *compiled = compile(words);//#sys.stdin
  interp(compiled);
  free_stack(compiled);
  printf("bye\n");
  
  return 0;
}
