#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "location-list.h" 
/**
 * Creates and returns a Location List.
 */ 
LocationListPtr LLCreate() 
{	
	// Allocate memory for the Location List
	LocationListPtr ll = calloc(1,sizeof(struct LocationList *));

	// Return Location List
	return ll;
}

/**
 * Destroy Location List
 */
void destroyLocationList(LNode* head)
{	
	// Check if there are more than one node in the location list.
	if(head->next == 0) 
	{
		// Free the memory allocated for the head of the location list.
		free(head);
		return;
	}
	
	// Recursively call itself til all memory is freed
	destroyLocationList(head->next);

	// Free the memory allocated for the given node
	free(head);
}

/**
 * LLDestroy will destroy the location list pointer
 */
void LLDestroy(LocationListPtr ll) 
{
	// Recursively destroy the Location List
	destroyLocationList(ll->head);
	
	// Free memory allocated for the location list pointer.
	free(ll);
} 


/* function to swap data of two nodes a and b*/
void swap(LNode *a, LNode *b)
{
	int temp = a->occurrences;
	char* tempFile = a->filepath;
	a->occurrences = b->occurrences;
	a->filepath = b->filepath;
	b->occurrences = temp;
	b->filepath = tempFile;
}
/* Bubble sort the given linked lsit */
void sortList(LNode* start)
{
	int swapped;
	LNode *ptr1 = start, *next;
	LNode *lptr = NULL;
 	
    	/* Checking for empty list */
    	if (ptr1 == NULL)
        	return;
 
    	do
    	{
        	swapped = 0;
        	ptr1 = start;
 
        	while (ptr1->next != lptr)
		{
			next = ptr1->next;
			if (ptr1->occurrences > next->occurrences)
			{
				swap(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	}while (swapped);
}

/**
 * Insert item in location list.
 */
int LLInsert(LocationListPtr ll, char* location) 
{ 

	LNode *nextNode, *index, *previous;
	//nextNode = calloc(1, sizeof(LNode));

	//printf("Adding location %s to:", location);
	if(ll->head == 0) 
	{ 		
		nextNode = calloc(1,sizeof(LNode*));
		nextNode->next = 0;
		nextNode->filepath = location;
		ll->numItems++;
		nextNode->occurrences=1;
		ll->head = nextNode;
		return 1;
	}
	if(strcmp((ll->head)->filepath,location) == 0)
	{
		(ll->head)->occurrences++;
		return 1;
	}
	
	if(ll->numItems == 1)
	{
		nextNode = calloc(1,sizeof(LNode*));
		nextNode->next = 0;
		nextNode->filepath = location;
		ll->numItems++;
		nextNode->occurrences=1;
		(ll->head)->next = nextNode;
		return 1;
	}

	index = (ll->head)->next;
	previous = ll->head;
	
	while(1) 
	{
		if(index == 0) 
		{ /* add to the tail */
			nextNode = calloc(1,sizeof(LNode*));
			nextNode->next = 0;
			nextNode->filepath = location;
			ll->numItems++;
			nextNode->occurrences=1;
			previous->next = nextNode;
			sortList(ll->head);
			return 1;
		}
		
		if(strcmp(index->filepath, location) == 0)
		{
			index->occurrences++;
			sortList(ll->head);
			return 1;
		}
		

		previous=index;
		index=index->next;

	}
	

	return 1;
}
