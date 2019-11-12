#include "find.h"

    FILE *fInput = NULL;  
    FILE *fp = NULL;
    char *line1; 
    char *line2;
    int endOfLineDetected = 0;
    size_t nrOfCharRead = 0;
    char ch;

    fWord *w = NULL;
    fWord *wordHead = NULL;
    fWord *wordTail = NULL;

    fList *list = NULL;
    fList *listHead = NULL;
    fList *listTail = NULL;

    fPath *pathHead = NULL;
    fPath *pathTail = NULL;

    fPosition *positionHead = NULL;

int main(int argc, char * argv[]){

    if(argc == 1){ //stampa le informazioni sul programma
		printf("Il programma find consente di individuare il numero di occorrenze di un insieme di stringhe all'interno di un grpathHeadpo di file.\r\n");
		printf("Per eseguire il programma utilizzare i seguenti parametri:\r\n");
		printf(" find --word|-w <inputfile> : per stampare il report alla fine dell'esecuzione\r\n");
		printf("Parametri opzionali:\r\n");
		printf(" --output|-o <outputfile> : per salvare il report su un file particolare\r\n");
		printf(" --exclude|-e <ext> : durante la fase di analisi sara' possibile ignorare i file con specifiche estensioni\r\n");
		printf(" --verbose|-v : per visionare il processo di analisi\r\n");
		printf("Una volta generato il file di report, il programma find potra' essere usato per recpathHeaderare\r\n");
		printf("le informazioni salvate con i seguenti comandi:\r\n");
		printf(" find --report|-r <reportfile> --show <word> <n> : stampare la lista del file dove occorre almeno <n> volte la parola <word>\r\n");
		printf(" find --report|-r <reportfile> --show <word> --file <file> : stampare tutte le posizioni dove la parola <word> occorre nel <file>\r\n");
		return 0;
	}

     //find --words|-w <inputfile> 
    if(argc == 3 && ((strcmp(argv[1], "--words") == 0) || (strcmp(argv[1], "-w") == 0))){
        execute(argv[2]);
        print(w, wordHead);
    }else if(argc > 3){
        for(int i = 3; i<argc; i++){
            if((strcmp(argv[i], "--output") == 0) || (strcmp(argv[i], "-o") == 0)){
                execute(argv[2]);
                //bisogna anche stampare su console???
                writeFile(w, wordHead, argv[i+1]);
            }
            if((strcmp(argv[i], "--exclude") == 0) || (strcmp(argv[i], "-e") == 0)){
                //esclude alcuni path quindi non devo usare execute ma un metodo nuovo
            }
            if((strcmp(argv[i], "--verbose") == 0) || (strcmp(argv[i], "-v") == 0)){
                executeVerbose(argv[2]);
                print(w,wordHead);
            }
        }

    }

    //operazioni da effettuare sul file di report
    if(((strcmp(argv[1], "--report") == 0) || (strcmp(argv[1], "-r") == 0)) && (strcmp(argv[3], "--show") == 0)){
        if(strcmp(argv[5], "--file") == 0){
            
        }else{ //fine --report|-r <reportfile> --show <word> <n>
            //Stampare la lista dei file dove occorre almeno <n> volte la parola <word>:
            printf("asdfgfsadfsdggfa");
            getFileOccurrences(argv[2]);

        }
    }



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
    line = realloc(NULL, sizeof(char)*lineSize); // expected size of the line is pathHead to typicalSize

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

void getFileOccurrences(char *reportFile){
    FILE * report;
    char *line;
    endOfLineDetected = 0;
    nrOfCharRead = 0;

    report = fopen(reportFile, "r");
    if(report == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", reportFile);
    	exit(1);
    }

    while(!endOfLineDetected){
        line = getLineOfAnySize(report,128,&endOfLineDetected,&nrOfCharRead);
        printf("%s\r\n", line);
    }

    fclose(report);
}
    
void execute(char *inputFile){
    fInput = fopen(inputFile, "r"); //the file that contains the path of the file in which search.

    if(fInput == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", inputFile);
    	exit(1);
    }
    
    while(!endOfLineDetected){ //read line by line the input file in order to save the path in a structure
        line1 = getLineOfAnySize(fInput,128,&endOfLineDetected,&nrOfCharRead);
        fList *node = malloc (sizeof(fList));
        node->path = line1;
        node->next = NULL;

        if(listHead == NULL){
            listHead = listTail = node;
        }else{
            listTail = listTail->next = node;
        }
    }

    list = listHead;

    fclose(fInput);

    do{
        fWord *app = malloc(sizeof(fWord));
        printf("Insert the word to search: ");
        scanf("%s", app->word);
        app->totalOccurences = 0;
        app->p = NULL;
        app->next = NULL;

        if(wordHead == NULL){
            wordTail = wordHead = app;
        }else{
            wordTail = wordTail->next = app;
        }
        printf("Do you want to insert another word? (Y/N): ");
        scanf(" %c", &ch);
    }while(ch == 'y' || ch == 'Y');

    w = wordHead;

    while(w != NULL){
        while(list != NULL){
            w->p = malloc(sizeof(fPath));
            w->p->fileOccurrences = 0;
            w->p->path = list->path;
            w->p->position = NULL;
            w->p->next = NULL;

            if(pathHead == NULL){
                pathTail = pathHead = w->p;
            }else{
                pathTail = pathTail->next = w->p;
            }

            fp = fopen(w->p->path, "r"); 
            if(fp == NULL){
                fprintf(stderr, "Cannot open %s, exiting. . .\n", w->p->path);
                exit(1);
            }

            int countLine = 0;
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
            list = list->next;
            fclose(fp);
        }
        w->p = pathHead;
        list = listHead;
        w = w->next;
        pathHead = NULL;
    }
}

void print(fWord *w, fWord *wordHead){
     
    w = wordHead;

    while(w != NULL){
        printf("WORD %s \r\n", w->word);
        printf("TOTAL %d \r\n", w->totalOccurences);
        pathHead = w->p;
        while(w->p != NULL){
            printf("FILE %s \r\n", w->p->path);
            printf("OCCURENCES %d	\r\n", w->p->fileOccurrences);
            positionHead = w->p->position;
            while (w->p->position != NULL){
                printf("%d %d\r\n", w->p->position->line, w->p->position->character);
                w->p->position = w->p->position->next;
            }
            w->p->position = positionHead;
            w->p = w->p->next;
        }
        w->p = pathHead;
        w = w->next;
    }

    w = wordHead;

    printf("\r\n"); //il file termina con una riga vuota
}

void writeFile(fWord *w, fWord *wordHead, char * outputFile){
    
    FILE *fOutput;

    w = wordHead;

    fOutput = freopen(outputFile, "w", stdout);

    while(w != NULL){
        printf("WORD %s\n", w->word);
        printf("TOTAL %d\n", w->totalOccurences);
        pathHead = w->p;
        while(w->p != NULL){
            printf("FILE %s\n", w->p->path);
            printf("OCCURENCES %d\n", w->p->fileOccurrences);
            positionHead = w->p->position;
            while (w->p->position != NULL){
                printf("%d %d\n", w->p->position->line, w->p->position->character);
                w->p->position = w->p->position->next;
            }
            w->p->position = positionHead;
            w->p = w->p->next;
        }
        w->p = pathHead;
        w = w->next;
    }

    printf("\n");
}


void executeVerbose(char *inputFile){

    clock_t t;

    fInput = fopen(inputFile, "r"); //the file that contains the path of the file in which search.

    if(fInput == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", inputFile);
    	exit(1);
    }
    
    while(!endOfLineDetected){ //read line by line the input file in order to save the path in a structure
        line1 = getLineOfAnySize(fInput,128,&endOfLineDetected,&nrOfCharRead);
        fList *node = malloc (sizeof(fList));
        node->path = line1;
        node->next = NULL;

        if(listHead == NULL){
            listHead = listTail = node;
        }else{
            listTail = listTail->next = node;
        }
    }

    list = listHead;

    fclose(fInput);

    do{
        fWord *app = malloc(sizeof(fWord));
        printf("Insert the word to search: ");
        scanf("%s", app->word);
        app->totalOccurences = 0;
        app->p = NULL;
        app->next = NULL;

        if(wordHead == NULL){
            wordTail = wordHead = app;
        }else{
            wordTail = wordTail->next = app;
        }
        printf("Do you want to insert another word? (Y/N): ");
        scanf(" %c", &ch);
    }while(ch == 'y' || ch == 'Y');

    w = wordHead;

    while(w != NULL){
        printf("Inizio elaborazione parola: %s\r\n", w->word);
        while(list != NULL){
            w->p = malloc(sizeof(fPath));
            w->p->fileOccurrences = 0;
            w->p->path = list->path;
            w->p->position = NULL;
            w->p->next = NULL;

            if(pathHead == NULL){
                pathTail = pathHead = w->p;
            }else{
                pathTail = pathTail->next = w->p;
            }

            fp = fopen(w->p->path, "r"); 
            if(fp == NULL){
                fprintf(stderr, "Cannot open %s, exiting. . .\n", w->p->path);
                exit(1);
            }

            printf("Inizio elaborazione directory: DA FARE\r\n");
            printf("Inizio elaborazione file: %s\r\n", list->path);
            t = clock();

            int countLine = 0;
            endOfLineDetected = 0;

            while(!endOfLineDetected){
                line2 = getLineOfAnySize(fp,128,&endOfLineDetected,&nrOfCharRead);
                int n = strlen(line2);
                int m = strlen(w->word);
                w->p->fileOccurrences = w->p->fileOccurrences + KMP(line2, w->word, n, m, countLine, w->p);
                countLine = countLine + 1;
            }   

            t = clock() - t;
            double time_taken = ((double)t)/CLOCKS_PER_SEC;

            printf("Fine elaborazione file: %s (%f)\r\n", list->path, time_taken);
            w->totalOccurences = w->totalOccurences + w->p->fileOccurrences;
            w->p->position = getHead(); 
            w->p = w->p->next;
            list = list->next;
            fclose(fp);
        }
        printf("Fine elaborazione parola: %s\r\n", w->word);
        w->p = pathHead;
        list = listHead;
        w = w->next;
        pathHead = NULL;
    }
}



