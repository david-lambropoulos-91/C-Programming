#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word-list.h"
#include "location-list.c"
#include "alerts.h" 
WordListPtr WLCreate() 
{  
	WordListPtr wl = calloc(1,20*sizeof(struct WordList));
	return wl;
}

 
void destroyWordList(WNode* head)
{
	destroyLocationList((head->location)->head);
	if(head->next == 0) 
	{
		free(head);
		return;
	}
	destroyWordList(head->next);
	free(head);
}

void WLDestroy(WordListPtr wl) 
{
	destroyWordList(wl->head);
	free(wl);
}

int WLInsert(WordListPtr wl, char* word, char* location) 
{
	WNode *nextNode, *index, *previous;
	//printf("\n%s | %s\n", word, location);
	//printf("Word inside word-list: %s\n", word);
	//printf("Location inside word-list: %s\n", location);
	// Check whether the list is empty
	if(wl->head == 0) 
	{	
		nextNode = calloc(1,sizeof(WNode));
		nextNode->next = 0;
		nextNode->data = word;
		wl->numItems++;
		nextNode->location = LLCreate();
		LLInsert(nextNode->location, location);
		//printf("%s\n", word);
		wl->head = nextNode;
		return 1;
	}
	
	// Check whether the data stored in the head 
	// is greater than or appearing lexicographically
	// after word. If so, swap the position.
	if(strcmp((wl->head)->data, word) > 0) 
	{ 
		nextNode = calloc(1,sizeof(WNode));
		nextNode->next = 0;
		nextNode->data = word;
		wl->numItems++;
		nextNode->location = LLCreate();
		LLInsert(nextNode->location, location);
		//printf("%s\n", word);
		nextNode->next = wl->head;
		wl->head = nextNode;
		return 1;
	}

	

	// Check whether the word is equal to the current 
	// data stored in the head. If so, push the location
	// onto the Location Sorted List.
	if(strcmp((wl->head)->data, word) == 0)
	{
		LLInsert((wl->head)->location, location);
		return 1;
	}

	if(wl->numItems==1) 
	{
		nextNode = calloc(1,sizeof(WNode));
		nextNode->next = 0;
		nextNode->data = word;
		wl->numItems++;
		nextNode->location = LLCreate();
		LLInsert(nextNode->location, location);
		//printf("%s\n", word);
		(wl->head)->next = nextNode;
		return 1;
	}
	
	index = (wl->head)->next;  
	previous = wl->head;
 
	while(1) 
	{
		if(index == 0) 
		{ /* add to the tail */
			nextNode = calloc(1,sizeof(WNode));
			nextNode->next = 0;
			nextNode->data = word;
			wl->numItems++;
			nextNode->location = LLCreate();
			LLInsert(nextNode->location, location);
			//printf("%s\n", word);
			previous->next = nextNode;
			return 1;
		}   
	
		if(strcmp(index->data,word) > 0) 
		{	
			nextNode = calloc(1,sizeof(WNode));
			nextNode->next = 0;
			nextNode->data = word;
			wl->numItems++;
			nextNode->location = LLCreate();
			LLInsert(nextNode->location, location);
			//printf("%s\n", word);
			previous->next = nextNode;
			nextNode->next = index;
			return 1;
		} 
		else if(strcmp(index->data, word) == 0)
		{	
			LLInsert(index->location, location);
			//printf("%s\n", word);
			return 1;
		}
		

		previous=index;
		index=index->next;

	}
	

	return 1;
}

void PrintJSON(WordListPtr list, char *file)
{
	if (list == NULL)
	{
		error("List is NULL");
		return;
	}
	if (list->head == NULL)
	{
		warning("Nothing to print");
		return;
	}
	FILE *f = fopen(file, "wr");

	if (f == NULL)
	{
		error("File could not be opened.");
		return;
	}
	fprintf(f,"{\"list\" : [\n");
	WNode * i = list->head;
		
	for(;i != 0; i = i->next)
	{
		fprintf(f,"\t{\"%s\" : [\n", i->data);
		LNode * j = i->location->head;
		for(; j != 0; j = j->next)
		{
			fprintf(f,"\t\t{\"%s\" : %d}\n", j->filepath, j->occurrences);
		}
		fprintf(f,"\t]}\n");
	}
	fprintf(f,"]}\n");	
}
