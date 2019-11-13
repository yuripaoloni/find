#include<time.h>
#include "kmp.h"

char * getLineOfAnySize(FILE* fp, size_t typicalSize, int *endOfLineDetected,size_t *nrOfCharRead);

void executeVerbose(char *inputFile, char *excluded);

void writeFile(fWord *w, fWord *wordHead, char * outputFile);

void execute(char *inputFile, char *excluded);

void print(fWord *w, fWord *wordHead);

void getFileList(char *reportFile, char *wordToCheck, int occurr);

void getWordOccurences(char *word, char *file, char *fileToCheck);

void freeMemory();

char *get_filename_ext(char *filename);









