#include "find.h"

//macros used during output file analyzing 
#define WORD_STR "WORD"
#define WORD_LEN 4

#define FILE_STR "FILE"
#define FILE_LEN 4

#define OCCURRENCES_STR "OCCURRENCES"
#define OCCURRENCES_LEN 11

//pointers used to manage the fWord struct which store the list of word to analyze.
fWord *w = NULL;
fWord *wordHead = NULL;
fWord *wordTail = NULL;

//pointers used to manage the fList struct which is used during recursive reading.
fList *list = NULL;
fList *listHead = NULL;
fList *listTail = NULL;

//pointers used to manage the fPath struct which store the list of path, their information and the occurrences of the word.
fPath *pathHead = NULL;
fPath *pathTail = NULL;

//pointers used to manage the llist struct which store the list of path to analyze.
llist *lHead = NULL;
llist *lTail = NULL;
llist *lCurr = NULL;

//keep track of the execution times, one for the files, the other for directories
clock_t tFile;
clock_t tDirectory;


int main(int argc, char * argv[]){

    int verboseFlag = 0; //set to 1 if the --verbose|-v param is passed
    int outputFlag = 0; //set to 1 if the --output|-o param is passed
    char *ext = NULL; //stores the extension to exclude
    char *out = NULL; //stores the file in which write the program output

    //if no argument is passed, the program prints out the list of possible commands
    if(argc == 1 || strcmp(argv[1], "--help") == 0){ 
		printHelp();
		return 0;
	}

    //passing the command --test, the program executes some test on all the possibile commands and other important function.
    //the files and directories used for testing are stored in the directory "test" located in the project folder.
    if(strcmp(argv[1], "--test") == 0){
        runTest();
        return 0;
    }

    //some nested "if" which check the input arguments and launch the correct execution
    if(((strcmp(argv[1], "--report") == 0) || (strcmp(argv[1], "-r") == 0)) //this if checks the output file operations
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
            getFileList(argv[2], argv[4], 1); //If <n> is omitted, the value "1" is used.
        }
    //this is check for the word searching operations
    }else if(argc == 5 
       && ((strcmp(argv[1], "--words") == 0) || (strcmp(argv[1], "-w") == 0))
       && ((strcmp(argv[3], "--input") == 0) || (strcmp(argv[3], "-i") == 0))){
        execute(argv[2], argv[4], ext, verboseFlag, outputFlag, out); //find (--words|-w) <wordfile> (--input|-i) <inputfile> 
    }else if(argc > 5
        && ((strcmp(argv[1], "--words") == 0) || (strcmp(argv[1], "-w") == 0))
        && ((strcmp(argv[3], "--input") == 0) || (strcmp(argv[3], "-i") == 0))){
            for(int i = 3; i<argc; i++){
                if((strcmp(argv[i], "--output") == 0) || (strcmp(argv[i], "-o") == 0)){ //find (--words|-w) <wordfile> (--input|-i) <inputfile> (--output|-o)
                    outputFlag = 1;
                    argumentsCheck(argv[i+1], "Please provide an output file."); 
                    out = argv[i+1];
                    i++;
                }
                if((strcmp(argv[i], "--exclude") == 0) || (strcmp(argv[i], "-e") == 0)){ //find (--words|-w) <wordfile> (--input|-i) <inputfile> (--exclude|-e) <ext>
                    argumentsCheck(argv[i+1], "Please provide the file extesion to exclude.");
                    ext = argv[i+1];
                    i++;
                }
                if((strcmp(argv[i], "--verbose") == 0) || (strcmp(argv[i], "-v") == 0)){ //find (--words|-w) <wordfile> (--input|-i) <inputfile> (--verbose|-v)
                    verboseFlag = 1;
                }
            }
            //based on the value setted by the nested IFs we launch the words searching
            execute(argv[2], argv[4], ext, verboseFlag, outputFlag, out);
    } else { //incorrect value
        printf("Please provide the correct arguments.\nLaunch the program with no parameters or using --help to see all the possibile executions.\n");
        exit(1);
    }
    freeMemory();
    return 0;
}

