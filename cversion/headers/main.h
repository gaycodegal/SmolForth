#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include "stack.h"

enum ForthByteCode{SYMHALT = 0, SYMRUCALL, SYMRCALL, SYMCUCALL, SYMCCALL, SYMRET, SYMINT, SYMSTR, SYMRBRANCH, SYMRJUMP};

#endif
