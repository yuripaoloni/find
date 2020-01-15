#include "../include/find.h"

void runTest(){

    // this function use freopen(), so in order to not interrupt the other tests execution, should be run alone
    // testExecutePrintOutput();
    testExecute();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testExecuteVerbose();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testExecuteExcludeExtension();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testGetWordOccurrences();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testGetFileList();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testKMP();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testCreateLlist();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testCreateFlist();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testCreateFword();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testCreateFpath();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testElapsedTime();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testGetDirectory();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testGetFilenameExt();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testIsRegularFile();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testSwapfPath();
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
    testArgumentsCheck(); //contains exit(1) must be executed in the end
    printf("\n-----------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void testKMP(){
    printf("\nTesting the KMP algorithm...\nPassing abcDabcDabcDabcDabc and searching for abc, we get : ");
    fPath* app = malloc(sizeof(fPath));
    char *line = malloc(sizeof(char)*20); 
    char *word = malloc(sizeof(char)*4); 
    strcpy(line, "abcdabcdabcdabcdabc");
    strcpy(word, "abc");
    int occur = KMP(line, word, strlen(line), strlen(word), 0, app);
    printf("\n%d occurrences\n", occur);
    free(app);
    free(line);
    free(word);
}

void testGetWordOccurrences(){
    printf("\nTesting : find --report test/testOutputFile --show abc --file ./test/testDir/testDir1/testFile6 :\n\n");
    getWordOccurences("abc", "./test/testOutputFile", "./test/testDir/testDir1/testFile6");
}

void testGetFileList(){
    printf("\nTesting : find --report test/testOutputFile --show abc :\n\n");
    getFileList("./test/testOutputFile", "abc", 1);
    printf("\nTesting : find --report test/testOutputFile --show abc 3 :\n\n");
    getFileList("./test/testOutputFile", "abc", 3);
}

void testExecute(){
    printf("\nTesting : find --words test/testInputWord --input test/testInputPath :\n\n");
    execute("./test/testInputWord", "./test/testInputPath", NULL, 0, 0, NULL);
}

void testExecutePrintOutput(){
    printf("\nTesting : find --words test/testInputWord --input test/testInputPath -o  test/testOutputFile :\n\n");
    execute("./test/testInputWord", "./test/testInputPath", NULL, 0, 1, "test/testOutputFile");
}

void testExecuteVerbose(){
    printf("\nTesting : find --words test/testInputWord --input test/testInputPath -v :\n\n");
    execute("./test/testInputWord", "./test/testInputPath", NULL, 1, 0, NULL);
}

void testExecuteExcludeExtension(){
    printf("\nTesting : find --words test/testInputWord --input test/testInputPath -e txt :\n\n");
    execute("./test/testInputWord", "./test/testInputPath", "txt", 0, 0, NULL);
}

void testCreateFlist(){
    printf("\nTesting the function createfList() reading recursively the directory test/testDir :\n\n");
    fList *head = NULL;
    fList *tail = NULL;
    head = createFList(head, tail, "./test/testDir/testDir1/testFile6", "./test/testDir/testDir1");
    head = createFList(head, tail, "./test/testDir/testDir2/testFile7", "./test/testDir/testDir2");
    head = createFList(head, tail, "./test/testDir/testDir2/testDir3/testFile3", "./test/testDir/testDir2/testDir3");
    head = createFList(head, tail, "./test/testDir/testFile", "./test/testDir");
    head = createFList(head, tail, "./test/testDir/testFile1", "./test/testDir");
    head = createFList(head, tail, "./test/testDir/testFile4", "./test/testDir");
    head = createFList(head, tail, "./test/testDir/testFile5.txt", "./test/testDir");

    while(head != NULL){
        printf("path : %s\n", head->path);
        printf("directory : %s\n", head->directory);
        head = head->next;
    }

}

void testCreateLlist(){
    printf("\nTesting the function createllist() using the file testInputPath :\n\n");
    llist *head = NULL;
    llist *tail = NULL;
    head = createllist(head, tail, "./test/testInputPath");

    while(head != NULL){
        printf("line : %s\n", head->line);
        head = head->next;
    }
}

void testCreateFword(){
    printf("\nTesting the function createfword() using the file testInputWord :\n\n");
    fWord *head = NULL;
    fWord *tail = NULL;
    head = createfWord(head, tail, "./test/testInputWord");

    while(head != NULL){
        printf("word: %s\n", head->word);
        head = head->next;
    }
}

void testCreateFpath(){
    printf("\nTesting the function createfPath() reading recursively the directory test/testDir :\n\n");
    fList *head = NULL;
    fList *tail = NULL;
    head = createFList(head, tail, "./test/testDir/testDir1/testFile6", "./test/testDir/testDir1");
    head = createFList(head, tail, "./test/testDir/testDir2/testFile7", "./test/testDir/testDir2");
    head = createFList(head, tail, "./test/testDir/testDir2/testDir3/testFile3", "./test/testDir/testDir2/testDir3");
    head = createFList(head, tail, "./test/testDir/testFile", "./test/testDir");
    head = createFList(head, tail, "./test/testDir/testFile1", "./test/testDir");
    head = createFList(head, tail, "./test/testDir/testFile4", "./test/testDir");
    head = createFList(head, tail, "./test/testDir/testFile5.txt", "./test/testDir");
    fPath *app = NULL;
    fPath *fHead = NULL;
    fPath *fTail = NULL;
    while(head != NULL){
        fHead = createfPath(&app, &fHead, &fTail, head->path, head->directory);
        head = head->next;
    }
    while(fHead != NULL){
        printf("path: %s\n", fHead->path);
        printf("directory: %s\n", fHead->directory);
        printf("occurrences: %d\n", fHead->fileOccurrences);
        fHead = fHead->next;
    }
}

void testArgumentsCheck(){
    printf("\nTesting the function argumentsCheck passing a NULL value :\n\n");

    argumentsCheck(NULL, "Null value detected\n");
    argumentsCheck("check", "Valid arguments\n");
}

void testElapsedTime(){
    printf("\nTesting the functions startTime() and elapsedTime() :\n\n");
    fList *head = NULL;
    fList *tail = NULL;
    head = createFList(head, tail, "./test/testDir/testDir1/testFile6", "./test/testDir/testDir1");
    head = createFList(head, tail, "./test/testDir/testDir2/testFile7", "./test/testDir/testDir2");
    head = createFList(head, tail, "./test/testDir/testDir2/testDir3/testFile3", "./test/testDir/testDir2/testDir3");
    head = createFList(head, tail, "./test/testDir/testFile", "./test/testDir");
    head = createFList(head, tail, "./test/testDir/testFile1", "./test/testDir");
    head = createFList(head, tail, "./test/testDir/testFile4", "./test/testDir");
    head = createFList(head, tail, "./test/testDir/testFile5.txt", "./test/testDir");
    while(head != NULL){
        startTime(&head->directory, head);
        elapsedTime(&head->directory, head);
        head = head->next;
    }
}

void testGetDirectory(){
    printf("\nTesting the function getDirectory() passing this path ./test/testDir/testFile :\n\n");
    printf("returned directory: %s", getDirectory("./test/testDir/testFile"));
}

void testGetFilenameExt(){
    printf("\nTesting the function getFilenameExt() passing this path ./test/testDir/testFile5.txt :\n\n");
    printf("returned extension: %s", getFilenameExt("./test/testDir/testFile5.txt"));
}

void testIsRegularFile(){
    printf("\nTesting the function isRegularFile() passing ./test/testDir/testFile :\n\n");
    if(isRegularFile("./test/testDir/testFile")){
        printf("Is a regular file\n");
    }else{
        printf("Not a regular file\n");
    }
    printf("\nTesting the function isRegularFile() passing ./test/testDir :\n\n");
    if(isRegularFile("./test/testDir")){
        printf("Is a regular file");
    }else{
        printf("Not a regular file");
    }
}

void testSwapfPath(){
    printf("\nTesting the function swapfPath() :\n\n");
    fPath *a = malloc(sizeof(fPath));
    a->directory = "test/A/testDir";
    a->path = "testFileA";
    a->fileOccurrences = 10;
    a->next = NULL;
    a->position = NULL;
    fPath *b = malloc(sizeof(fPath));
    b->directory = "test/B/testDir";
    b->path = "testFileB";
    b->fileOccurrences = 20;
    b->next = NULL;
    b->position = NULL;
    printf("Before swap:\n");
    printf("A dir : %s\t\tB dir : %s\n", a->directory, b->directory);
    printf("A path : %s\t\tB path : %s\n", a->path, b->path);
    printf("A occurrences : %d\t\tB occurrences : %d\n\n", a->fileOccurrences, b->fileOccurrences);
    swapfPath(a, b);
    printf("After swap:\n");
    printf("A dir : %s\t\tB dir : %s\n", a->directory, b->directory);
    printf("A path : %s\t\tB path : %s\n", a->path, b->path);
    printf("A occurrences : %d\t\tB occurrences : %d\n\n", a->fileOccurrences, b->fileOccurrences);

}
