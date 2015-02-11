/*
* tokenizer.c
* Name : Demetrios Lambropoulos
* 		   David Lambropoulos
* Date : Tuesday, February 10, 2015
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
/*
* Tokenizer type. You need to fill in the type as part of your implementation.
*/
struct TokenizerT_
{
	char *str; // char pointer to point to pointer at argv[num] - Jan 31 22:35
	int curChar; // current string index - Jan 31 22:35
	int manyTokens; // amount of tokens contained in argv[num] - Jan 31 22:35
	char **tokens;
	char **tokenDesc;
	int strSize;
};

typedef struct TokenizerT_ TokenizerT;

/*
* TKCreate creates a new TokenizerT object for a given token stream
* (given as a string).
*
* TKCreate should copy the arguments so that it is not dependent on
* them staying immutable after returning. (In the future, this may change
* to increase efficiency.)
*
* If the function succeeds, it returns a non-NULL TokenizerT.
* Else it returns NULL.
*
* You need to fill in this function as part of your implementation.
*/
TokenizerT *TKCreate( char *ts )
{
	if (ts != NULL)
	{
		//struct TokenizerT_ *tk = malloc(sizeof(struct TokenizerT_));
		struct TokenizerT_ *tk = calloc(1,sizeof(TokenizerT));
		assert(tk != NULL);
		tk->manyTokens = 0;
		tk->curChar = 0;
		tk->strSize = 1;
		while ((char)ts[tk->curChar] != '\0')
		{
			tk->strSize++;
			tk->curChar++;
		}
	tk->str = calloc(tk->strSize, sizeof(char *));
	int i;
	for(i = 0; i < tk->strSize; i++)
	{
		tk->str[i] = ts[i];
	}
	tk->curChar = 0;
	tk->tokens = calloc(tk->strSize, sizeof(char *));
	tk->tokenDesc = calloc(tk->strSize, sizeof(char *));
	return tk;
	}
	else
	{
		return NULL;
	}
}

/*
* TKDestroy destroys a TokenizerT object. It should free all dynamically
* allocated memory that is part of the object being destroyed.
*
* You need to fill in this function as part of your implementation.
*/
void TKDestroy( TokenizerT * tk )
{
	int i;
	assert(tk != NULL);
	for(i = 0; tk->manyTokens-- > 0; i++)
	{
		if(tk->tokens[i]!=NULL && tk->tokenDesc[i]!=NULL)
		{
			free(tk->tokens[i]);
		}
	}
	free(tk->str);
	free(tk->tokens);
	free(tk->tokenDesc);
	free(tk);
}

/*
* TKGetNextToken returns the next token from the token stream as a
* character string. Space for the returned token should be dynamically
* allocated. The caller is responsible for freeing the space once it is
* no longer needed.
*
* If the function succeeds, it returns a C string (delimited by '\0')
* containing the token. Else it returns 0.
*
* You need to fill in this function as part of your implementation.
*/

