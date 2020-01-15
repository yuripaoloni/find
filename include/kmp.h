
#ifndef KMP_H
#define KMP_H


#include<stdio.h>
#include<stdlib.h>
#include "../include/struct.h"

fPosition * getHead();

int KMP(const char* X, const char* Y, int m, int n, int line, fPath *app);

void freeKMP();

#endif // !KMP_H

