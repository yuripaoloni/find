#include "find.h"

int main(int argc,char* argv[]){

	char currentLine[255]; //conterrà la riga corrente
	char line[255];
	char word[30];
	int count = 0;

	FILE * fInput = fopen(argv[2], "r"); //apre il file passato come parametro in lettura
	if(fInput == NULL){ //check sull'apertura del file
        fprintf(stderr, "Cannot open %s, exiting. . .\n", argv[2]);
        exit(1);
    }

	printf("Enter the word to search: ");
	scanf("%s", word);

	while(fgets(currentLine, sizeof(currentLine), fInput) != NULL){ //legge tutte le righe del file
		int n = strlen(currentLine);
		int m = strlen(word);
		KMP(currentLine, word, n, m, count);
		count = count + 1;
	}

	fclose(fInput);

	return 0;
}