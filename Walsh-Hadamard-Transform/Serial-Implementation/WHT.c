/*walsh Hadamard Transform
 * Serial Implementation
 * @author
 * 	Harsh Shah
 * 	David Lambropolos
 * 	Demetrios Lambropoulos
 */

#include <stdio.h>
#include <stdlib.h>
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



int main(int argc, char** argv){
	
	int m; //used for dimension calculation for vector and matrix
	
	printf("Please Enter the m in 2^m \n");
	scanf("%d", &m);

	int dimension = (int)pow(2,(m));

	//Display Input to user
	printf("Input M is: %d \n", m);
	printf("Row and Column size is %d \n", dimension);
	printf("\n");
	//Allocate Vector
	int *vector = (int*)malloc(dimension * sizeof(int));
		
	//Allocate Matrix
	int ** matrix;
	matrix = (int**)malloc(sizeof(int**)* dimension);
	int z = 0;
	for(; z < dimension; z++){
		matrix[z] = (int*)malloc(sizeof(int)*dimension);
	}

	//Allocate Result Vector
	double* result = (double*)malloc(sizeof(double) * dimension);

	//Fill vector with random integers
	z = 0;
	for(; z < dimension; z++){
		vector[z] = rand()%2;
	}
	

	//Generate the Hadamard Matrix
	int i,j;
	for(i = 0; i < dimension; i++){
		for(j = 0; j < dimension; j++){ //iterate through matrix
			matrix[i][j] = hadamard_entry(i,j);
		}
	}


	//Perform Matrix Vector Multipliocation
	for(i = 0; i < dimension; i++){
		int sum = 0;
		for(j = 0; j < dimension; j++){
			sum += matrix[i][j]*vector[j];
		}
		result[i] = sum;
	}
	
	//Multiply the result with Hadamard Matrix Coeffecient
	double hadamard_coeff = 1 / pow(2,(double)(dimension/2));
	for(i = 0; i < dimension; i++){
		result[i] = result[i] * hadamard_coeff;
	}

	//Print the Matrix to Test for Correctness
	printf("Hadamard Matrix \n");
	for(i = 0; i < dimension; i++){
		for(j = 0; j < dimension; j++){
			printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	printf("Input Vector: \n");	
	for(i = 0; i < dimension; i++){
		printf("%d \n",vector[i]);
	}	


	printf("\n");
	//Print Result Vector to Test for Correctness
	printf("Result Vector: \n");	
	for(i = 0; i < dimension; i++){
		printf("%f \n",result[i]);
	}	

	//Free the memory
	free(vector);
	z = 0;
	for(;z < dimension; z++){
		free(matrix[z]);
	}
	free(matrix);
	free(result);

	return 0;

}
