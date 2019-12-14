#include "find.h"

#define WORD_STR "WORD"
#define WORD_LEN 4

#define FILE_STR "FILE"
#define FILE_LEN 4

#define OCCURRENCES_STR "OCCURRENCES"
#define OCCURRENCES_LEN 11

    fWord *w = NULL;
    fWord *wordHead = NULL;
    fWord *wordTail = NULL;

    fList *list = NULL;
    fList *listHead = NULL;
    fList *listTail = NULL;

    fPath *pathHead = NULL;
    fPath *pathTail = NULL;

    llist *lHead = NULL;
    llist *lTail = NULL;
    llist *lCurr = NULL;

int main(int argc, char * argv[]){

    int verboseFlag = 0; //set to 1 if the --verbose|-v param is passed
    int outputFlag = 0; //set to 1 if the --output|-o param is passed
    char *ext = NULL; //stores the extension to exclude
    char *out = NULL; //stores the file in which write the program output

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

    //operazioni da effettuare sul file di report
    if(((strcmp(argv[1], "--report") == 0) || (strcmp(argv[1], "-r") == 0))
        && (argv[3] != NULL && (strcmp(argv[3], "--show") == 0))){
        if(argv[5] != NULL){
            if(strcmp(argv[5], "--file") == 0){        
                argumentsCheck(argv[4], "Please provide a word to search.");     
                argumentsCheck(argv[6], "Please provide a file.");   
                getWordOccurences(argv[4], argv[2], argv[6]); //find --report|-r <reportfile> --show <word> --file <file>
            }else{  
                argumentsCheck(argv[4], "Please provide a word to search.");            
                getFileList(argv[2], argv[4], atoi(argv[5])); //find --report|-r <reportfile> --show <word> <n>
            }
        }else{ 
            argumentsCheck(argv[4], "Please provide a word to search.");   
            getFileList(argv[2], argv[4], 1); //Se <n> viene omesso, si utilizza il valore 1.
        }
    //find (--words|-w) <wordfile> (--input|-i) <inputfile> 
    }else if(argc == 5 
       && ((strcmp(argv[1], "--words") == 0) || (strcmp(argv[1], "-w") == 0))
       && ((strcmp(argv[3], "--input") == 0) || (strcmp(argv[3], "-i") == 0))){
        execute(argv[2], argv[4], ext, verboseFlag, outputFlag, out);
    }else if(argc > 5
        && ((strcmp(argv[1], "--words") == 0) || (strcmp(argv[1], "-w") == 0))
        && ((strcmp(argv[3], "--input") == 0) || (strcmp(argv[3], "-i") == 0))){
            for(int i = 3; i<argc; i++){
                if((strcmp(argv[i], "--output") == 0) || (strcmp(argv[i], "-o") == 0)){
                    outputFlag = 1;
                    argumentsCheck(argv[i+1], "Please provide an output file."); 
                    out = argv[i+1];
                    i++;
                }
                if((strcmp(argv[i], "--exclude") == 0) || (strcmp(argv[i], "-e") == 0)){
                    argumentsCheck(argv[i+1], "Please provide the file extesion to exclude.");
                    ext = argv[i+1];
                    i++;
                }
                if((strcmp(argv[i], "--verbose") == 0) || (strcmp(argv[i], "-v") == 0)){
                    verboseFlag = 1;
                }
            }
            execute(argv[2], argv[4], ext, verboseFlag, outputFlag, out);
    } else {
        printf("Please provide the correct arguments.\n");
        exit(1);
    }

    freeMemory(); //FARE FREE DI TUTTE LE VARIABILI UTILIZZATE NEI NUOVI METODI
                 // LETTURA RICORSIVA E METODI PER I PARAMETRI NUOVI
    return 0;
}

void argumentsCheck(const char *args, const char *message){
    if(args == NULL){
        printf("%s\n", message);
        exit(1);
    }
}

void checkingForRecursion(llist *lCurr, const char *excluded){
    int index = 0;

    char *rec = NULL;  //will store the r that indicates if a file should be read recursively

    while(lCurr != NULL){ //traverse the lineList structure
        rec = strchr(lCurr->line, ' '); 
        if(rec != NULL) {
            rec++; //saving into rec the "r" if present
            index = 0;
            while(1){ //using this loop the save into line only the path, eliminating the "r" at the end.
                if(lCurr->line[index] == ' '){
                    lCurr->line[index] = '\0';
                    break;
                }
                index++;
            }
        }

        if(excluded != NULL){
            if(strcmp(excluded, get_filename_ext(lCurr->line)) == 0){
                lCurr = lCurr->next;
                continue;
            }
        }

        if (is_regular_file(lCurr->line)){  //If it is a file we pass to the function a zero
            listFilesRecursively(lCurr->line, rec, 0, excluded);
        }else{
            listFilesRecursively(lCurr->line, rec, 1, excluded);  //if it is a directory we pass 1.
        }

        lCurr = lCurr->next; //pointing to the next line.
    }
}

