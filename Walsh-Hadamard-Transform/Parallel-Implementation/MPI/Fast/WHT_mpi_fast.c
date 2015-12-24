/*
 *	Walsh Hadamard Transform 
 *	Fast Implementation
 *	@author
 *		Harsh Shah
 *		Davidek Lambropoulos
 *		Demetrios Lambropoulos
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "timer.h"

#define MASTER 0
#define FROM_MASTER 1
#define FROM_WORKER 2
#define DONE 3

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

MPI_Status status;

int main(int argc, char* argv[]){
	
	//timing
	double start, end;

	//Input Dimension Parameter
	int m = atoi(argv[1]);
	int dim = (int)pow(2,(double)m);
	
	//Input Vector and Result Vector
	int *vector = (int*)malloc(sizeof(int)*dim);	
	int *result = (int*)malloc(sizeof(int)*dim);

	//Initialize the input vector
	int iter;
	for(iter = 0; iter < dim; iter++){
		vector[iter] = iter;
	}
	for(iter = 0; iter < dim; iter++){
		result[iter] = 0;
	}

	/** MPI Params ***/
	int numprocs, numworkers, procid;
	int i,j,x,z;
	int start_index, end_index;
	int dest;
	int count = 1;

	//Build Reception Structure
	int ri,rj,h,curr, div;
	h = 0;
	curr = 1;
	int finished = 1;
	
	start = get_time();
/************** MPI START ************************/	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &procid);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	//Generate Space for reception matrix
	numworkers = numprocs - 1;


	MPI_Bcast(&(vector[0]), dim, MPI_INT, MASTER, MPI_COMM_WORLD);
	MPI_Bcast(&(result[0]), dim, MPI_INT, MASTER, MPI_COMM_WORLD);

	if(procid == MASTER){
        	i = dim/2;
		start_index = 0; 
		end_index = i - 1;
		for(dest = 1; dest <= 2; dest++){
			MPI_Send(&start_index, 1 , MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
			MPI_Send(&end_index, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
			MPI_Send(&i, 1, MPI_INT, dest, FROM_MASTER, MPI_COMM_WORLD);
			start_index = end_index + 1;
			end_index = start_index + i - 1;
		}
		for(dest = 1; dest <= 2; dest++){
			MPI_Recv(&finished, 1, MPI_INT, dest, FROM_WORKER, MPI_COMM_WORLD, &status);
		}

	}

	if(procid > MASTER){
		if(procid >=  3){}
		else{
			MPI_Recv(&start_index, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
			MPI_Recv(&end_index, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
			MPI_Recv(&i, 1, MPI_INT, MASTER, FROM_MASTER, MPI_COMM_WORLD, &status);
			end_index = i - 1;
			int ti, tj, tx, tz, titer;
			tx = 2; ti = i/2;
			for(ti; ti >= 1; ti = ti/2){
				for(tj = start_index; tj < (int)pow(2,(double)tx); tj = tj + ti + ti){
					for(tz = tj; tz < (tj + ti); tz++){
						result[tz] = vector[tz] + vector[tz + ti];
						result[tz + ti] = vector[tz] - vector[tz + ti];
						printf("%d : %d\n",tz, result[tz]);
						printf("%d : %d\n",tz + ti, result[tz + ti]);
					}
				}
				printf("\n");
				for(titer = start_index; titer <= end_index; titer++){
						vector[titer] = result[titer];
				}
				tx++;
			}
			MPI_Send(&finished, 1, MPI_INT, MASTER, FROM_WORKER, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();
/************* MPI END ***************************/
	free(result);
	free(vector);
	end = get_time();
	printf("Elapsed time %g\n", (end - start));

	return 0;
}

