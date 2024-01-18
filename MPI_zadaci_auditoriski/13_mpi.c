#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 13_mpi 13_mpi.c
  Execute: mpirun -np 4 13_mpi
*/

int main(int argc, char *argv[]) {
	int id, p;
	int source;
	int *array, *array_local, count, count_local, t;
	int local_min, global_min;
	MPI_Status status;

	MPI_Init(&argc, &argv);
		
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id); 
	
	source = 0;

	if (id == source) {
		t = scanf("%d", &count);
		count_local = count / p;

		array = (int *) malloc(count * sizeof(int));
		for (int i = 0; i < count; i++) {
			t = scanf("%d", &array[i]);
		}
	}

	MPI_Bcast(&count_local, 1, MPI_INT, source, MPI_COMM_WORLD);
	array_local = (int *) malloc(count_local * sizeof(int));

	MPI_Scatter(array, count_local, MPI_INT, array_local, count_local, MPI_INT, source, MPI_COMM_WORLD);

	printf("Process %d go primi svojot del od nizata:\n", id);
	for (int i = 0; i < count_local; i++) {
		printf("%d \t", array_local[i]);
	}
	printf("\n");

	local_min = array_local[0];
	for (int i = 0; i < count_local; i++) {
		if (array_local[i] < local_min) {
			local_min = array_local[i];
		}
	}

	MPI_Reduce(&local_min, &global_min, 1, MPI_INT, MPI_MIN, source, MPI_COMM_WORLD);

	if (id == source) {
		printf("Minimum е %d.\n", global_min);
	}

	if (id == source) {
		free(array);
	}
	free(array_local);
	
	MPI_Finalize();
}


