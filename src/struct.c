#include "../include/struct.h"

//sorts the list of file in descending order by file occurrences
void sortFileByOccurrences(fPath *head){
    int swapped;
    fPath *ptr1;
    fPath *lptr = NULL;

    //checking for empty list
    if(head == NULL) return;

    do{
        swapped = 0;
        ptr1 = head;

        while(ptr1->next != lptr){
            if(ptr1->fileOccurrences < ptr1->next->fileOccurrences){
                swapfPath(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }while(swapped);
}

//swaps the data of the fPath element
void swapfPath(fPath *a, fPath *b){
    char *dir = a->directory;
    char *path = a->path; 
    int fileOccur = a->fileOccurrences;
    fPosition *p = a->position;
    a->directory = b->directory;
    a->path = b->path;
    a->fileOccurrences = b->fileOccurrences;
    a->position = b->position;
    b->directory = dir;
    b->path = path;
    b->fileOccurrences = fileOccur;
    b->position = p;
}

//creates the fileList structure
fList* createFList(fList* listHead, fList* listTail, char* basePath, char* dir){
    if(checkDuplicateFile(listHead, basePath) == 0) return listHead;
    fList *n = malloc (sizeof(fList));
    n->path = basePath;
    n->directory = dir;
    n->next = NULL;

    listHead = sortfList(listHead, listTail, n);

    return listHead;
}

//sorts alphabetically the fileList structure
fList* sortfList(fList *listHead, fList *listTail, fList *n){

    //If the head is null (means that we are inserting the first element) or the current path ASCII value is greater than the head's one.
    if(listHead == NULL || strcmp(n->path, listHead->path) < 0){ 
            n->next = listHead;
            listHead = n;
    } else { //not the first element and lower ASCII value
        listTail = listHead;
        while((listTail->next != NULL) && (strcmp(n->path, listTail->next->path) >= 0)){
            listTail = listTail->next;
        }
        n->next = listTail->next;
        listTail->next = n;
    }

    return listHead;
}

//sorts alphabetically the lineList structure
llist * sortllist(llist *lHead, llist *lTail, llist *l){

    //If the head is null (means that we are inserting the first element) or the current path ASCII value is greater than the head's one.
    if(lHead == NULL || strcmp(l->line, lHead->line) < 0){   
        l->next = lHead;
        lHead = l;
    } else { //not the first element and lower ASCII value
        lTail = lHead;
        while((lTail->next != NULL) && (strcmp(l->line, lTail->next->line) >= 0)){
            lTail = lTail->next;
        }
        l->next = lTail->next;
        lTail->next = l;
    }

    return lHead;
}

//discards the duplicate in the list of file
int checkDuplicateFile(fList *listHead, const char *line){
    fList *app = listHead;

    while(listHead != NULL){
        if(strcmp(listHead->path, line) == 0) return 0; //we found a duplicate
        listHead = listHead->next;
    }

    listHead = app;
    return 1; //no duplicate
}

//returns 0 if there are duplicate paths, otherwise 1
int checkDuplicatePath(llist *lHead, const char *line){
    llist *app = lHead;

    while(lHead != NULL){
        if(strcmp(lHead->line, line) == 0) return 0; //we found a duplicate
        lHead = lHead->next;
    }

    lHead = app;
    return 1; //no duplicate
}

//creates the lineList structure
llist * createllist(llist *lHead, llist *lTail, const char *inputFile){
    FILE * fInput;
    char *line = NULL;
    size_t len = 0;
    ssize_t line_size;

    fInput = fopen(inputFile, "r"); //the file that contains the path of the file in which search.

    if(fInput == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", inputFile);
    	exit(1);
    }
    
    while((line_size = getline(&line, &len, fInput)) != -1){
        trimTrailing(line); //deletes all the blank spaces in the line
        if(checkDuplicatePath(lHead, line) == 0) continue; //if the path is a duplicated, pass to the next line
        //saving into the lineList structure
        llist *l = malloc (sizeof(llist)); 
        l->line = (char*)malloc((strlen(line)+1)*sizeof(char));
        strcpy(l->line,line);
        l->next = NULL;   

        lHead = sortllist(lHead, lTail, l);
       
    }
    fclose(fInput);
    
    return lHead;
}

//sorts the fileWord structure
fWord * sortfWord(fWord *wordHead, fWord *wordTail, fWord *app){

    //If the head is null (means that we are inserting the first element) or the current path ASCII value is greater than the head's one.
    if(wordHead == NULL || strcmp(app->word, wordHead->word) < 0){
        app->next = wordHead;
        wordHead = app;
    } else { //not the first element and lower ASCII value
        wordTail = wordHead;
        while((wordTail->next != NULL) && (strcmp(app->word, wordTail->next->word) >= 0)){
            wordTail = wordTail->next;
        }
        app->next = wordTail->next;
        wordTail->next = app;
    }

    return wordHead;
}

//returns 0 if there are duplicate words, otherwise 1
int checkDuplicateWord(fWord *wordHead, const char *line){
    fWord *app = wordHead;

    while(wordHead != NULL){
        if(strcmp(wordHead->word, line) == 0) return 0;
        wordHead = wordHead->next;
    }

    wordHead = app;
    return 1;
}

//creates the fileWord structure
fWord * createfWord(fWord *wordHead, fWord *wordTail, const char *wordFile){
    FILE * fw;
    char *line = NULL;
    size_t len = 0;
    ssize_t line_size;

    fw = fopen(wordFile, "r");

    if(fw == NULL){
        fprintf(stderr, "Cannot open %s, exiting. . .\n", wordFile);
    	exit(1);
    }

    while((line_size = getline(&line, &len, fw)) != -1){
        trimTrailing(line); //deletes all the blank spaces in the line
        if(checkDuplicateWord(wordHead, line) == 0) continue; //if the word is duplicated, pass to the next
        fWord *app = malloc(sizeof(fWord));
        app->word = (char*)malloc((strlen(line)+1)*sizeof(char));
        strcpy(app->word, line);
        app->totalOccurences = 0;
        app->p = NULL;
        app->next = NULL;

        wordHead = sortfWord(wordHead, wordTail, app);
    }

    fclose(fw);

    return wordHead;
}

//creates the filePath structure
fPath * createfPath(fPath **app, fPath **pathHead, fPath **pathTail, char *path, char *directory){
    fPath *p = malloc(sizeof(fPath));
    p->fileOccurrences = 0;
    p->path = path;
    p->directory = directory;
    p->next = NULL;

    *app = p;
    if(*pathHead == NULL){
        *pathTail = *pathHead = *app;
    }else{
        *pathTail = (*pathTail)->next = *app;
    }

    return *pathHead;
}

//deletes blank spaces after a given string
void trimTrailing(char * str){ 
    int index, i;

    /* Set default index */
    index = -1;

    /* Find last index of non-white space character */
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index= i;
        }

        i++;
    }

    /* Mark next character to last non-white space character as NULL */
    str[index + 1] = '\0';
}