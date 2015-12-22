#ifndef LOCATION_LIST_H
#define LOCATION_LIST_H

#include <stdlib.h>

typedef struct LNode_ 
{
	char* filepath;
	void* next;
	int occurrences;
}LNode;


struct LocationList 
{
	int numItems;
	LNode* head;
};

typedef struct LocationList* LocationListPtr;

/**
*	Creates a location list pointer and returns it back to the
*	user.
**/
LocationListPtr LLCreate();

/**
*	Walk through the location list and free the node contained
*	within the list
**/
void LLDestroy(LocationListPtr list);

/**
*	swap the contents of two given nodes.
**/
void swap(LNode *a, LNode *b);

/**
*	Sort the contents of the node that are contained within
*	the sorted list of locations based on the occurrences.
**/
void sortList(LNode* start);

/**
*	Inserts a node into the list of location nodes.
**/
int LLInsert(LocationListPtr list, char* data);

#endif
