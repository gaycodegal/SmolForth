#include "main.h"

int main(int argc, char **argv){
  //char **words = split_words("1 pop pop stack");
  //char **words = split_words(": range dup 0 > if dup 1 - range else then ; 5 range 6 1 stack  nrot stack");
  //
  //": TESTIF 0 + if 30 else 40 then ; 1 TESTIF 0 TESTIF + .");
  //": THREE 1 2 + ; THREE THREE + .");
  //": THREE 1 2 + if NOTZERO else ZERO then ; THREE .");
  char * source = ": FIB dup 1 <= if else dup 1 - FIB rot 2 - FIB + then ; 6 FIB .";
  printf("> %s\n", source);
  interpret(source);
  repl();
  return 0;
}
