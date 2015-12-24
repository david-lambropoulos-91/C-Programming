#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include "timer.h"

#define MASTER 0
#define FROM_MASTER 1  /* setting a message type */
#define FROM_WORKER 2  /* setting a message type */

//Generate the matrix
int alloc_matrix(int ***array, int n, int m){

    /* allocate the n*m contiguous items */
    int *p = (int *)malloc(n*m*sizeof(int));
    if (!p) return -1;

    /* allocate the row pointers into the memory */
    (*array) = (int **)malloc(n*sizeof(int*));
    if (!(*array)) {
       free(p);
       return -1;
    }

    /* set up the pointers into the contiguous memory */
    for (int i=0; i<n; i++)
       (*array)[i] = &(p[i*m]);

    return 0;
}

//dealloc matrix
int dealloc_matrix(int ***array){
	/* free the memory - the first element of the array is at the start */
	free(&((*array)[0][0]));

	/* free the pointers into the memory */
	free(*array);

	return 0;
}

//Performs a bitwise dot product
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

//Calculates the value of the Hadamard matrix at any index
int hadamard_entry(int k, int n){
	int a = k & n;
    	int count = bitwise_dotproduct(a);
	if((count % 2) == 0){
		return 1;
	}
	return -1;
}

//MPI Error Printer
void check(int e){
	if(e != MPI_SUCCESS){
		printf("MPI ERROR CODE: &d", e);
	}
}

//Prints a vector
void printvec(double *arr, int size){
	int i = 0;
	for(i = 0; i < size; i++){
		printf("%f ", arr[i]);
	}
}

MPI_Status status;

int main(int argc, char **argv){

	//Dimension Paramter dim
	int dim = (int)pow(2,(double)atoi(argv[1]));

	int procid, numprocs, numworkers;
	int dest;
	int finished;

	//Allocate Memory for arguements
	int  *vector;

	//Initiliaze the vector
	vector = (int*)malloc(sizeof(int)*dim);
	int iter;
	for(iter = 0; iter < dim; iter++){
		vector[iter] = 1;
	}

	//Initialize Result Vector
	int *result = (int*)malloc(sizeof(int)*dim);

	//MPI Params
	int start_index, end_index, width, extra;
	int temp = 0;

	//Timing 
	double start, end;
	double *times = (double*)malloc(sizeof(double)*dim);

	start = get_time();

	//Initialize MPI Framework
	MPI_Init(&argc, &argv);
	//Assign IDs to all the nodes
	MPI_Comm_rank(MPI_COMM_WORLD, &procid);
	//Get the number of processes
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);



	//Num workers is one minus processes due to master process
	numworkers = numprocs - 1;
	width = (dim/numprocs);
	
	MPI_Bcast(&(times[0]), dim, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);	
	MPI_Bcast(&width, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&(vector[0]), dim, MPI_INT, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&dim, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&(result[0]), dim, MPI_INT, MASTER, MPI_COMM_WORLD);

	/********** master process **********/
	if(procid == MASTER){
		//printf("Master sees dim as %d\n",dim);
		width = (dim)/numprocs;
		if(width == 0){	//Number of workers > elements of the matrix
			start_index = 0;
			end_index = start_index + dim - 1;
			for(dest = 1; dest < dim; dest++){
				MPI_Send(&start_index, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
				MPI_Send(&end_index, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
				start_index = end_index + 1;
				end_index = start_index + dim - 1;
			}

		}else{		//Number of workers < elements of the matrix
			start_index = 0;
			end_index = (width*dim) - 1;
			int ff = 0;
			for(dest = 1; dest <= numworkers; dest++){
				MPI_Send(&start_index, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
				MPI_Send(&end_index, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
				start_index = end_index + 1;
				end_index = start_index + (width*dim) - 1;
				ff++;
			}
			//Master Will do work
			while(start_index <= ((dim*dim) - 1)){
				int h_i = start_index / dim;
				int h_j = 0;
				int t_end = h_i + width - 1;
				while(h_i <= t_end){
					//printf("Processor %d is doing %d\n", procid, h_i);
					int value = 0;
					for(h_j = 0; h_j < dim; h_j = h_j + 1){
						value += hadamard_entry(h_i, h_j);
					}
					result[h_i] = value;
					h_i = h_i + 1;
				}
				start_index = end_index + 1;
				end_index = start_index + (width*dim) - 1;
			}
		}
	}
	/********** worker process **********/
	if(procid > MASTER){
		if(width == 0){
			if(procid > dim){
				//Do nothing
			}else{
				MPI_Recv(&start_index, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
				MPI_Recv(&end_index, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
				int h_i = start_index/dim;
				int h_j = 0;
				int value = 0;
				for(h_j = 0; h_j < dim; h_j++){
					value += hadamard_entry(h_i, h_j) + vector[h_j];
				}
				result[h_i] = value;
			}
			times[procid] = end - start;
		}

		if(width != 0){	
			MPI_Recv(&start_index, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
			MPI_Recv(&end_index, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
			int h_i = start_index / dim; //Hadamard Matrix Index i
			int h_j = 0; //Hadamard Matrix index j
			int t_end = h_i + width - 1;
	
			while(h_i <= t_end){

				int value = 0;
				for(h_j = 0; h_j < dim; h_j = h_j + 1){
					value += hadamard_entry(h_i, h_j) + vector[h_j];
				}
				result[h_i] = value;
				h_i = h_i + 1;
			}
			times[procid] = end - start;
		}

	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();
	
	end = get_time();
	printf("Elapsed time %g\n",end - start);
	
	free(times);
	free(result);
	free(vector);
	return EXIT_SUCCESS;
}
