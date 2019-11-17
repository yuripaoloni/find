#include<time.h>
#include<dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "kmp.h"


char * getLineOfAnySize(FILE* fp, size_t typicalSize, int *endOfLineDetected,size_t *nrOfCharRead);

void execute(char *wordFile, char *inputFile, char *excluded, int num);

void writeFile(fWord *w, fWord *wordHead, char * outputFile);

void print(fWord *w, fWord *wordHead);

void getFileList(char *reportFile, char *wordToCheck, int occurr);

void getWordOccurences(char *word, char *file, char *fileToCheck);

void freeMemory();

char *get_filename_ext(char *filename);

int is_regular_file(const char *path);

void listFilesRecursively(char *path, char *recursive, int a, char *exclude);









