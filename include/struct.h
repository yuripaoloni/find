#ifndef STRUCT_H
#define STRUCT_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<string.h>

//stores all the path to analyze during the execution of KMP algorithm, 
//this paths can be obtained from the recursive read of a folder given in input by the user.
struct fileList{
    char *path;
    char *directory;
    struct fileList *next;
};

//stores all the files where the program searched and the word occurrences founded in them.
struct filePath{
    char *path;
    char *directory;
    int fileOccurrences;
    struct OccurrencesPosition *position;
    struct filePath *next;
};

//struct with all the line of the input file
struct lineList{ 
    char *line;
    struct lineList *next;
};

//stores all the words to search and all the data retrieved from the analysis of the list of file.
struct fileWord{
	char *word;
	int totalOccurences;
    struct filePath *p;
    struct fileWord *next;
};

//stores the position of each occurrences founded in a file.
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

fList* createFList(fList* listHead, fList* listTail, char* basePath, char* dir);

fList* sortfList(fList *listHead, fList *listTail, fList *n);

llist* createllist(llist *lHead, llist *lTail, const char *inputFile);

llist * sortllist(llist *lHead, llist *lTail, llist *l);

fWord * createfWord(fWord *wordHead, fWord *wordTail, const char *wordFile);

fWord * sortfWord(fWord *wordHead, fWord *wordTail, fWord *app);

fPath * createfPath(fPath **app, fPath **pathHead, fPath **pathTail, char *path, char *directory);

void trimTrailing(char * str);

int checkDuplicatePath(llist *lHead, const char *line);

int checkDuplicateWord(fWord *wordHead, const char *line);

int checkDuplicateFile(fList *listHead, const char *line);

void sortFileByOccurrences(fPath *head);

void swapfPath(fPath *a, fPath *b);

#endif // !STRUCT_H