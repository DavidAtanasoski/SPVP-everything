#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 17_mpi 17_mpi.c
  Execute: mpirun -np 4 17_mpi
*/

int main(int argc, char *argv[]) {
	int id, p;
	int i;
	int source;
	int *array, *array_local, count, count_local, t;
	int *counts, *displacements;
	int *global_result, local_result;
	MPI_Status status;

	MPI_Init(&argc, &argv);
		
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id); 
	
	source = 0;

	if (id == source) {
		t = scanf("%d", &count);

		array = (int *) malloc(count * sizeof(int));
		for (i = 0; i < count; i++) {
			t = scanf("%d", &array[i]);
		}

		counts = (int *) malloc(p * sizeof(int));
		displacements = (int *) malloc(p * sizeof(int));

		for (i = 0; i < p; i++) {
			counts[i] = count/p;
		}

		for (i = 0; i < count - (count/p) * p; i++) { // for (i = 0; i < count % p; i++)
			counts[i]++;
		}

		displacements[0] = 0;
		for (i = 1; i < p; i++) {
			displacements[i] = displacements[i-1] + counts[i-1];
		}
	}

	MPI_Scatter(counts, 1, MPI_INT, &count_local, 1, MPI_INT, 0, MPI_COMM_WORLD);
	array_local = (int *) malloc(count_local * sizeof(int));

	MPI_Scatterv(array, counts, displacements, MPI_INT, array_local, count_local, MPI_INT, source, MPI_COMM_WORLD);

	printf("Process %d go primi svojot del od nizata:\n", id);
	for (int i = 0; i < count_local; i++) {
		printf("%d \t", array_local[i]);
	}
	printf("\n");

	local_result = 0;
	for (int i = 0; i < count_local; i++) {
		if (array_local[i] % 2 == 0) {
			local_result++;
		}
	}

	if (id == source) {
		global_result = (int *) malloc(p * sizeof(int));
	}

	MPI_Gather(&local_result, 1, MPI_INT, global_result, 1, MPI_INT, source, MPI_COMM_WORLD);

	if (id == source) {
		printf("Brojot na elementi shto ispolnuvaat po procesi e:\n");
		for (i = 0; i < p; i++) {
			printf("%d) %d,", i, global_result[i]);
		}
		printf("\n");
	}

	if (id == source) {
		free(array);
	}
	free(array_local);
	
	MPI_Finalize();
}


