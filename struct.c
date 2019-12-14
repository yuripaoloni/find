#include "struct.h"

fList* createFList(fList* listHead, fList* listTail, char* basePath, char* dir){
    fList *n = malloc (sizeof(fList));
    n->path = basePath;
    n->directory = dir;
    n->next = NULL;

    listHead = sortfList(listHead, listTail, n);

    return listHead;
}

fList* sortfList(fList *listHead, fList *listTail, fList *n){

    if(listHead == NULL || strcmp(n->path, listHead->path) < 0){ //sorting alphabetically and creating the structure
            n->next = listHead;
            listHead = n;
    } else {
        listTail = listHead;
        while((listTail->next != NULL) && (strcmp(n->path, listTail->next->path) >= 0)){
            listTail = listTail->next;
        }
        n->next = listTail->next;
        listTail->next = n;
    }

    return listHead;
}

llist * sortllist(llist *lHead, llist *lTail, llist *l){

    if(lHead == NULL || strcmp(l->line, lHead->line) < 0){   
        l->next = lHead;
        lHead = l;
    } else {
        lTail = lHead;
        while((lTail->next != NULL) && (strcmp(l->line, lTail->next->line) >= 0)){
            lTail = lTail->next;
        }
        l->next = lTail->next;
        lTail->next = l;
    }

    return lHead;
}

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
        trimTrailing(line); 

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

fWord * sortfWord(fWord *wordHead, fWord *wordTail, fWord *app){
    if(wordHead == NULL || strcmp(app->word, wordHead->word) < 0){
        app->next = wordHead;
        wordHead = app;
    } else {
        wordTail = wordHead;
        while((wordTail->next != NULL) && (strcmp(app->word, wordTail->next->word) >= 0)){
            wordTail = wordTail->next;
        }
        app->next = wordTail->next;
        wordTail->next = app;
    }

    return wordHead;
}

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
        trimTrailing(line); 
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

void trimTrailing(char * str){ //delete blank spaces after a given string
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