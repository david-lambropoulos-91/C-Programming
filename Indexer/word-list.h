#ifndef WORD_LIST_H
#define WORD_LIST_H

#include <stdlib.h>
#include "location-list.h"

typedef struct WNode_ 
{
	char* data;
	void* next;
	LocationListPtr location;
}WNode;


struct WordList 
{
	int numItems;
	WNode* head;
};

typedef struct WordList* WordListPtr;

WordListPtr WLCreate();

/**
*	Walk through a list and destroy the nodes and contents.
**/
void WLDestroy(WordListPtr list);

/**
*	Walks through a list printing out the contents of the 
*	words and the location in which the words are located
*	in the files in a JSON format to a file provided by 
*	the user.
**/
void PrintJSON(WordListPtr list, char *file);

/**
*	Walks through entering words in the appropriate location 
*	in the word list.
**/
int WLInsert(WordListPtr list, char* data, char* location);

#endif
