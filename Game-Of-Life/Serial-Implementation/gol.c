#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"

#define DEBUG 1

int lifecheck(int lifecounter, int A)
{
	if (A == 1)
	{
		if ((lifecounter < 2) || (lifecounter > 3))
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		if (lifecounter == 3)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

int main(int argc, char ** argv)
{
	// @count  - Used to determine how many life cycles specified by the user
	// @height - Specifies the height of the life grid
	// @width  - Specifies the width of the life grid 
	int count, width, height, ** A, **B;
	count = atoi(argv[1]);
	height = atoi(argv[2]);
	width = atoi(argv[3]);

	// Life counter determines the state of the next cell
	int lifecounter = 0;
	
	// Loop variables
	int i, j, a, b;

	// Timer variables
	double start, end;

	// Matrix A holds the values of the initial states
	A = (int **) calloc(height, sizeof(int *));

	// Matrix B holds the new values
	B = (int **)calloc(height, sizeof(int *));

	// Allocate and initialize the two matrices
	for(i = 0; i < height; i++)
	{
		// Allocate the space of an int* multiplied by the width specified by the user per row
		A[i] = (int *) calloc(width, sizeof(int *));
		
		// Allocate the space of an int* multiplied by the width specified by the user per row 
		B[i] = (int *) calloc(width, sizeof(int *));
		
		// Iterate through the elements of that row, populating with elements
		for(j = 0; j < width; j++)
		{
			A[i][j] = ((rand()%2)+(rand()%2)+(rand()%2))%2;
			B[i][j] = 0;
		}
	}
	
	start = get_time();	

	// Iterate through as many life cycles as specified by the user.
	while (count > 0)
	{
		// Print out the current state of the matrix
        	/*for(i = 0; i < height; i++)
		{
               		for(j = 0; j < width; j++)
               		{
                	        printf("%d ", A[i][j]);
			}
                        printf("\n");
 	        }
		printf("\n");*/
		for(i = 0; i < height; i++)
		{
			for(j = 0; j < width; j++)
			{
				// Check if top left corner
				if ((i == 0) && (j == 0))
				{
					// Search through the moore's neighborhood of the cell at the
					// specifed indices
					for(a = 0; a <= 1; a++)
					{
						for(b = 0; b <= 1; b++)
						{
							if ((a == b) && (a == 0))
							{
								// Do nothing
							}
							else
							{
								if (A[i+a][j+b] == 1)
								{	
									// Increment the neighbor counter
									lifecounter++;
								}
							}
						}
					}
				}
				// Check if top right corner
				else if ((i == 0) && (j == width-1))
				{
					for(a = 0; a <= 1; a++)
					{
						for(b = -1; b < 1; b++)
						{
							if ((a == b) && (a == 0))
							{
								// Do nothing
							}
							else
							{
								if (A[i+a][j+b] == 1)
								{
									// Increment the neighbor counter
									lifecounter++;
								}
							}
						}
					}
				}
				// Check if top
                                else if (i == 0)
                                {
                                        for(a = 0; a <= 1; a++)
                                        {
                                                for(b = -1; b <= 1; b++)
                                                {
                                                        if ((a == b) && (a == 0))
                                                        {
                                                                // Do nothing
                                                        }
                                                        else
                                                        {
                                                                if (A[i+a][j+b] == 1)
                                                                {
                                                                        // Increment the neighbor counter
                                                                        lifecounter++;
                                                                }
                                                        }
                                                }
                                        }
                                }
				// Check if bottom left corner
                                else if ((i == height-1) && (j == 0))
                                {
                                        for(a = -1; a < 1; a++)
                                        {
                                                for(b = 0; b <= 1; b++)
                                                {
                                                        if ((a == b) && (a == 0))
                                                        {
                                                                // Do nothing
                                                        }
                                                        else
                                                        {
                                                                if (A[i+a][j+b] == 1)
                                                                {
                                                                        // Increment the neighbor counter
                                                                        lifecounter++;
                                                                }
                                                        }
                                                }
                                        }
                                }
				// Check if bottom right corner
                                else if ((i == height-1) && (j == width-1))
                                {
                                        for(a = -1; a < 1; a++)
                                        {
                                                for(b = -1; b < 1; b++)
                                                {
                                                        if ((a == b) && (a == 0))
                                                        {
                                                                // Do nothing
                                                        }
                                                        else
                                                        {
                                                                if (A[i+a][j+b] == 1)
                                                                {
                                                                        // Increment the neighbor counter
                                                                        lifecounter++;
                                                                }
                                                        }
                                                }
                                        }
                                }
				// Check if left side
                                else if (j == 0)
                                {
                                        for(a = -1; a <= 1; a++)
                                        {
                                                for(b = 0; b <= 1; b++)
                                                {
                                                        if ((a == b) && (a == 0))
                                                        {
                                                                // Do nothing
                                                        }
                                                        else
                                                        {
                                                                if (A[i+a][j+b] == 1)
                                                                {
                                                                        // Increment the neighbor counter
                                                                        lifecounter++;
                                                                }
                                                        }
                                                }
                                        }
                                }
				// Check if right side
                                else if (j == width-1)
                                {
                                        for(a = -1; a <= 1; a++)
                                        {
                                                for(b = -1; b < 1; b++)
                                                {
                                                        if ((a == b) && (a == 0))
                                                        {
                                                                // Do nothing
                                                        }
                                                        else
                                                        {
                                                                if (A[i+a][j+b] == 1)
                                                                {
                                                                        // Increment the neighbor counter
                                                                        lifecounter++;
                                                                }
                                                        }
                                                }
                                        }
                                }
				// Check if bottom
                                else if (i == height-1)
                                {
                                        for(a = -1; a < 1; a++)
                                        {
                                                for(b = -1; b <= 1; b++)
                                                {
                                                        if ((a == b) && (a == 0))
                                                        {
                                                                // Do nothing
                                                        }
                                                        else
                                                        {
                                                                if (A[i+a][j+b] == 1)
                                                                {
                                                                        // Increment the neighbor counter
                                                                        lifecounter++;
                                                                }
                                                        }
                                                }
                                        }
                                }
				// Else just check the standard Moore's neighborhood of the given cell
                                else
                                {
                                        for(a = -1; a <= 1; a++)
                                        {
                                                for(b = -1; b <= 1; b++)
                                                {
                                                        if ((a == b) && (a == 0))
                                                        {
                                                                // Do nothing
                                                        }
                                                        else
                                                        {
                                                                if (A[i+a][j+b] == 1)
                                                                {
                                                                        // Increment the neighbor counter
                                                                        lifecounter++;
                                                                }
                                                        }
                                                }
                                        }
                                }

	
	                        // Set the new life value at the specified indices
        	                B[i][j] = lifecheck(lifecounter, A[i][j]);

                	        // Bring the life counter down to zero
                        	lifecounter = 0;

			}
		}
		
		// Decrement the amount of life cycles remaining
		count--;
		
		// Set the values of the matrix A to the new values
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				A[i][j] = B[i][j];
			}
		}
	}
	
	end = get_time();
	printf("Elapsed time %g\n", (end - start));

	// Free allocated space to matrices
	for (i = 0; i < height; i++)
	{
		free(A[i]);
		free(B[i]);
	}

	free(A);
	free(B);
	return 0;
}
