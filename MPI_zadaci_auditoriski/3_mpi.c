#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 3_mpi 3_mpi.c
  Execute: mpirun -np 2 3_mpi
*/

int main(int argc, char *argv[]) {
	int id, p, t;
	int tag, source, destination;
	int *array, count;
	MPI_Status status;

	MPI_Init(&argc, &argv); 
		
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	
	source = 0; destination = 1;
	tag = 1234; 

	if (id == source) { 
		t = scanf("%d", &count);
		array = (int *) malloc(count * sizeof(int));
		for (int i = 0; i < count; i++) {
			array[i] = rand() % 50;
		}
		MPI_Send(array, count, MPI_INT, destination, tag, MPI_COMM_WORLD);
	}

	if (id == destination) {
		MPI_Probe(0, tag, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &count);

		array = (int *) malloc(count * sizeof(int));
		MPI_Recv(array, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

		printf("Nizata ima %d elementi i e: ", count);
		for (int i = 0; i < count; i++) {
			printf("%d, ", array[i]); 
		}
	}
	
	MPI_Finalize();
}


