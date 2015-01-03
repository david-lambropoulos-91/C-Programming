/**
*	@author: David Lambropoulos
*	@Date: 1/3/2015
*	Problems 2-6,2-7,2-8 from The C Programming 
*	Language Ritchie & Kernighan. 
**/

#include <stdio.h>

/**
*	Returns the (right adjusted) n-bit field of x
*	that begins at position p.
**/
unsigned getbits(unsigned x, int p, int n)
{
	return (x >> (p+1-n)) & ~(~0 << n);
}

/**
*	Returns x with the n bits that begin at position
*	p set the rightmost n bits of y, leaving the
*	other bits unchanged.
**/
unsigned setBits(unsigned x, int p, int n, unsigned y)
{
	return getbits(x,p,n) ^ getbits(y,p,n) ^ x;	
}

/**
*	Returns x with the n bits that begin at position
*	p inverted(i.e., 1 changed into 0 and vice versa),
*	leaving others unchanged.
**/
unsigned invert(unsigned x, int p, int n)
{
	return getbits(~x,p,n) ^ getbits(x,p,n) ^ x;
}

/**
*	Returns the value of the integer x rotated to the
*	right by n positions.
**/
unsigned rightrot(unsigned x, int n)
{
	for(n; n > 0; --n)
	{
		if((x & 1) == 0)
			x = (x >> 1);
		else
			x = (x >> 1) | ~(~0U >> 1);
	}

	return x;
}

main()
{
	unsigned x = 4;	//First n-bit number.
	int p = 2;	//Position
	int n = 2;	//Number of bits
	unsigned y = 4;	//Second n-bit number.
	unsigned z = setBits(x,p,n,y);	//new value of x
	unsigned w = invert(x,p,n);	
	unsigned v = rightrot(x,n);
	
	//DIsplay values
	printf("The new bits of x after setBits are: %u\n", z); 
	printf("The new inverted bits of x are: %u\n", w);
	printf("The x rotated n bits to the right is: %u\n", v);
}
