#include "kmp.h"

struct printOccurences{
int app[255][255];
};

struct printOccurences fPrint[2];

// Function to implement KMP algorithm
int KMP(const char* X, const char* Y, int m, int n, int line, int index){

	int count = 0;

	// Base Case 1: Y is NULL or empty
//	if (*Y == '\0' || n == 0)
//		printf("Pattern occurs with shift 0 at line %d\r\n", line);

	// Base Case 2: X is NULL or X's length is less than that of Y's
//	if (*X == '\0' || n > m)
//		printf("Pattern not found\r\n");


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
		if (*(X + i) == *(Y + j)){
			if (++j == n){
				//printf("Pattern occurs with shift %d at line %d\n", i - j + 1, line);
				count = count + 1; //conta le occorrenze della parola nella riga in input	
				fPrint[index].app[line][i - j + 1] = 1;
			}
		}
		else if (j > 0) {
			j = next[j];
			i--;	// since i will be incremented in next iteration
		}
	}

	return count;
}

void printPosition(int index){

	int row = sizeof(fPrint[index].app) / sizeof(fPrint[index].app[0]);
	int column = sizeof(fPrint[index].app[0])/sizeof(fPrint[index].app[0][0]);

	for(int i = 0; i<row; i++){
		for(int j = 0; j<column; j++){
			if(fPrint[index].app[i][j] == 1){
				printf("%d %d \r\n", i, j);
			}
		}
	}
	//memset(fPrint[index].app, 0, sizeof(fPrint[index].app));
}

int getTotalOccurences(int index){

	int count = 0;
	int row = sizeof(fPrint[index].app) / sizeof(fPrint[index].app[0]);
	int column = sizeof(fPrint[index].app[0])/sizeof(fPrint[index].app[0][0]);

	for(int i = 0; i<row; i++){
		for(int j = 0; j<column; j++){
			if(fPrint[index].app[i][j] == 1){
				count++;
			}
		}
	}
	return count;
}