#include<time.h>
#include "kmp.h"

char * getLineOfAnySize(FILE* fp, size_t typicalSize, int *endOfLineDetected,size_t *nrOfCharRead);

void executeVerbose(char *inputFile);

void writeFile(fWord *w, fWord *wordHead, char * outputFile);

void execute(char *inputFile, char *excluded);

void print(fWord *w, fWord *wordHead);

void getFileOccurrences(char *reportFile);

void freeMemory();

char *get_filename_ext(char *filename);









