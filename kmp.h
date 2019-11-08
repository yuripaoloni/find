#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct filePath{
    char *path;
    int fileOccurrences;
    int position[255][255];
    struct filePath *next;
};

struct fileWord{
	char word[50];
	int totalOccurences;
	struct filePath *p;
};	

typedef struct filePath fPath; 
typedef struct fileWord fWord;

int KMP(const char* X, const char* Y, int m, int n, int line, fPath *app);

void printPosition(fPath *app);

void allocate2dArray(fPath *app, int row, int column);