//prints the list of possible commands
void printHelp(){
    printf("\nThe find program allows you to find the number of occurrences of a set of strings within a group of files.\n");
	printf("To run the program use the following parameters:\n");
    printf("\nPrincipal execution:\n");
	printf("-\t[ --word|-w <wordfile> --input|-i <inputFile> ] : to print the report at the end of the run\n");
	printf("\nOptional parameters:\n");
	printf("-\t[ --output|-o <outputfile> ] : to save the report on a particular file\n");
	printf("-\t[ --exclude|-e <ext> ] : during the analysis phase it will be possible to ignore files with specific extensions\n");
	printf("-\t[ --verbose|-v ] : to view the analysis process\n");
	printf("\nOnce the report file has been generated, the find program can be used to retrieve the saved information with the following commands:\n");
	printf("-\t[ --report|-r <reportfile> --show <word> <n> ] : print the file list where the word <word> occurs at least <n> times\n");
	printf("-\t[ --report|-r <reportfile> --show <word> --file <file> ] : print all positions where the word <word> occurs in the <file>\n");
    printf("\nTo run some tests on the program:\n");
    printf("-\t[ --test ] : performs the tests contained in the test.c file\n\n");
}

//in case of invalid arguments, prints an error message
void argumentsCheck(const char *args, const char *message){
    if(args == NULL){
        printf("%s\n", message);
        exit(1);
    }
}

//gets the struct that contains all the lines read from the input file and looks for the "r" at the end which indicates recursive read
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

        //discards all the files with the extension to exclude (in case has been passed)
        if(excluded != NULL){ 
            if(strcmp(excluded,getFilenameExt(lCurr->line)) == 0){
                lCurr = lCurr->next;
                continue;
            }
        }
        if (isRegularFile(lCurr->line)){  //If it is a file we pass to the function a zero
            listFilesRecursively(lCurr->line, rec, 0, excluded);
        }else{
            listFilesRecursively(lCurr->line, rec, 1, excluded);  //if it is a directory we pass 1.
        }

        lCurr = lCurr->next; //pointing to the next line.
    }
}

//starts the time tracking for files and directory
void startTime(char **appDir, fList *list){
    //if the directories open is changed, starts a new timer
    if((*appDir != NULL && (strcmp(*appDir, list->directory) != 0)) || *appDir == NULL){
        printf("Start processing directory: %s\n", list->directory);
        tDirectory = clock();
    } 
    *appDir = list->directory;
    printf("Start processing file: %s\n", list->path);
    tFile = clock();
}

//prints the elapsed time during files and directories elaboration
void elapsedTime(char **appDir, fList *list){
    double time_taken = 0;
    tFile = clock() - tFile;
    time_taken = ((double)tFile)/CLOCKS_PER_SEC;
    printf("Terminate processing file: %s (%f)\n", list->path, time_taken);
    //if the next file is located into a different directory, prints the current directory elaboration time
    if(list->next == NULL || (appDir != NULL && (strcmp(*appDir, list->next->directory)) != 0)){
        tDirectory = clock() - tDirectory;
        time_taken = ((double)tDirectory)/CLOCKS_PER_SEC; 
        printf("Terminate processing directory: %s (%f)\n", *appDir, time_taken);
    }
}

//executes the words searching operation. Based on the param passed in input, it executes a different features.
void execute(const char *wordFile, const char *inputFile, const char *excluded, const int verboseFlag, const int outputFlag, const char *outputFile){

    FILE *fp = NULL;
    char *appDir = NULL;

    //creates the structure that stores the lines read from the input file which contains the paths
    lHead = createllist(lHead, lTail, inputFile);
    lCurr = lHead;

    //checks if some path need to be opened recursively
    checkingForRecursion(lCurr, excluded);

    list = listHead; //pointer back to the head of fileList

    //creates the structure that stores the words to search
    wordHead = createfWord(wordHead, wordTail, wordFile);
    w = wordHead;

    while(w != NULL){
        if(verboseFlag == 1) printf("Start processing word: %s\r\n", w->word);
        while(list != NULL){

            //creates the structure that stores all the files where the program searched and the word occurrences founded in them.
            pathHead = createfPath(&w->p, &pathHead, &pathTail, list->path, list->directory);

            fp = fopen(w->p->path, "r"); 

            if(fp == NULL){
                fprintf(stderr, "Cannot open %s, exiting. . .\n", w->p->path);
                exit(1);
            }

            if(verboseFlag == 1) startTime(&appDir, list);

            executeKMP(w, fp); //execute the KMP algorithm

            if(verboseFlag == 1) elapsedTime(&appDir, list);
            
            list = list->next;
            fclose(fp);
        }
        if(verboseFlag == 1) printf("Terminate processing word: %s\n", w->word);
        w->p = pathHead;
        list = listHead;
        w = w->next;
        pathHead = NULL;
    }

    print(w, wordHead, pathHead);
    if(outputFlag == 1) writeFile(w, wordHead, pathHead, outputFile);
}

