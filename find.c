#include "find.h"

struct filePath{
	char path[255];
	int fileOccurences;
	int position[255][255];
};

struct fileWord{
	char word[30];
	int totalOccurences;
	struct filePath fPath[2];
};

int main(int argc,char* argv[]){

	struct filePath fPath[2];
	struct fileWord fWord[1];
	char currentLine[255]; //conterrà la riga corrente
    int i, ch;
	int k = 0;

	if(argc == 1){ //stampa le informazioni sul programma
		printf("Il programma find consente di individuare il numero di occorrenze di un insieme di stringhe all'interno di un gruppo di file.\r\n");
		printf("Per eseguire il programma utilizzare i seguenti parametri:\r\n");
		printf(" find --word|-w <inputfile> : per stampare il report alla fine dell'esecuzione\r\n");
		printf("Parametri opzionali:\r\n");
		printf(" --output|-o <outputfile> : per salvare il report su un file particolare\r\n");
		printf(" --exclude|-e <ext> : durante la fase di analisi sara' possibile ignorare i file con specifiche estensioni\r\n");
		printf(" --verbose|-v : per visionare il processo di analisi\r\n");
		printf("Una volta generato il file di report, il programma find potra' essere usato per recuperare\r\n");
		printf("le informazioni salvate con i seguenti comandi:\r\n");
		printf(" find --report|-r <reportfile> --show <word> <n> : stampare la lista del file dove occorre almeno <n> volte la parola <word>\r\n");
		printf(" find --report|-r <reportfile> --show <word> --file <file> : stampare tutte le posizioni dove la parola <word> occorre nel <file>\r\n");
		return 0;
	}
	
	FILE * fInput = fopen(argv[2], "r"); //apre il file passato come parametro in lettura
	if(fInput == NULL){ //check sull'apertura del file
    	fprintf(stderr, "Cannot open %s, exiting. . .\n", argv[2]);
    	exit(1);
    }

	while(!feof(fInput)){
		for (i = 0; (i < (sizeof(fPath[k].path)-1) && ((ch = fgetc(fInput)) != EOF) && (ch != '\n')); i++){
			fPath[k].path[i] = ch;
		}
		fPath[k].path[i] = '\0';
		k = k + 1;
	}

	fclose(fInput);

	//per farne inserire di più possiamo chiedere all'utente di dividerla con , o - 
	printf("Enter the words to search: "); 
	scanf("%s", fWord[0].word);

	fWord[0].totalOccurences = 0;

	for(int j = 0; j<2; j++){
		FILE * fp = fopen(fPath[j].path, "r"); //apre il file passato come parametro in lettura
		if(fp == NULL){ //check sull'apertura del file
			fprintf(stderr, "Cannot open %s, exiting. . .\n", fPath[j].path);
			exit(1);
		}

		int countLine = 0;
		fPath[j].fileOccurences = 0;

		while(!feof(fInput)){
			for (i = 0; (i < (sizeof(currentLine)-1) && ((ch = fgetc(fp)) != EOF) && (ch != '\n')); i++){
				currentLine[i] = ch;
			}
			currentLine[i] = '\0';
			int n = strlen(currentLine);
			int m = strlen(fWord[0].word);
			fPath[j].fileOccurences = fPath[j].fileOccurences + KMP(currentLine, fWord[0].word, n, m, countLine, j);
			countLine = countLine + 1;
		}
		fWord[0].totalOccurences = fWord[0].totalOccurences + getTotalOccurences(j);
		fclose(fp);
	}

	printf("WORD %s \r\n", fWord[0].word);
	printf("TOTAL %d \r\n", fWord[0].totalOccurences);
	for(int h = 0; h<2; h++){
		printf("FILE %s \r\n", /*path del file*/ fPath[h].path);
		printf("OCCURENCES %d	\r\n", fPath[h].fileOccurences);
		printPosition(h);
	}
	printf("\r\n"); //il file termina con una riga vuota

	return 0;
}