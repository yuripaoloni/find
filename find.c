#include "find.h"

int main(int argc, char * argv[]){

    FILE *fInput = NULL;   // file handle
    FILE *fp = NULL;
    char *line1; // 
    char *line2;
    int endOfLineDetected = 0;
    size_t nrOfCharRead = 0;
    char ch;

    fWord *w = NULL;
    fWord *start = NULL;
    fWord *tail = NULL;

    fPath *head = NULL;
    fPath *current = NULL;

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

    fInput = fopen(argv[2], "r"); //the file that contains the path of the file in which search.

    if(fInput == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", argv[2]);
    	exit(1);
    }
    
    while(!endOfLineDetected){ //read line by line the input file in order to save the path in a structure
        line1 = getLineOfAnySize(fInput,128,&endOfLineDetected,&nrOfCharRead);
        fPath *node = malloc (sizeof(fPath));
        node->path = line1;
        node->fileOccurrences = 0;
        node->position = NULL;
        node->next = NULL;

        if(head == NULL){
            current = head = node;
        }else{
            current = current->next = node;
        }
    }

    fclose(fInput);

    do{
        fWord *app = malloc(sizeof(fWord));
        printf("Insert the word to search: ");
        scanf("%s", app->word);
        app->totalOccurences = 0;
        app->p = head;
        app->next = NULL;

        if(start == NULL){
            tail = start = app;
        }else{
            tail = tail->next = app;
        }
        printf("Do you want to insert another word? (Y/N): ");
        scanf(" %c", &ch);
    }while(ch == 'y' || ch == 'Y');

    w = start;

    while(w != NULL){
        while(w->p != NULL){
            fp = fopen(w->p->path, "r"); //apre il file passato come parametro in lettura
            if(fp == NULL){ //check sull'apertura del file
                fprintf(stderr, "Cannot open %s, exiting. . .\n", w->p->path);
                exit(1);
            }

            int countLine = 0;
            w->p->fileOccurrences = 0;
            endOfLineDetected = 0;

            while(!endOfLineDetected){
                line2 = getLineOfAnySize(fp,128,&endOfLineDetected,&nrOfCharRead);
                int n = strlen(line2);
                int m = strlen(w->word);
                w->p->fileOccurrences = w->p->fileOccurrences + KMP(line2, w->word, n, m, countLine, w->p);
                countLine = countLine + 1;
            }   

            w->totalOccurences = w->totalOccurences + w->p->fileOccurrences; 
            w->p->position = getHead();
            w->p = w->p->next;
            fclose(fp);
        }
        w->p = head;
        printf("WORD %s \r\n", w->word);
        printf("TOTAL %d \r\n", w->totalOccurences);
        while(w->p != NULL){
            printf("FILE %s \r\n", w->p->path);
            printf("OCCURENCES %d	\r\n", w->p->fileOccurrences);
            while (w->p->position != NULL){
                printf("%d %d\r\n", w->p->position->line, w->p->position->character);
                w->p->position = w->p->position->next;
            }
            w->p = w->p->next;
        }
        w = w->next;
    }

    // w = start;

    // while(w != NULL){
    //     printf("WORD %s \r\n", w->word);
    //     printf("TOTAL %d \r\n", w->totalOccurences);
    //     while(w->p != NULL){
    //         printf("FILE %s \r\n", w->p->path);
    //         printf("OCCURENCES %d	\r\n", w->p->fileOccurrences);
    //         while (w->p->position != NULL){
    //             printf("%d %d\r\n", w->p->position->line, w->p->position->character);
    //             w->p->position = w->p->position->next;
    //         }
    //         w->p = w->p->next;
    //     }
    //     w = w->next;
    // }

	printf("\r\n"); //il file termina con una riga vuota

    return 0;
}
    

char * getLineOfAnySize(FILE* fp, size_t typicalSize, int *endOfLineDetected,size_t *nrOfCharRead){ 
    char *line;       // buffer for our string
    int ch;           // we will read line character by character
    size_t len = 0;   // number of characters read (character counter)
    size_t lineSize = typicalSize;  // initial size of the buffer allocated for the line
    *nrOfCharRead = 0;

    if(!fp) return NULL; // protection

    // allocating the buffer
    line = realloc(NULL, sizeof(char)*lineSize); // expected size of the line is up to typicalSize

    if (!line) return line; // protection, if we fail to allocate the memory we will return NULL

    while (1) { // loop forever     
        ch = fgetc(fp);       // getting character by character from file

        if (ch == '\n') break; // end of line detected - breaking the loop 
        if( ch == EOF)  {
            *endOfLineDetected = 1;
            break; // end of file detected - breaking the loop
         }

        line[len++] = ch;     // store the character in the line buffer, increase character counter

        if (len == lineSize){ // we reached the end of line buffer (no more room)

            lineSize = lineSize + 64; // we have to increase the line size 
            line = realloc(line, sizeof(char)*(lineSize)); // line buffer has new size now

            if (!line) return line; // if we fail to allocate memory we will return NULL
        }
        if( (len == 0) && *endOfLineDetected){ // empty file
            *endOfLineDetected = 1;
            break; 
        } 
    }


    line[len++] ='\0';  // ending the string (notice there is no '\n' in the string)
    *nrOfCharRead = len;

    return line;       // return the string
}



