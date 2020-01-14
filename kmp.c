#include "kmp.h"

fPosition *head = NULL;
fPosition *current = NULL;

// Function to implement KMP algorithm
int KMP(const char* X, const char* Y, int m, int n, int line, fPath *app){

	int count = 0;	
	int next[n + 1]; // next[i] stores the index of next best partial match

	for (int i = 0; i < n + 1; i++)
		next[i] = 0;

	for (int i = 1; i < n; i++){
		int j = next[i + 1];

		while (j > 0 && Y[j] != Y[i])
			j = next[j];

		if (j > 0 || Y[j] == Y[i])
			next[i + 1] = j + 1;
	}

	for (int i = 0, j = 0; i < m; i++){
		if(X[i] == Y[j]){
			if (++j == n){
				count = count + 1; //counts the occurrences of the word in the input line

				//adds a new node in the structure
				fPosition *node = malloc (sizeof(fPosition));
        		node->line = line;
        		node->character = i - j + 1;
        		node->next = NULL;

        		if(head == NULL){
            		current = head = node;
        		}else{
            		current = current->next = node;
        		}
				app->position = current;
			}
		}
		else if (j > 0) {
			j = next[j];
			i--;	// since i will be incremented in next iteration
		}
	}

	return count; //returns the number of occurrences
}

//pointer back to the head of list
fPosition * getHead(){ 
	fPosition *app = head;
	head = NULL; 
	return app;
}

//free the memory allocated 
void freeKMP(){ 
	free(head);
	free(current);
}


