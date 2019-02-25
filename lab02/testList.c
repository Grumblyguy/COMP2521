// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "DLList.h"

typedef struct DLListNode {
	char   *value;  // value of this list item (string)
	struct DLListNode *prev;
	               // pointer previous node in list
	struct DLListNode *next;
	               // pointer to next node in list
} DLListNode;

typedef struct DLListRep {
	int  nitems;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *curr;  // current node in list
	DLListNode *last;  // last node in list
} DLListRep;

void printstate(DLList L);
void DLListAfterTests(DLList L);
int main(int argc, char *argv[])
{

    /*DLList myList;
	myList = getDLList(stdin);
	putDLList(stdout,myList);
	assert(validDLList(myList));
    */
    
    DLListRep *L = newDLList();

   
    //Tests for the DLListAfter function
    printf("DLListAfter tests\n");
    printf("------------------------------------------------------------------\n");
    DLListAfterTests(L);
    printf("------------------------------------------------------------------\n");

    // TODO: more tests needed here
	return 0;
}

void printstate(DLList L){

    printf("-----------------------------------------------\n");
    printf("Number of items: %d\n", L->nitems);
    putDLList(stdout, L);
    printf("-----------------------------------------------\n");

}

void DLListAfterTests(DLList L){
    printf("This tests the function's handling of an empty list\n");
    printf("Current state: \n");
    printstate(L); 
    printf("Running DLListafter with input 'hello'\n");
    DLListAfter(L, "hello");
    assert(L->nitems == 1);
    assert(strcmp("hello", L->curr->value) == 0);
    assert(L->first == L->last);
    assert(L->first == L->curr);
    assert(validDLList(L));
    printf("New state: \n");
    printstate(L);
    printf("\n \n");

    printf("This tests the function's handling of a list with one node\n");
    printf("Current state: \n");
    printstate(L);
    printf("Running DLListafter with input 'my name is'\n");
    DLListAfter(L, "my name is");
    assert(L->nitems == 2);
    assert(strcmp("my name is", L->curr->value) == 0);
    assert(L->first == L->curr->prev);
    assert(validDLList(L));
    printf("New state: \n");
    printstate(L);
    printf("\n \n");
    
    printf("This tests the function's handling of a list with more than one node\n");
    printf("Current state: \n");
    printstate(L);
    printf("Running DLListafter with input 'jas'\n");
    DLListAfter(L, "jas");
    assert(L->nitems == 3);
    assert(strcmp("jas", L->curr->value) == 0);
    assert(L->last == L->curr);
    assert(validDLList(L));

    printf("New state: \n");
    printstate(L);
    
}
