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

    int var = 0;

    int outputFlag = 0;

    char *ext = NULL;
    char *out = NULL;

int main(int argc, char * argv[]){

    if(argc == 1){ //stampa le informazioni sul programma
		printf("Il programma find consente di individuare il numero di occorrenze di un insieme di stringhe all'interno di un grpathHeadpo di file.\r\n");
		printf("Per eseguire il programma utilizzare i seguenti parametri:\r\n");
		printf(" find --word|-w <wordfile> --input|-i <inputFile> : per stampare il report alla fine dell'esecuzione\r\n");
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

    //find (--words|-w) <wordfile> (--input|-i) <inputfile> 
    if(argc == 5 
       && ((strcmp(argv[1], "--words") == 0) || (strcmp(argv[1], "-w") == 0))
       && ((strcmp(argv[3], "--input") == 0) || (strcmp(argv[3], "-i") == 0))){
        execute(NULL, argv[4], ext, var);
        print(w, wordHead);
    }else if(argc > 5){
        //operazioni da effettuare sul file di report
        if(((strcmp(argv[1], "--report") == 0) || (strcmp(argv[1], "-r") == 0)) && (strcmp(argv[3], "--show") == 0)){
                if(argv[5] != NULL){
                if(strcmp(argv[5], "--file") == 0){
                    //fine --report|-r <reportfile> --show <word> --file <file>
                    //Stampare tutte le posizioni dove la parola <word> occorre nel file <file>:
                    //Se <word> non occorre in <file>, viene stampato a video un messaggio opportuno.
                    getWordOccurences(argv[4], argv[2], argv[6]);
                }else{
                    //find --report|-r <reportfile> --show <word> <n>
                    //Stampare la lista dei file dove occorre almeno <n> volte la parola <word>:
                    getFileList(argv[2], argv[4], atoi(argv[5]));
                }
                }else{
                    //Se <n> viene omesso, si utilizza il valore 1.
                getFileList(argv[2], argv[4], 1);
                }
        }else{
            for(int i = 3; i<argc; i++){
                if((strcmp(argv[i], "--output") == 0) || (strcmp(argv[i], "-o") == 0)){
                    outputFlag = 1;
                    out = argv[i+1];
                    i++;
                }
                if((strcmp(argv[i], "--exclude") == 0) || (strcmp(argv[i], "-e") == 0)){
                    ext = argv[i+1];
                    i++;
                }
                if((strcmp(argv[i], "--verbose") == 0) || (strcmp(argv[i], "-v") == 0)){
                    var = 1;
                    print(w,wordHead);
                }
            }
            execute(NULL, argv[2], ext, var);
            print(w, wordHead);

            if(outputFlag == 1){
                writeFile(w, wordHead, out);
            }
        } 
    }

    freeMemory();
    freeKMP();

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

void getWordOccurences(char *word, char *file, char *fileToCheck){
    FILE *f;
    char *curr;
    endOfLineDetected = 0;
    nrOfCharRead = 0;
    char *currentWord;
    char *currentFile;
    char *p;

    f = fopen(file, "r");
    if(f == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", file);
    	exit(1);
    }


    while(!endOfLineDetected){
        curr = getLineOfAnySize(f,128,&endOfLineDetected,&nrOfCharRead);
        if(curr[0] == 'W'){
            currentWord = strrchr(curr, ' ');
            currentWord++;
            continue;
        }
        if(curr[0] == 'F'){
            currentFile = strrchr(curr, ' ');
            currentFile++;
            continue;
        }
        if(curr[0] == 'O'){
            p = strrchr(curr, ' ');
            p++;
            continue;
        }
        if(strcmp(currentWord, word) == 0){
            if(strcmp(currentFile, fileToCheck) == 0){
                if(atoi(p) > 0){
                    printf("%s\r\n", curr);
                    continue;
                }else{
                    printf("La parola %s non occorre nel file %s\r\n", word, fileToCheck);
                    break;
                }
            }
                    
        }
    }
    fclose(f);
    free(currentWord);
    free(currentFile);
    free(p);
}

void getFileList(char *reportFile, char *wordtoCheck, int occurr){
    FILE * report;
    char *line;
    endOfLineDetected = 0;
    nrOfCharRead = 0;
    char *word;
    char *file;
    char *p;

    report = fopen(reportFile, "r");
    if(report == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", reportFile);
    	exit(1);
    }

    while(!endOfLineDetected){
        line = getLineOfAnySize(report,128,&endOfLineDetected,&nrOfCharRead);
        if(line[0] == 'W'){
            word = strrchr(line, ' ');
            word++;  
            continue;
        }
        if(line[0] == 'F'){
            file = strrchr(line, ' ');
            file++;
            continue;
        }
        if(line[0] == 'O'){
            p = strrchr(line, ' ');
            p++;
            if(strcmp(word, wordtoCheck) == 0){
                if(atoi(p) >= occurr){
                    printf("%s\r\n", file);
                }
            }
        }
    }
    fclose(report);
    free(word);
    free(file);
    free(p);
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

    fclose(fOutput);
}

void execute(char *wordFile, char *inputFile, char *excluded, int num){

    clock_t t;

    double time_taken = 0;

    fInput = fopen(inputFile, "r"); //the file that contains the path of the file in which search.

    if(fInput == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", inputFile);
    	exit(1);
    }else{
        printf("Opened %s successfully\r\n", inputFile);
    }
    
    while(!endOfLineDetected){ //read line by line the input file in order to save the path in a structure
        line1 = getLineOfAnySize(fInput,128,&endOfLineDetected,&nrOfCharRead);
        //if(endOfLineDetected) break; //questo
        if(excluded != NULL){
            if(strcmp(excluded, get_filename_ext(line1)) == 0){
                continue;
            }
        }
        fList *node = malloc (sizeof(fList));
        node->path = line1;
        node->next = NULL;

        if(listHead == NULL){
            listTail = listHead = node;
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
        if(num == 1) printf("Inizio elaborazione parola: %s\r\n", w->word);
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
            }else{
                printf("Opened %s successfully\r\n", w->p->path);
            }

            if(num == 1) printf("Inizio elaborazione directory: DA FARE\r\n");
            if(num == 1) printf("Inizio elaborazione file: %s\r\n", list->path);
            if(num == 1) t = clock();

            int countLine = 0;
            endOfLineDetected = 0;

            while(!endOfLineDetected){
                line2 = getLineOfAnySize(fp,128,&endOfLineDetected,&nrOfCharRead);
                int n = strlen(line2);
                int m = strlen(w->word);
                w->p->fileOccurrences = w->p->fileOccurrences + KMP(line2, w->word, n, m, countLine, w->p);
                countLine = countLine + 1;
            }   

            if(num == 1){
                t = clock() - t;
                time_taken = ((double)t)/CLOCKS_PER_SEC;
            } 
                
            if(num == 1) printf("Fine elaborazione file: %s (%f)\r\n", list->path, time_taken);
            w->totalOccurences = w->totalOccurences + w->p->fileOccurrences;
            w->p->position = getHead(); 
            w->p = w->p->next;
            list = list->next;
            fclose(fp);
        }
        if(num == 1) printf("Fine elaborazione parola: %s\r\n", w->word);
        w->p = pathHead;
        list = listHead;
        w = w->next;
        pathHead = NULL;
    }
}

void freeMemory(){

    list = listHead;
    fList *tempL = NULL;
    while(list != NULL){
        tempL = list;
        list = list->next;
        free(tempL);
    }

    w = wordHead;
    fWord *tempW = NULL;
    fPath *tempP = NULL;
    fPosition *tempO = NULL;
    while(w != NULL){
        while(w->p != NULL){
            while(w->p->position != NULL){
                tempO = w->p->position;
                w->p->position = w->p->position->next;
                free(tempO);
            }
            tempP = w->p;
            w->p = w->p->next;
            free(tempP);
        }
        tempW = w;
        w = w->next;
        free(tempW);
    }

    free(line1);
    free(line2);
    free(wordHead);
    free(wordTail);
    free(listHead);
    free(listTail);
    free(pathHead);
    free(pathTail);
    free(positionHead);
}

char *get_filename_ext(char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}   