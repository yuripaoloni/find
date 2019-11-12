#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct fileList{
    char *path;
    struct fileList *next;
};

struct filePath{
    char *path;
    int fileOccurrences;
    struct OccurrencesPosition *position;
    struct filePath *next;
};

struct fileWord{
	char word[50];
	int totalOccurences;
    struct filePath *p;
    struct fileWord *next;
};

struct OccurrencesPosition{
    int line;
    int character;
    struct OccurrencesPosition *next;    
};

typedef struct filePath fPath; 
typedef struct fileWord fWord;
typedef struct OccurrencesPosition fPosition;
typedef struct fileList fList;

fPosition * getHead();

int KMP(const char* X, const char* Y, int m, int n, int line, fPath *app);

