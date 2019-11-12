#include "kmp.h"

fPosition *head = NULL;
fPosition *current = NULL;

// Function to implement KMP algorithm
int KMP(const char* X, const char* Y, int m, int n, int line, fPath *app){

	int count = 0;

	// next[i] stores the index of next best partial match
	int next[n + 1];

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
				count = count + 1; //conta le occorrenze della parola nella riga in input	
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

	return count;
}

fPosition * getHead(){ //rimette il puntatore alla testa della lista
	fPosition *app = head;
	head = NULL;
	return app;
}


