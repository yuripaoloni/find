#include "find.h"

struct filePath{
	char path[255];
	int fileOccurences;
};

int main(int argc,char* argv[]){

	struct filePath fPath[2];
	char currentLine[255]; //conterrà la riga corrente
	char path[255];
	char word[30];
    int i, ch;
	int k = 0;

	FILE * fInput = fopen(argv[1], "r"); //apre il file passato come parametro in lettura
	if(fInput == NULL){ //check sull'apertura del file
    	fprintf(stderr, "Cannot open %s, exiting. . .\n", argv[1]);
    	exit(1);
    }

	while(!feof(fInput)){
		for (i = 0; (i < (sizeof(path)-1) && ((ch = fgetc(fInput)) != EOF) && (ch != '\n')); i++){
			fPath[k].path[i] = ch;
		}
		fPath[k].path[i] = '\0';
		k = k + 1;
	}

	fclose(fInput);

	printf("Enter the word to search: ");
	scanf("%s", word);

	printf("WORD <%s> \r\n", word);
	printf("TOTAL <%d> \r\n", 0);

	for(int j = 0; j<2; j++){
		FILE * fp = fopen(fPath[j].path, "r"); //apre il file passato come parametro in lettura
		if(fp == NULL){ //check sull'apertura del file
			fprintf(stderr, "Cannot open %s, exiting. . .\n", fPath[j].path);
			exit(1);
		}

		int count = 0;

		while(!feof(fInput)){
			for (i = 0; (i < (sizeof(currentLine)-1) && ((ch = fgetc(fp)) != EOF) && (ch != '\n')); i++){
				currentLine[i] = ch;
			}
			currentLine[i] = '\0';
			int n = strlen(currentLine);
			int m = strlen(word);
			KMP(currentLine, word, n, m, count);
			count = count + 1;
		}

		printf("FILE <%s> \r\n", /*path del file*/ fPath[j].path);
		printf("OCCURENCES <%d>	\r\n", 0);
		printf("<linea> <posizione>\r\n");
		fclose(fp);
	}

	printf("\r\n"); //il file termina con una riga vuota

	return 0;
}