#ifndef TOKENIZER_h
#define TOKENIZER_h

#include <stdlib.h>
#include <string.h>

/**
*	Tokenizer type.
**/
struct TokenizerT_
{
	char** tokens;	// char pointer to point to char pointers of valid tokens
	char** invalidTokens; // char pointer to point to char pointers of invalid tokens
	int curChar;	// used for indexing
	int manyTokens;	// amount of tokens contained in Tokenizer
	char* str;	//pointer used for indexing.
	int strSize;
};

typedef struct TokenizerT_ TokenizerT;

/**
*	TKCreate creates a new TokenizerT object for a given token stream
*	(given as a string.)
*
*	TKCreate should copy the arguments so that it is not dependent on
*	them staying immutable after returning. (In the future, this may change
*	to increase efficiency.)
*
*	If the function succeeds, it returns a non-NULL TokenizerT.
*	Else it return NULL.
*
**/
TokenizerT *TKCreate(char* ts);

/**
*	TKDestroy destroys a TokenizerT object. It should free all dynamically
*	allocated memory that is part of the object being destroyed.
*
**/
void TKDestroy(TokenizerT* tk);

/**
*	TKGetNextToken returns the next token from the token steam as a 
*	character string. Space for the returned token should be dynamically
*	allocated. The caller is responsible for freeing the space once it is
*	no longer needed.
*
*	If the function succeeds, it returns a C string (delimited by '\0')
*	containing the token. Else it returns 0.
*
**/
char *TKGetNextToken(TokenizerT* tk);

#endif
