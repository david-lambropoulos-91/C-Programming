/**
*	@author: David Lambropoulos
*	Date: 12/28/2014
*	
*	Exercise 1-13: The C Programming Language 
*			Ritchie and Kernighan
*
*	Write a program to print a histogram of the lengths
*	of words in its input using vertical orientation.
**/

#include <stdio.h>

main()
{
	int c, wc, i, j, largest;
	int histogram[10];

	wc = 0;
  
  // Initialize the array
	for(i = 0; i < 10; ++i)
		histogram[i] = 0;

  // Get the input from the user
	while ((c = getchar()) != EOF)
	{
		if (c == ' ' || c == '\n' || c == '\t')
		{

			histogram[wc] = histogram[wc] + 1;
			wc = 0;
		}
		else
		{
			 wc++;
		}
	}

	printf("\n");

  // Determine the largest value in the histogram
	for (i = 0; i < 10; ++i)
		if (i == 0)
			largest = histogram[i];
		else if (histogram[i] > largest)
			largest = histogram[i];
  
  // Print the histogram row by row
	for(i = largest; i > 0; --i)
	{
		printf("%i |", i);
		for(j = 0; j < 10; ++j)
		{
			if (histogram[j] == i)
			{
				printf("x");
				histogram[j] = histogram[j] - 1;
			}
			else
				printf(" ");
		}
		printf("\n");
	}

	printf("-------------\n   ");

	for (i = 0; i < 10; ++i)
		printf("%i",i);
	printf("\n");
}
