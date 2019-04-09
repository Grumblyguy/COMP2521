// MMList.c ... implementation of List-of-Mail-Messages ADT
// Written by John Shepherd, Feb 2019

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "MMList.h"
#include "MailMessage.h"

// data structures representing 
typedef struct MMListNode *Link;

typedef struct MMListNode {
	MailMessage data; // message associated with this list item
	Link next;		  // pointer to node containing next element
} MMListNode;

typedef struct MMListRep {
	Link first; // node containing first value
	Link last;  // node containing last value
	Link curr;  // current node (for iteration)
} MMListRep;


static Link newMMListNode (MailMessage mesg);

// create a new empty MMList
MMList newMMList (void)
{
	MMListRep *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate MMList");
	*new = (MMListRep) { };
	return new;
}

// free up memory associated with list
// note: does not remove Mail Messages
void dropMMList (MMList L)
{
	assert (L != NULL);
	Link curr = L->first;
	while (curr != NULL) {
		Link next = curr->next;
		free (curr);
		curr = next;
	}
	free (L);
}

// display list as one integer per line on stdout
void showMMList (MMList L)
{
	assert (L != NULL);
	for (Link curr = L->first; curr != NULL; curr = curr->next)
		showMailMessage (curr->data, 0);
}

// insert mail message in order
// ordering based on MailMessageDateTime
void MMListInsert (MMList L, MailMessage mesg)
{
	assert (L != NULL);
	assert (MMListIsOrdered (L));
    assert (mesg != NULL);

    //Like in one of the earlier labs
    //Adds in the first message if no other ones in list
    struct MMListNode *newnode = newMMListNode(mesg);
    int madechange = 0;

    if (L->first == NULL){ //If list is empty, initiate it with new node
        L->first = newnode;
        L->first->next = NULL;
        L->last = L->first;
        L->curr = L->first;
    }else{ 
        L->curr = L->first;
        //If the new message is most recent
        if(DateTimeBefore(MailMessageDateTime(mesg), MailMessageDateTime(L->curr->data))){ 
            L->first = newnode;
            newnode->next = L->curr;
        }else{
            while(L->curr->next != NULL){
                //If message date is earlier than current date  
                if(DateTimeBefore(MailMessageDateTime(mesg), MailMessageDateTime(L->curr->next->data))){
                    newnode->next = L->curr->next;
                    L->curr->next = newnode;
                    madechange = 1;
                    break;
                }
                // This is if the mail time is the same
                else if(DateTimeBefore(MailMessageDateTime(mesg), MailMessageDateTime(L->curr->next->data)) == 0 && 
                            DateTimeAfter(MailMessageDateTime(mesg), MailMessageDateTime(L->curr->next->data)) == 0){
                    newnode->next = L->curr->next;
                    L->curr->next = newnode;
                    madechange = 1;
                    break;
                }

                L->curr = L->curr->next;
            }
            
            //We are here if L->curr->next == NULL and no insert has been made yet
            if (madechange != 1){ 
                //This statement is only used for the last node   
                if(DateTimeBefore(MailMessageDateTime(mesg), MailMessageDateTime(L->curr->data)) != 1){ 
                    L->curr->next = newnode;
                    newnode->next = NULL;
                    L->last = newnode;
                }
            }
        }
        
    } 

}

// create a new MMListNode for mail message
// (this function is used only within the ADT)
static Link newMMListNode (MailMessage mesg)
{
	Link new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate MMList node");
	*new = (MMListNode) { .data = mesg };
	return new;
}

// check whether a list is ordered (by MailMessageDate)
bool MMListIsOrdered (MMList L)
{
	DateTime prevDate = NULL;
	for (Link n = L->first; n != NULL; n = n->next) {
		DateTime currDate = MailMessageDateTime (n->data);
		if (prevDate != NULL && DateTimeAfter (prevDate, currDate))
			return false;
		prevDate = currDate;
	}
	return true;
}

// start scan of an MMList
void MMListStart (MMList L)
{
	assert (L != NULL);
	L->curr = L->first;
}

// get next item during scan of an MMList
MailMessage MMListNext (MMList L)
{
	assert (L != NULL);
	if (L->curr == NULL)
		// this is probably an error
		return NULL;

	MailMessage mesg = L->curr->data;
	L->curr = L->curr->next;
	return mesg;
}

// check whether MMList scan is complete
bool MMListEnd (MMList L)
{
	assert (L != NULL);
	return (L->curr == NULL);
}
