#ifndef FIND_H
#define FIND_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include "../include/test.h"
#include "../include/kmp.h"

void printHelp();

void argumentsCheck(const char *args, const char *message);

void checkingForRecursion(llist *lCurr, const char *excluded);

void execute(const char *wordFile, const char *inputFile, const char *excluded, const int verboseFlag, const int outputFlag, const char *outputFile);

void writeFile(fWord *w, fWord *wordHead, fPath *pathHead, const char * outputFile);

void print(fWord *w, fWord *wordHead, fPath *pathHead);

void getFileList(const char *reportFile, const char *wordToCheck, const int occurr);

void getWordOccurences(const char *word, const char *file, const char *fileToCheck);

void freeMemory();

char *getFilenameExt(const char *filename);

int isRegularFile(const char *path);

void listFilesRecursively(char *path, const char *recursive, const int a, const char *exclude);

void executeKMP(fWord *w, FILE *fp);

int analyzeOutputFile(char **currentWord, char **currentFile, char **p, char **line);

char *getDirectory(const char *basePath);

void startTime(char **appDir, fList *list);

void elapsedTime(char **appDir, fList *list);

#endif // !FIND_H