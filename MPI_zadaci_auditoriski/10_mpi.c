#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 9_mpi 9_mpi.c
  Execute: mpirun -np 4 9_mpi
*/

int main(int argc, char *argv[]) {
	int id, p;
	int source;
	int *array, count;
	int local_min, global_min;
	MPI_Status status;

	MPI_Init(&argc, &argv);
		
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id); 
	
	source = 0;

	if (id == source) {
		count = 20;
		array = (int *) malloc(count * sizeof(int));
		for (int i = 0; i < count; i++) {
			array[i] = rand() % 50;
		}
	}

	MPI_Bcast(&count, 1, MPI_INT, source, MPI_COMM_WORLD);
	if (id != source) {
		array = (int *) malloc(count * sizeof(int));
	}

	MPI_Bcast(array, count, MPI_INT, source, MPI_COMM_WORLD);

	if (id != source) {
		printf("Process %d ja primi nizata. Nizata ima %d elementi i e:\n", id, count);
		for (int i = 0; i < count; i++) {
			printf("%d, ", array[i]); 
		}
		printf("\n");
	}

	local_min = array[id];
	for (int i = id; i < count; i += p) {
		if (array[i] < local_min) {
			local_min = array[i];
		}
	}

	MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, source, MPI_COMM_WORLD);

	if (id == source) {
		printf("Minimum is %d.\n", global_min);
	}
	
	MPI_Finalize();
}