void execute(const char *wordFile, const char *inputFile, const char *excluded, const int verboseFlag, const int outputFlag, const char *outputFile){

    FILE *fp = NULL;
    clock_t t;
    clock_t t1;
    double time_taken = 0;
    char *appDir = NULL;

    lHead = createllist(lHead, lTail, inputFile);
    lCurr = lHead;

    checkingForRecursion(lCurr, excluded);

    list = listHead; //pointer back to the head of fileList

    wordHead = createfWord(wordHead, wordTail, wordFile);
    w = wordHead;

    while(w != NULL){
        if(verboseFlag == 1) printf("Inizio elaborazione parola: %s\r\n", w->word);
        while(list != NULL){
            w->p = malloc(sizeof(fPath));
            w->p->fileOccurrences = 0;
            w->p->path = list->path;
            w->p->directory = list->directory;
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

            if(verboseFlag == 1){
                if((appDir != NULL && (strcmp(appDir, list->directory) != 0)) || appDir == NULL){
                    printf("Inizio elaborazione directory: %s\n", list->directory);
                    t1 = clock();
                } 
                appDir = list->directory;
                printf("Inizio elaborazione file: %s\n", list->path);
                t = clock();
            }

            executeKMP(w, fp);

            if(verboseFlag == 1){
                t = clock() - t;
                time_taken = ((double)t)/CLOCKS_PER_SEC;
                printf("Fine elaborazione file: %s (%f)\n", list->path, time_taken);
            } 

            w->totalOccurences = w->totalOccurences + w->p->fileOccurrences;
            w->p->position = getHead(); 
            w->p = w->p->next;
            if((list->next == NULL || (appDir != NULL && (strcmp(appDir, list->next->directory) != 0))) && verboseFlag == 1){
                t1 = clock() - t1;
                time_taken = ((double)t1)/CLOCKS_PER_SEC; 
                printf("Fine elaborazione directory: %s (%f)\n", appDir, time_taken);
            }
            list = list->next;
            fclose(fp);
        }
        if(verboseFlag == 1) printf("Fine elaborazione parola: %s\n", w->word);
        w->p = pathHead;
        list = listHead;
        w = w->next;
        pathHead = NULL;
    }

    print(w, wordHead, pathHead);
    if(outputFlag == 1) writeFile(w, wordHead, pathHead, outputFile);
}

void executeKMP(fWord *w, FILE *fp){
    char *line = NULL;
    size_t len = 0;
    ssize_t line_size;
    int countLine = 0;

    while((line_size = getline(&line, &len, fp)) != -1){
        trimTrailing(line); 
        int n = strlen(line);
        int m = strlen(w->word);
        w->p->fileOccurrences = w->p->fileOccurrences + KMP(line, w->word, n, m, countLine, w->p);
        countLine = countLine + 1;
     }
}

void listFilesRecursively(char *basePath, const char *recursive, const int a, const char *exclude){

    struct dirent *dp;
    DIR *dir;

    if(a == 0){ //which means is a file
        char *str = malloc(sizeof(char)*strlen(basePath) + 1);
        strcpy(str,basePath);
        for(int i = strlen(str); i>0; i--){ //use this loop the obtain the directory, elimating all the character after the last "\"
            if(str[i] == '/'){
                str[i] = '\0';
                i = 0;
            }
        }
        listHead = createFList(listHead, listTail, basePath, str);
    }else{ //if it's a directory
        char *path = malloc(sizeof(char)*1000); //will store the path
        dir = opendir(basePath); //open the directory
      
        if (!dir) return;

        while ((dp = readdir(dir)) != NULL){ //we traverse the directory
            if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){

                // Construct new path from our base path
                strcpy(path, basePath);
                strcat(path, "/"); 
                strcat(path, dp->d_name); //adding the file name to the path
                
                if (is_regular_file(path)){
                    if(exclude != NULL){
                        if(strcmp(exclude, get_filename_ext(path)) == 0){                           
                            continue;
                        }
                    }
                    listHead = createFList(listHead, listTail, strdup(path), basePath);
                }

                if(recursive == NULL){ //in case we don't need to read recursively
                    continue;
                } 
                    
                if(strcmp(recursive, "r") == 0){   //if we passed a "r" to the function, the program start read recursively all the subdirectories
                    listFilesRecursively(path, "r", 1, exclude);
                }
            }
        }
        closedir(dir);
    }
}

