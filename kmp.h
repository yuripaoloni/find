#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int KMP(const char* X, const char* Y, int m, int n, int line, int index);

void printPosition(int index);

int getTotalOccurences(int index);