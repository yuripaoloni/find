#include "kmp.h"

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
		if (*(X + i) == *(Y + j)){
			if (++j == n){
				//printf("Pattern occurs with shift %d at line %d\n", i - j + 1, line);
				count = count + 1; //conta le occorrenze della parola nella riga in input	
				//fPrint[index].app[line][i - j + 1] = 1;
				app->position[line][i - j + 1] = 1;
			}
		}
		else if (j > 0) {
			j = next[j];
			i--;	// since i will be incremented in next iteration
		}
	}

	return count;
}

void printPosition(fPath *app){

	int row = sizeof(app->position) / sizeof(app->position[0]);
	int column = sizeof(app->position[0])/sizeof(app->position[0][0]);

	for(int i = 0; i<row; i++){
		for(int j = 0; j<column; j++){
			if(app->position[i][j] == 1){
				printf("%d %d \r\n", i, j);
			}
		}
	}
}


// void allocate2dArray(fPath *app, int row, int column){

// 	size_t typicalSizeRow = 64;
// 	size_t typicalSizeColumn = 64;

// 	app->position = (int *)malloc(typicalSizeRow * typicalSizeColumn * sizeof(int));

// 	int rowSize = sizeof(app->position) / sizeof(app->position[0]);
// 	int columnSize = sizeof(app->position[0])/sizeof(app->position[0][0]);

// 	*(app->position + row*columnSize + column) = 1;

// 	if() //size raggiunta
// }
