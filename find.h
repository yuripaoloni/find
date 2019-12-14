#include<time.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include "kmp.h"

void argumentsCheck(const char *args, const char *message);

void checkingForRecursion(llist *lCurr, const char *excluded);

void execute(const char *wordFile, const char *inputFile, const char *excluded, const int verboseFlag, const int outputFlag, const char *outputFile);

void writeFile(fWord *w, fWord *wordHead, fPath *pathHead, const char * outputFile);

void print(fWord *w, fWord *wordHead, fPath *pathHead);

void getFileList(const char *reportFile, const char *wordToCheck, const int occurr);

void getWordOccurences(const char *word, const char *file, const char *fileToCheck);

void freeMemory();

char *get_filename_ext(const char *filename);

int is_regular_file(const char *path);

void listFilesRecursively(char *path, const char *recursive, const int a, const char *exclude);

void executeKMP(fWord *w, FILE *fp);