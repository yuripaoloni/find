#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct fileList{
    char *path;
    char *directory;
    struct fileList *next;
};

struct filePath{
    char *path;
    char *directory;
    int fileOccurrences;
    struct OccurrencesPosition *position;
    struct filePath *next;
};

struct lineList{ //structure with all the line of the input file
    char *line;
    struct lineList *next;
};

struct fileWord{
	char *word;
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

typedef struct lineList llist;

fPosition * getHead();

int KMP(const char* X, const char* Y, int m, int n, int line, fPath *app);

void freeKMP();

