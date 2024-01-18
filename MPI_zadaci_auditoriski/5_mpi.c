#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 5_mpi 5_mpi.c
  Execute: mpirun -np 4 5_mpi
*/

int main(int argc, char *argv[]) {
	int id, p, t;
	int source;
	int *array, count;
	MPI_Status status;

	MPI_Init(&argc, &argv); 
		
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id); 
	
	source = 0;

	if (id == source) { 
		count = 5;
		array = (int *) malloc(count * sizeof(int));
		for (int i = 0; i < count; i++) {
			t = scanf("%d", &array[i]);
		}
	}

	MPI_Bcast(&count, 1, MPI_INT, source, MPI_COMM_WORLD);
	if (id != source) {
		array = (int *) malloc(count * sizeof(int));
	}

	MPI_Bcast(array, count, MPI_INT, source, MPI_COMM_WORLD);

	if (id != source) {
		printf("Process %d ja primi nizata. Nizata ima %d elementi i e: ", id, count);
		for (int i = 0; i < count; i++) {
			printf("%d, ", array[i]); 
		}
	}
	
	MPI_Finalize();
}


