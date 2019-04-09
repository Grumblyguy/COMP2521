// ThreadTree.c ... implementation of Tree-of-Mail-Threads ADT
// Written by John Shepherd, Feb 2019

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "MMList.h"
#include "MMTree.h"
#include "MailMessage.h"
#include "ThreadTree.h"

// Representation of ThreadTree's

typedef struct ThreadTreeNode *Link;

typedef struct ThreadTreeNode {
	MailMessage mesg;
	Link next, replies;
} ThreadTreeNode;

typedef struct ThreadTreeRep {
	Link messages;
} ThreadTreeRep;

// Auxiliary data structures and functions

// Add any new data structures and functions here ...

static void doDropThreadTree (Link t);
static void doShowThreadTree (Link t, int level);
static void insertNext(ThreadTree n, Link insert);
static Link newThreadTreeNode (MailMessage m);
static int replies(Link curr, MMList mesgs, ThreadTree t);
// END auxiliary data structures and functions


// create a new empty ThreadTree
ThreadTree newThreadTree (void)
{
	ThreadTreeRep *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate ThreadTree");
	*new = (ThreadTreeRep) { };
	return new;
}

void dropThreadTree (ThreadTree tt)
{
	assert (tt != NULL);
	doDropThreadTree (tt->messages);
}

// free up memory associated with list
static void doDropThreadTree (Link t)
{
	if (t == NULL)
		return;

	for (Link curr = t, next; curr != NULL; curr = next) {
		next = curr->next;
		doDropThreadTree (curr->replies);
		// don't drop curr->mesg, in case referenced elsehwere
		free (curr);
	}
}

void showThreadTree (ThreadTree tt)
{
	assert (tt != NULL);
	doShowThreadTree (tt->messages, 0);
}

// display thread tree as hiearchical list
static void doShowThreadTree (Link t, int level)
{
	if (t == NULL)
		return;
	for (Link curr = t; curr != NULL; curr = curr->next) {
		showMailMessage (curr->mesg, level);
		doShowThreadTree (curr->replies, level + 1);
	}
}

// insert mail message into ThreadTree
// if a reply, insert in appropriate replies list
// whichever list inserted, must be in timestamp-order

static Link newThreadTreeNode (MailMessage m){

    Link new = malloc(sizeof(ThreadTreeNode)); 
    new->next = NULL;
    new->replies = NULL;
    new->mesg = m;
    return new;
}

static void insertNext(ThreadTree n, Link insert){
    
    if (n->messages == NULL){
        n->messages = insert;
    }
    else{
        Link curr = n->messages;
        while(curr->next != NULL){
            curr = curr->next;
        }
        curr->next = insert;   
    } 
    return;
}



ThreadTree ThreadTreeBuild (MMList mesgs, MMTree msgids)
{   
    if (mesgs == NULL){
        return NULL;
    }
    assert(msgids != NULL);
    ThreadTreeRep *new = newThreadTree();
    MailMessage newMM;  
    MMListStart(mesgs);
    ThreadTreeNode *newNode;
    //Create thread tree of messages that arent replies
    while((newMM = MMListNext(mesgs)) != NULL){
        if (MailMessageRepliesTo(newMM) == NULL){
            newNode = newThreadTreeNode(newMM);
            insertNext(new, newNode);        
        }   
    } 
    //At this point the tree consists of only messages that arent replying to anything
    //Now to build the rest of the tree

    Link current = new->messages;
    //This will iterate through the nodes that dont reply to anything, then call the recursive function replies
    //To add the rest of the tree
    while(current != NULL){
        replies(current, mesgs, new); 
        current = current->next;
    }

    return new;

}

//This recursive function will go to every node until there are no replies or nexts, then returns -1
//At every node, it will check for a reply and next, and call itself again depending on which branch to go down

static int replies(Link curr, MMList mesgs, ThreadTree t){
    MailMessage newMM;
    MMListStart(mesgs); 
    //nreplies and nnext will be 1 if there is a reply or next on a specific node
    int nreplies = 0;
    int nnext = 0;
    //This will find the newest reply to this message
    while((newMM = MMListNext(mesgs)) != NULL){
        if (MailMessageRepliesTo(newMM) != NULL && strcmp(MailMessageID(curr->mesg), MailMessageRepliesTo(newMM)) == 0 &&
            strcmp(MailMessageID(curr->mesg), MailMessageID(newMM)) != 0){
            //Only counts a reply if the new message is newer than the current 
            if(DateTimeBefore(MailMessageDateTime(curr->mesg), MailMessageDateTime(newMM))){
                nreplies++;
                break;
            }
        }
    }

    MMListStart(mesgs);
    MailMessage newMM2;
    //This will find the newest next to the message
    while((newMM2 = MMListNext(mesgs)) != NULL){
        if ((MailMessageRepliesTo(newMM2) != NULL && MailMessageRepliesTo(curr->mesg) != NULL) && 
            (strcmp(MailMessageRepliesTo(curr->mesg), MailMessageRepliesTo(newMM2)) == 0) && (strcmp(MailMessageID(curr->mesg), MailMessageID(newMM2)) != 0)){
            //Only counts a next if the new message is newer than the current
            if(DateTimeBefore(MailMessageDateTime(curr->mesg), MailMessageDateTime(newMM2))){
                nnext++;
                break;
            }
        }
    }
    //The recursive part, adds the reply and/or next, then passes a recursion in to that node
    if(nreplies != 0){
        curr->replies = newThreadTreeNode(newMM);
        replies(curr->replies, mesgs, t);   
    }

    if(nnext != 0){
        curr->next = newThreadTreeNode(newMM2);        
        replies(curr->next, mesgs, t);
    }
   
    return -1;

}



























