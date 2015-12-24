/*
	Walsh Hadamard Transform
	OpenMP Implementation
	@authors
		Harsh Shah
		Demetrious Lambropoulos
		Davidek Lambropoulos
*/

#include "omp.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int bitwise_dotproduct(int input) {
  int numOneBits = 0;

  int currNum = input;
  while (currNum != 0) {
    if ((currNum & 1) == 1) {
      numOneBits++;
    }
    currNum = currNum >> 1;
  }
  return numOneBits;
} 

int hadamard_entry(int k, int n){
	int a = k & n;
    int count = bitwise_dotproduct(a); 
	if((count % 2) == 0){
		return 1;
	}
	return -1;
}

int main(){
	
	//input argument
	int m;	
	printf("Please Enter the m in 2^m \n");
	scanf("%d", &m);
	
	//Dimensions	
	int dim = (int)(pow(2, (double)m));
	
	//Input and Result Vector
	double	*in, *out;
	in = (double*)malloc(sizeof(double) * dim);
	out = (double*)malloc(sizeof(double) * dim);

	//Generate in vector
	int z = 0;
	for(z = 0; z < dim; z++){
		in[z] = 1;
	}

	//Hadamard coeff
	double hadamard_coeff = 1 / pow(2,(double)(dim/2));

	int i;
//Compute the result of WHT
#pragma omp parallel for 
for(i = 0; i < dim; i++){
		int sum = 0;
		int j;
#pragma omp parallel for reduction(+ : sum)
for(j = 0; j < dim; j++){
		sum = sum + hadamard_entry(i,j) * in[j];
}
		out[i] = sum * hadamard_coeff;
}

	
	//Print the result
	for(z = 0; z < dim; z++){
		printf("%f \n", out[z]);
	}

	return 0;		

}