char *TKGetNextToken( TokenizerT * tk )
{ // Begin TKGetNextToken
	/**
	* Check that the string is not empty.
	*/
	if(tk->str == '\0')
	{ // Begin if-statement
		return '\0';
	} // End if-statement

	/*
	* calloc(tk->strSize-tk->curChar, sizeof(char));
	* This will assign for the first token the amount of space
	* required to contain the entire string in the case that the first token
	* the whole input is one token. The next time the function is used it will
	* allocate the size of the string minus the amount of characters already
	* surpassed.
	*/
	char *arr = calloc(tk->strSize-tk->curChar, sizeof(char));
	int i = 0; // Current index in arr[i]
	while ((char)tk->str[tk->curChar] != '\0')
	{ // Begin while-loop
	/**
	* This if-statement will check if tk->str[tk->curChar] is
	* a whitespace character. If a whitespace character is found
	* the current character which contains the whitespace
	* will be incremented over (e.g. tk->curChar++) so that the
	* next time the method is called it will start after the
	* whitespace character.
	*/
	if((char)tk->str[tk->curChar] == ' ')
	{ // Begin if-statement
		tk->curChar++;
		if((char)tk->str[tk->curChar] == '\0')
		{
			arr[i] = ' ';
			break;
		}
	} // End if-statement
	else if(isalpha(tk->str[tk->curChar]))
	{ // Begin if-statement
		arr[i] = tk->str[tk->curChar]; // Store current str[index] in arr[i]
		i++;
		tk->curChar++;
		/**
		* Continue tokenizing word while next character is an alphanumeric
		* character.
		*/
		while(isalnum(tk->str[tk->curChar]) && (char)tk->str[tk->curChar] != '\0')
		{ // Begin while loop
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->curChar++;
		} //
		tk->tokenDesc[tk->manyTokens] = "word";
		break;
	}
	else if((char)tk->str[tk->curChar] == '.')
	{

		tk->tokenDesc[tk->manyTokens] = "float";
		arr[i] = tk->str[tk->curChar];
		i++;
		tk->curChar++;
		while(isdigit(tk->str[tk->curChar]))
		{
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->curChar++;
			if((char)tk->str[tk->curChar] == 'e')
			{
				arr[i] = tk->str[tk->curChar];
				i++;
				tk->curChar++;
				if((char)tk->str[tk->curChar] == '-' || (char)tk->str[tk->curChar] == '+')
				{
					arr[i] = tk->str[tk->curChar];
					i++;
					tk->curChar++;
					if(!isdigit(tk->str[tk->curChar]))
					{
						tk->tokenDesc[tk->manyTokens] = "improper float";
						break;
					}
					while(isdigit(tk->str[tk->curChar]))
					{
						arr[i] = tk->str[tk->curChar];
						i++;
						tk->curChar++;
					}
					tk->tokenDesc[tk->manyTokens] = "scientific float";
					break;
				}
				else
				{
					tk->tokenDesc[tk->manyTokens] = "improper float";
					break;
				}
			}
		}
		break;
	}
	else if(isdigit(tk->str[tk->curChar]))
	{
		if((char)tk->str[tk->curChar] == '0')
		{
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->curChar++;

			if((char)tk->str[tk->curChar] == 'x' || (char)tk->str[tk->curChar] == 'X')
			{
				arr[i] = tk->str[tk->curChar];
				i++;
				tk->curChar++;

				while(((char)tk->str[tk->curChar] >= '0' && (char)tk->str[tk->curChar] <= '9') || ((char)tk->str[tk->curChar] >= 'a' && (char)tk->str[tk->curChar] <= 'f') || ((char)tk->str[tk->curChar] >= 'A' && (char)tk->str[tk->curChar] <= 'F'))
				{
					arr[i] = tk->str[tk->curChar];
					i++;
					tk->curChar++;
				}
				tk->tokenDesc[tk->manyTokens] = "Hexadecimal Number";
				break;
			}
			else if((char)tk->str[tk->curChar] >= '0' && (char)tk->str[tk->curChar] < '8')
			{
				arr[i] = tk->str[tk->curChar];
				i++;
				tk->curChar++;
				while((char)tk->str[tk->curChar] >= '0' && (char)tk->str[tk->curChar] < '8')
				{
					arr[i] = tk->str[tk->curChar];
					i++;
					tk->curChar++;
				}
				tk->tokenDesc[tk->manyTokens] = "Octal Number";
				break;
			}
			else if((char)tk->str[tk->curChar] == '.')
			{
				tk->tokenDesc[tk->manyTokens] = "float";
				arr[i] = tk->str[tk->curChar];
				i++;
				tk->curChar++;
				while(isdigit(tk->str[tk->curChar]))
				{
					arr[i] = tk->str[tk->curChar];
					i++;
					tk->curChar++;
					if((char)tk->str[tk->curChar] == 'e')
					{
						arr[i] = tk->str[tk->curChar];
						i++;
						tk->curChar++;
						if((char)tk->str[tk->curChar] == '-' || (char)tk->str[tk->curChar] == '+')
						{
							arr[i] = tk->str[tk->curChar];
							i++;
							tk->curChar++;
							if(!isdigit(tk->str[tk->curChar]))
							{
								tk->tokenDesc[tk->manyTokens] = "improper float";
								break;
							}
							while(isdigit(tk->str[tk->curChar]))
							{
								arr[i] = tk->str[tk->curChar];
								i++;
								tk->curChar++;
							}
							tk->tokenDesc[tk->manyTokens] = "scientific float";
							break;
						}
						else
						{
							tk->tokenDesc[tk->manyTokens] = "improper float";
							break;
						}
					}
				}
				break;
			}
			break;
		}
		else
		{
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->curChar++;

			while(isdigit(tk->str[tk->curChar]) && (char)tk->str[tk->curChar] != '\0')
			{
				arr[i] = tk->str[tk->curChar];
				i++;
				tk->curChar++;
			}
			if((char)tk->str[tk->curChar] == 'L')
			{
				arr[i] = tk->str[tk->curChar];
				tk->tokenDesc[tk->manyTokens] = "long decimal number";
				tk->curChar++;
				break;
			}
			else if((char)tk->str[tk->curChar] == '.')
			{
				tk->tokenDesc[tk->manyTokens] = "float";
				arr[i] = tk->str[tk->curChar];
				i++;
				tk->curChar++;
				while(isdigit(tk->str[tk->curChar]))
				{
					arr[i] = tk->str[tk->curChar];
					i++;
					tk->curChar++;
					if((char)tk->str[tk->curChar] == 'e')
					{
						arr[i] = tk->str[tk->curChar];
						i++;
						tk->curChar++;
						if((char)tk->str[tk->curChar] == '-' || (char)tk->str[tk->curChar] == '+')
						{
							arr[i] = tk->str[tk->curChar];
							i++;
							tk->curChar++;
							if(!isdigit(tk->str[tk->curChar]))
							{
								tk->tokenDesc[tk->manyTokens] = "improper float";
								break;
							}
							while(isdigit(tk->str[tk->curChar]))
							{
								arr[i] = tk->str[tk->curChar];
								i++;
								tk->curChar++;
							}
							tk->tokenDesc[tk->manyTokens] = "scientific float";
							break;
						}
						else
						{
							tk->tokenDesc[tk->manyTokens] = "improper float";
							break;
						}
					}
				}
				break;
			}
			tk->tokenDesc[tk->manyTokens] = "decimal number";
			tk->curChar++;
			break;
		}
	}
	else if((char)tk->str[tk->curChar] == '[')
	{
		arr[i] = tk->str[tk->curChar];
		tk->tokenDesc[tk->manyTokens] = "left brace";
		tk->curChar++;
		break;
	}
	else if((char)tk->str[tk->curChar] == ']')
	{
		arr[i] = tk->str[tk->curChar];
		tk->tokenDesc[tk->manyTokens] = "right brace";
		tk->curChar++;
		break;
	}
	else if((char)tk->str[tk->curChar] == '(')
	{
		arr[i] = tk->str[tk->curChar];
		tk->tokenDesc[tk->manyTokens] = "left parentheses";
		tk->curChar++;
		break;
	}
	else if((char)tk->str[tk->curChar] == ')')
	{
		arr[i] = tk->str[tk->curChar];
		tk->tokenDesc[tk->manyTokens] = "right parentheses";
		tk->curChar++;
		break;
	}
	else if((char)tk->str[tk->curChar] == '+')
	{
		arr[i] = tk->str[tk->curChar];
		i++;
		tk->tokenDesc[tk->manyTokens] = "plus";
		tk->curChar++;
		if((char)tk->str[tk->curChar] == '=')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "plusequals";
			tk->curChar++;
		}
		else if((char)tk->str[tk->curChar] == '+')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "increment";
			tk->curChar++;
		}
		break;
	}
	else if((char)tk->str[tk->curChar] == '-')
	{
		arr[i] = tk->str[tk->curChar];
		i++;
		tk->tokenDesc[tk->manyTokens] = "minus";
		tk->curChar++;
		if((char)tk->str[tk->curChar] == '=')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "minusequals";
			tk->curChar++;
		}
		else if((char)tk->str[tk->curChar] == '-')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "decrement";
			tk->curChar++;
		}
		break;
	}
	else if((char)tk->str[tk->curChar] == '=')
	{
		arr[i] = tk->str[tk->curChar];
		i++;
		tk->tokenDesc[tk->manyTokens] = "equals sign";
		tk->curChar++;
		if((char)tk->str[tk->curChar] == '=')
		{
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->tokenDesc[tk->manyTokens] = "Relational Operator: equals to";
			tk->curChar++;
		}
		break;
	}
	else if((char)tk->str[tk->curChar] == '*')
	{
		arr[i] = tk->str[tk->curChar];
		i++;
		tk->tokenDesc[tk->manyTokens] = "Binary Operator: multiply";
		tk->curChar++;
		if((char)tk->str[tk->curChar] == '=')
		{
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->tokenDesc[tk->manyTokens] = "Assignment Operator: timesEquals";
			tk->curChar++;
		}
		break;
	}
	else if((char)tk->str[tk->curChar] == '/')
	{
       	tk->curChar++;
       	if((char)tk->str[tk->curChar] == '/')
       	{
       		while((char)tk->str[tk->curChar] != '\n')
       		{
       			if((char)tk->str[tk->curChar] == '\0')
       			{
       				break;
       			}
       			if((char)tk->str[tk->curChar] == '\\')
       			{
       					if((char)tk->str[tk->curChar+1] == 'n')
       					{
       						tk->curChar+=2;
       						break;
       					}
       			}
       			tk->curChar++;
       		}
       		break;
		}
       	else
       	{
       		arr[i] = tk->str[tk->curChar - 1];
       		arr[i + 1] = tk->str[tk->curChar];
       		i+=2;
       		tk->tokenDesc[tk->manyTokens] = "Binary Operator: divide";
       		tk->curChar++;
       	}
        if((char)tk->str[tk->curChar] == '=')
        {
            arr[i] = tk->str[tk->curChar];
            i++;
            tk->tokenDesc[tk->manyTokens] = "Assignment Operator: divideEquals";
			tk->curChar++;
        }
        break;
    }
	else if((char)tk->str[tk->curChar] == '%')
	{
		arr[i] = tk->str[tk->curChar];
		i++;
		tk->tokenDesc[tk->manyTokens] = "Binary Operator: modulus";
		tk->curChar++;
		if((char)tk->str[tk->curChar] == '=')
		{
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->tokenDesc[tk->manyTokens] = "Assignment Operator: modulousEquals";
			tk->curChar++;
		}
		break;
	}
	else if((char)tk->str[tk->curChar] == '<')
	{
		arr[i] = tk->str[tk->curChar];
		i++;
		tk->tokenDesc[tk->manyTokens] = "Binary Operator: Less Than";
		tk->curChar++;
		if((char)tk->str[tk->curChar] == '=')
		{
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->tokenDesc[tk->manyTokens] = "Binary Operator: Less Than or Equal To";
			tk->curChar++;
		}
		else if((char)tk->str[tk->curChar] == '<')
		{
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->tokenDesc[tk->manyTokens] = "Binary Operator: Left Shift";
			tk->curChar++;

			if((char)tk->str[tk->curChar] == '=')
			{
				arr[i] = tk->str[tk->curChar];
				i++;
				tk->tokenDesc[tk->manyTokens] = "Assignment Operator: Left Shift Equals To";
				tk->curChar++;
			}
		}
		break;
	}
	else if((char)tk->str[tk->curChar] == '>')
	{
		arr[i] = tk->str[tk->curChar];
		i++;
		tk->tokenDesc[tk->manyTokens] = "Binary Operator: Greater Than";
		tk->curChar++;
		if((char)tk->str[tk->curChar] == '=')
		{
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->tokenDesc[tk->manyTokens] = "Binary Operator: Greater Than or Equal To";
			tk->curChar++;
		}
		else if((char)tk->str[tk->curChar] == '>')
		{
			arr[i] = tk->str[tk->curChar];
			i++;
			tk->tokenDesc[tk->manyTokens] = "Binary Operator: Right Shift";
			tk->curChar++;

			if((char)tk->str[tk->curChar] == '=')
			{
				arr[i] = tk->str[tk->curChar];
				i++;
				tk->tokenDesc[tk->manyTokens] = "Assignment Operator: Right Shift Equal To";
			}
		}
		break;
	}
	else if((char)tk->str[tk->curChar] == '\%')
	{
		arr[i] = tk->str[tk->curChar];
		i++;
		tk->curChar++;
		if((char)tk->str[tk->curChar] == 'd')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "signed decimal integer conversion character";
		}
		else if((char)tk->str[tk->curChar] == 'u')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "unsigned decimal integer conversion character (used in printf only)";
		}
		else if((char)tk->str[tk->curChar] == 'x')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "unsigned hexidecimal integer conversion character";
		}
		else if((char)tk->str[tk->curChar] == 'h')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "unsigned short integer conversion character (used in scanf only)";
		}
		else if((char)tk->str[tk->curChar] == 'o')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "unsigned octal integer conversion character";
		}
		else if((char)tk->str[tk->curChar] == 'c')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "single character conversion character";
		}
		else if((char)tk->str[tk->curChar] == 's')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "null terminated string conversion character";
		}
		else if((char)tk->str[tk->curChar] == 'f')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "fixed point notation for float or double conversion character";
		}
		else if((char)tk->str[tk->curChar] == 'e')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "scientific notation for float or double conversion character (printf only)";
		}
		else if((char)tk->str[tk->curChar] == 'g')
		{
			arr[i] = tk->str[tk->curChar];
			tk->tokenDesc[tk->manyTokens] = "use %e or %f, whichever is shorter conversion character (printf only)";
		}
		tk->curChar++;
		break;
	}
	} // End while-loop
	return arr;
} // End TKGetNextToken

/*
* main will have a string argument (in argv[1]).
* The string argument contains the tokens.
* Print out the tokens in the second string in left-to-right order.
* Each token should be printed on a separate line.
*/
int main(int argc, char **argv)
{ // Begin Main Method
	/**
	* Build a structure of type TokenizerT.
	*/
	struct TokenizerT_ *tk = TKCreate(argv[1]);
	printf("Original string: %s\n", tk->str);
	while(tk->str[tk->curChar] != '\0')
	{ // Begin while-loop
		if (tk->curChar > tk->strSize-1)
		{ // Begin if-statement
			break;
		} // End if-statement
		tk->tokens[tk->manyTokens] = TKGetNextToken(tk);
		if(tk->tokenDesc[tk->manyTokens] != NULL)
		{ // Begin if-statement
			printf("%s is a %s\n", tk->tokens[tk->manyTokens], tk->tokenDesc[tk->manyTokens]) ;
			tk->manyTokens++;
		} // End if-statement
	} // End while-loop

	printf("Original string analtered after tokenizing: %s\n", tk->str);
	// Destroy the Tokenizer structure.
	TKDestroy(tk);
	return 0;
} // End Main Method
