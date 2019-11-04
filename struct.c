#include "struct.h"

// A linked list node 
struct Node { 
	int data; 
	struct Node* next; 
}; 

// This function prints contents of linked list starting from 
// the given node 
void printList(struct Node* n) 
{ 
    while (n != NULL) { 
        printf(" %d ", n->data); 
        n = n->next; 
    } 
} 