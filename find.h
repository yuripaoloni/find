#include<time.h>
#include "kmp.h"

char * getLineOfAnySize(FILE* fp, size_t typicalSize, int *endOfLineDetected,size_t *nrOfCharRead);

void execute(char *inputFile, char *excluded, int num);

void writeFile(fWord *w, fWord *wordHead, char * outputFile);

void print(fWord *w, fWord *wordHead);

void getFileList(char *reportFile, char *wordToCheck, int occurr);

void getWordOccurences(char *word, char *file, char *fileToCheck);

void freeMemory();

char *get_filename_ext(char *filename);