void getWordOccurences(const char *word, const char *file, const char *fileToCheck){
    FILE *f;
    char *curr = NULL;
    size_t len = 0;
    ssize_t line_size;
    char *currentWord = NULL;
    char *currentFile = NULL;
    char *p = NULL;
    int check = 0;

    if(fileToCheck == NULL){
        printf("Pass a file as arguments\r\n");
        exit(1);
    }else{
        f = fopen(file, "r");
        if(f == NULL){
            fprintf(stderr, "Cannot open %s, exiting. . .\n", file);
            exit(1);
        }
        while((line_size = getline(&curr, &len, f)) != -1){
            trimTrailing(curr);
            if(strncmp(curr, WORD_STR, WORD_LEN) == 0){
                currentWord = (char*)malloc((strlen(curr) - WORD_LEN + 1)*sizeof(char));
                sscanf(strchr(curr, ' '), "%s", currentWord);
                p = NULL;
                continue;
            }
            if(strncmp(curr, FILE_STR, FILE_LEN) == 0){
                currentFile = (char*)malloc((strlen(curr) - FILE_LEN + 1)*sizeof(char));
                sscanf(strchr(curr, ' '), "%s", currentFile);
                continue;
            }
            if(strncmp(curr, OCCURRENCES_STR, OCCURRENCES_LEN) == 0){
                p = (char*)malloc((strlen(curr) - OCCURRENCES_LEN+1)*sizeof(char));
                sscanf(strchr(curr, ' '), "%s", p);
                continue;
            }
            if((strcmp(currentWord, word) == 0) && currentWord != NULL){
                if(currentFile != NULL && (strcmp(currentFile, fileToCheck) == 0)){
                    if(p != NULL && (atoi(p) > 0)){
                        check = 1;
                        printf("%s\n", curr);
                        continue;
                    }
                }         
            }
        }

        if(check == 0) printf("The word %s doesn't occur in the file %s \n", word, fileToCheck);
            
        fclose(f);
    }
}

void getFileList(const char *reportFile, const char *wordtoCheck, const int occurr){
    FILE * report;
    char *line = NULL;
    size_t len = 0;
    ssize_t line_size;
    char *word = NULL;
    char *file = NULL;
    char *p = NULL;
    int check = 0;

    report = fopen(reportFile, "r");

    if(report == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", reportFile);
    	exit(1);
    }

    while((line_size = getline(&line, &len, report)) != -1){
        trimTrailing(line);
        if(strncmp(line, WORD_STR, WORD_LEN) == 0){
            word = (char*)malloc((strlen(line) - WORD_LEN + 1)*sizeof(char));
            sscanf(strchr(line, ' '), "%s", word);
            p = NULL;
            continue;
        }
        if(strncmp(line, FILE_STR, FILE_LEN) == 0){
            file = (char*)malloc((strlen(line)- FILE_LEN + 1)*sizeof(char));
            sscanf(strchr(line, ' '), "%s", file);
            continue;
        }
        if(strncmp(line, OCCURRENCES_STR, OCCURRENCES_LEN) == 0){
            p = (char*)malloc((strlen(line) - OCCURRENCES_LEN + 1)*sizeof(char));
            sscanf(strchr(line, ' '), "%s", p);
            continue;
        }

        if(word!= NULL && (strcmp(word, wordtoCheck) == 0)){
            if(p != NULL && (atoi(p) >= occurr)){
                check = 1;
                printf("%s\n", file);
                p = NULL;
                continue;
            }
        }
    }
    if(check == 0) printf("The word %s doesn't occur in the searched file.\n", wordtoCheck);
    fclose(report);
}
  
void print(fWord *w, fWord *wordHead, fPath *pathHead){
     
    fPosition *positionHead = NULL;
    w = wordHead;

    while(w != NULL){
        printf("WORD %s\n", w->word);
        printf("TOTAL %d\n", w->totalOccurences);
        pathHead = w->p;
        while(w->p != NULL){
            printf("FILE %s\n", w->p->path);
            printf("OCCURRENCES %d\n", w->p->fileOccurrences);
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

    w = wordHead;

    printf("\n"); //il file termina con una riga vuota
}

void writeFile(fWord *w, fWord *wordHead, fPath *pathHead, const char * outputFile){
    
    FILE *fOutput;

    fOutput = freopen(outputFile, "w", stdout);

    if(fOutput == NULL){
        fprintf(stderr, "Problem while creating the file %s, exiting . . .\n", outputFile);
    	exit(1);
    }

    print(w, wordHead, pathHead);

    fclose(fOutput);
}

void freeMemory(){

    lCurr = lHead;
    llist *templl = NULL;

    while(lCurr != NULL){
        templl = lCurr;
        lCurr = lCurr->next;
        templl->next = NULL;
        free(templl->line);
        free(templl);
    }

    list = listHead;
    fList *tempL = NULL;
    while(list != NULL){
        tempL = list;
        list = list->next;
        tempL->next = NULL;
        // free(tempL->directory);
        // free(tempL->path);
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
                tempO->next = NULL;
                free(tempO);
            }
            tempP = w->p;
            w->p = w->p->next;
            tempP->next = NULL;
            free(tempP->path);
            free(tempP->directory);
            free(tempP->position);
            free(tempP);
        }
        tempW = w;
        w = w->next;
        tempW->next = NULL;
        free(tempW->p);
        free(tempW->word);
        free(tempW);
    }
}

char *get_filename_ext(const char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}  

int is_regular_file(const char *path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}