/*
* tokenizer.c
* Name : David Lambropoulos
* 	 Demetrios Lambropoulos
* Date : Wednesday, February 11, 2015
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include "tokenizer.h"

TokenizerT *TKCreate(char* ts)
{	// Begin TKCreate
	if (ts != NULL)
	{	// Begin if-statement
		struct TokenizerT_ *tk = calloc(1,sizeof(TokenizerT));

		// Assert tk is not NULL
		assert(tk != NULL);

		// Set the amount of tokens for the structure to 0.
		tk->manyTokens = 0;

		// Set the current character pointer in the string to the beginning.
		tk->curChar = 0;
		tk->strSize = 1;

		// Find and set the size of the string to the size of argv[i].
		while ((char)ts[tk->curChar] != '\0')
		{	// Begin While loop
			tk->strSize++;
			tk->curChar++;
		}	// End while loop

		// Allocate space for the char *str
		tk->str = calloc(tk->strSize, sizeof(char *));

		int i;
		for(i = 0; i < tk->strSize; i++)
		{	// Begin for loop
			// Index of str to point to the index of argv
			tk->str[i] = ts[i];
		}	// End for loop
		tk->curChar = 0;
		tk->tokens = calloc(tk->strSize, sizeof(char *));
		tk->invalidTokens = calloc(tk->strSize, sizeof(char *));
		return tk;
	}
	else
	{
		return NULL;
	}
}

void TKDestroy(TokenizerT* tk)
{	// Begin TKDestroy 
	int i;

	// Assert tk is not NULL
	assert(tk != NULL);

	for(i = 0; tk->manyTokens-- > 0; i++)
	{	//Begin for loop
		
		if(tk->tokens[i] != NULL)
		{	// Begin if-statement
			
			// Free the char* stored at tokens[i]
			free(tk->tokens[i]);

		}	// End if-statement
	}	//End for loop

	// Free the memory allocated for str
	free(tk->str);

	// Free the memory allocated for tokens
	free(tk->tokens);

	// Free the memory allocated for tk
	free(tk);
}

char *TKGetNextToken(TokenizerT* tk)
{	// Begin TKGetNextToken
	
	/**
	*	Check that the string is not empty.
	**/
	if(tk->str == '\0')
	{	// Begin if-statement
		return '\0';
	}	// End if-statement
	
	/**
	*	calloc(tk->strSize-tk->curChar, sizeof(char));
	*	This will assign for the first token the amount of space
	*	required to contain the entire string in the case that the first token
	*	the whole input is one token. The next time the function is used it will
	*	allocate the size of the string minus the amount of characters already
	*	surpassed.
	**/
	char* arr = calloc(tk->strSize-tk->curChar, sizeof(char));
	int i = 0;	// Current index in arr[i]
	
	while((char)tk->str[tk->curChar] != '\0')
	{	//Begin while-loop
		
		/**
		*	This if-statement will check if tk->str[tk->curChar] is
		*	a whitespace character. If a whitespace character is found
		*	the current character which contains the whitespace 
		*	will be incremented over (e.g. so that the
		*	next time the method is called it will start after the 
		*	whitespace character.)
		**/
	
		if((char)tk->str[tk->curChar] == ' ' || (char)tk->str[tk->curChar] == '\t' || (char)tk->str[tk->curChar] == '\n' || (char)tk->str[tk->curChar] == '\v' || (char)tk->str[tk->curChar] == '\f')
		{	//	Begin if-statement
			tk->curChar++;
			if((char)tk->str[tk->curChar] == '\0')
			{
				arr[i] = ' ';
				break;
			}
		}	// End if-statement
		else if(isalpha(tk->str[tk->curChar]))
		{	// Begin else-if-statement
			arr[i] = tolower(tk->str[tk->curChar]);	// Store current str[index] in arr[i]
			i++;
			tk->curChar++;

			/**
			*	Continue tokenizing word while next character is an alphanumeric
			*	character.
			**/
			while(isalnum(tk->str[tk->curChar]) && (char)tk->str[tk->curChar] != '\0')
			{	// Begin while loop
				arr[i] = tolower(tk->str[tk->curChar]);
				i++;
				tk->curChar++;
				if(i > 200)
				{
					break;
				}
			}	// End while loop
			break;
		}	// End else-if-statement
		else
		{
				i++;
				tk->curChar++;
				break;

		}
	}
	arr[i] = '\0';
	
	return arr;
}	// End TKGetNextToken
