#ifndef UTIL_H
#define UTIL_H
#define MALLOC(TYPE, AMOUNT) (TYPE*)malloc(sizeof(TYPE) * (AMOUNT))
#define NEW(TYPE) (TYPE*)malloc(sizeof(TYPE))

#endif