//execute the KMP algorithm on the file, and word passed in input
void executeKMP(fWord *w, FILE *fp){
    char *line = NULL;
    size_t len = 0;
    ssize_t line_size;
    int countLine = 0;

    while((line_size = getline(&line, &len, fp)) != -1){
        trimTrailing(line); //deletes all the blank spaces after the read line
        int n = strlen(line);
        int m = strlen(w->word);
        w->p->fileOccurrences = w->p->fileOccurrences + KMP(line, w->word, n, m, countLine, w->p); //save the number of occurrences founded
        countLine = countLine + 1; //increments the number of lines
     }
     w->totalOccurences = w->totalOccurences + w->p->fileOccurrences; //increments the number of total occurrences
            w->p->position = getHead(); //pointer back to head of the list
            w->p = w->p->next;
}

//obtains the directory of a specific path
char *getDirectory(const char *basePath){
    char *str = malloc(sizeof(char)*strlen(basePath) + 1);
    strcpy(str,basePath);
    for(int i = strlen(str); i>0; i--){ //use this loop the obtain the directory, elimating all the character after the last "/"
        if(str[i] == '/'){
            str[i] = '\0';
            i = 0;
        }
    }
    return str;
}

//lists all the file into the correct structure and, if requested, opens them recursively
void listFilesRecursively(char *basePath, const char *recursive, const int a, const char *exclude){
    struct dirent *dp;
    DIR *dir;

    if(a == 0){ //which means is a file
        listHead = createFList(listHead, listTail, basePath, getDirectory(basePath));
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
                
                if(isRegularFile(path)){
                    if(exclude != NULL){
                        if(strcmp(exclude, getFilenameExt(path)) == 0){                           
                            continue;
                        }
                    }
                    listHead = createFList(listHead, listTail, strdup(path), strdup(basePath));
                }

                if(recursive == NULL){ //in case we don't need to read recursively
                    continue;
                } 
                    
                if(strcmp(recursive, "r") == 0){  //if we passed a "r" to the function, the program start read recursively all the subdirectories
                    listFilesRecursively(path, "r", 1, exclude);
                }
            }
        }
        closedir(dir);
    }
}

//returns 1 when WORD, FILE or OCCURRENCES are founded in the output file. Otherwise returns 0.
int analyzeOutputFile(char **currentWord, char **currentFile, char **p, char **line){
    if(strncmp(*line, WORD_STR, WORD_LEN) == 0){
        *currentWord = (char*)malloc((strlen(*line) - WORD_LEN + 1)*sizeof(char));
        sscanf(strchr(*line, ' '), "%s", *currentWord);
        *p = NULL;
        return 1;
    }
    if(strncmp(*line, FILE_STR, FILE_LEN) == 0){
        *currentFile = (char*)malloc((strlen(*line) - FILE_LEN + 1)*sizeof(char));
        sscanf(strchr(*line, ' '), "%s", *currentFile);
        return 1;
    }
    if(strncmp(*line, OCCURRENCES_STR, OCCURRENCES_LEN) == 0){
        *p = (char*)malloc((strlen(*line) - OCCURRENCES_LEN+1)*sizeof(char));
        sscanf(strchr(*line, ' '), "%s", *p);
        return 1;
    }

    return 0;
}

//prints the all the occurrences of a word in a specific file passed in input. 
void getWordOccurences(const char *word, const char *file, const char *fileToCheck){
    FILE *f;
    char *line = NULL;
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
        while((line_size = getline(&line, &len, f)) != -1){
            trimTrailing(line);
            if(analyzeOutputFile(&currentWord, &currentFile, &p, &line) == 1) continue;
            if((strcmp(currentWord, word) == 0) && currentWord != NULL){
                if(currentFile != NULL && (strcmp(currentFile, fileToCheck) == 0)){
                    if(p != NULL && (atoi(p) > 0)){
                        check = 1;
                        printf("%s\n", line);
                        continue;
                    }
                }         
            }
        }

        if(check == 0) printf("The word %s doesn't occur in the file %s \n", word, fileToCheck);
            
        fclose(f);
    }
}

//prints the list of file in which the word passed in input occurs at least "occurr" times.
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
        if(analyzeOutputFile(&word, &file, &p, &line) == 1) continue;
        if(word!= NULL && (strcmp(word, wordtoCheck) == 0)){
            if(p != NULL && (atoi(p) >= occurr)){
                check = 1;
                printf("%s\n", file);
                p = NULL;
                continue;
            }
        }
    }
    if(check == 0) printf("The word %s doesn't occur at least %d times in none of the analyzed file.\n", wordtoCheck, occurr);
    fclose(report);
}
  
//prints the report returned from the "execute()" function.
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

//prints on a file the report returned from the "execute()" function.
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

//returns the exetension of the file passed in input
char *getFilenameExt(const char *filename) {
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}  

//checks if the path passed in input is a file.
int isRegularFile(const char *path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

//free the allocated memory
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