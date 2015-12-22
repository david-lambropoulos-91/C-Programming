#include <stdio.h>  
#include <stdlib.h>
#include "tokenizer.c"
#include "fileManager.c"
#include "word-list.c"
#include <time.h>
#include "alerts.c"
 
/**
 * Print a menu to be displayed to the user about how to handle a given file
 */
void printMenu()
{
	printf("Please Choose an Option\n");
	printf("1) Enter new file to save to.\n");
	printf("2) Overwrite the file\n");
	printf("3) Do nothing and leave.\n");
}
      
int main(int argc, char **argv)
{
	char* string;
	char** strings;
	int index;
	int numFiles;
	int i;
	int binaryFiles;
	char** fileNames;
	//int choice;
	//char* newFile = "";

	//Begin timing the program
	clock_t begin, end;
	double time_spent;
	
	begin = clock();

	if(argc == 3) 	
	{  
		if(Exists(argv[1]))
		{
			printf("The file you have specified to save to already exits!\n");
			//printMenu();
			//scanf("%d", &choice);

			//switch(choice)
			//{
			//	case 1:
			//		fgets(newFile, sizeof(newFile), stdin);
			//		break;
			//	case 2:
			//		break;
			//	case 3:
			//		return 0;
			//	default:
			//		printf("\nTHAT IS NOT AN OPTION PROVIDED!!!\n");
			//		return 0;
			//}
			return 0;
		}		


  		if(!Exists(argv[2]))		
		{
			printf("\nCould not open file/directory %s to store into %s\n", argv[2], argv[1]);
		}
		else
		{
			if(isRegularFile(argv[2]))
			{
				if(isBinary(argv[2]) == 0)
				{
					string = fileToString(argv[2]);
	
					TokenizerT *tk = TKCreate(string);
					WordListPtr wl = WLCreate(); 

					while(tk->str[tk->curChar] != '\0')
					{
						if(tk->curChar > tk->strSize-2)
						{
					 		break;
					 	}
				  
						tk->tokens[tk->manyTokens] = TKGetNextToken(tk);
			 			if(strcmp(tk->tokens[tk->manyTokens],"") != 0)
						{
	 						WLInsert(wl, tk->tokens[tk->manyTokens], argv[2]);
						}

						tk->manyTokens++;						
						 
				 	}     
					PrintJSON(wl, argv[1]);
					TKDestroy(tk);
					WLDestroy(wl);
					free(string);
				}
				else
				{
					printf("\n%s is a binary file!\n", argv[2]);
				}
			}
			else if(isDirectory(argv[2]))
			{
				
				binaryFiles = numBinaryFileInDirectory(argv[2]);
				
				numFiles = (numFilesInDirectory(argv[2])-binaryFiles);

				printf("\nThere are %d files in the directory: %s\n", numFiles, argv[2]);
				
				if(binaryFiles != 0)
				{
					if(binaryFiles == 1)
					{
						printf("There is %d Binary File!\n", binaryFiles);
					}
					else
					{
						printf("There are %d Binary Files!\n", binaryFiles);
					}
				}
				
				printFilesInDirectory(argv[2]);
				
				fileNames = filesInDirectory(argv[2]);

				TokenizerT** tk = calloc(numFiles, sizeof(TokenizerT));
				WordListPtr wl = WLCreate();
				index = 1;				

				strings = directoryToString(argv[2]);
				
				for(i = 0; i < numFiles; i++, index++)
				{
					printf("\nFile %d: %s\n",index, strings[i]);
				}
				
				for(i = 0; i < numFiles; i++)
				{				
					tk[i] = TKCreate(strings[i]);

					while(tk[i]->str[tk[i]->curChar] != '\0')
					{
						if(tk[i]->curChar > tk[i]->strSize-2)
						{
							break;
						}
				
						tk[i]->tokens[tk[i]->manyTokens] = TKGetNextToken(tk[i]);

						if(strcmp(tk[i]->tokens[tk[i]->manyTokens],"") != 0)
						{
							//printf("\n%s is a word stored in %s\n", tk[i]->tokens[tk[i]->manyTokens], fileNames[i]);
							WLInsert(wl, tk[i]->tokens[tk[i]->manyTokens], fileNames[i]);
						}
						tk[i]->manyTokens++;
					}
				}
				PrintJSON(wl, argv[1]);

				for(i = 0; i < numFiles; i++)
				{
					TKDestroy(tk[i]);
					free(strings[i]);
				}
				free(strings);
				free(tk);
				WLDestroy(wl);
			}

		}
	}
	else if(argc > 3)
	{
		warning("Too many arguments! Correct execution could not be ensured!");
		//printf("Too many arguments! Correct execution could not be ensured!\n");
		printf("Try again:\nSample: index <inverted-index file name> <directory or file name>\n");
	
	}
	else if(argc == 2)
	{
		warning("Too few arguments! Correct execution could not be ensured!");
		//printf("Too few arguments! Correct execution could not be ensured!\n");
		printf("Try again:\nSample: index <inverted-index file name> <directory or file name>\n");
	}
	else
	{
		warning("No arguments! Correct execution count not be ensured!");
		//printf("No arguments! Correct execution could not be ensured!\n");
		printf("Try again:\nSample: index <inverted-index file name> <directory or file name>\n");
	}
	
	//Stop timing program
	end = clock();

	//Calculate Program execution time
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	//Display time to the user.
	printf("Execution Time: %fs\n", time_spent);

	return 0;
}
