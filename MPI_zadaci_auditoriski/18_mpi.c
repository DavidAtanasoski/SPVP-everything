#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 18_mpi 18_mpi.c
  Execute: mpirun -np 4 18_mpi
*/

int main(int argc, char *argv[]) {
	int id, p;
	int i, j;
	int source;
	int *array, *array_local, count, count_local, recv_total=0, t;
	int *counts, *displacements, *recv_displacements;
	int *global_result, local_result;
	int *global_results, *local_results;
	MPI_Status status;

	MPI_Init(&argc, &argv);
		
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id); 
	
	source = 0;

	if (id == source) {
		t = scanf("%d", &count);

		array = (int *) malloc(count * sizeof(int));
		for (i = 0; i < count; i++) {
			array[i] = rand() % 20;
		}

		counts = (int *) malloc(p * sizeof(int));
		displacements = (int *) malloc(p * sizeof(int));

		for (i = 0; i < p; i++) {
			counts[i] = count/p;
		}

		for (i = 0; i < count % p; i++) { // for (i = 0; i < count % p; i++)
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

	local_results = (int *) malloc(count_local * sizeof(int));

	local_result = 0;
	for (int i = 0; i < count_local; i++) {
		if (array_local[i] % 2 == 0) {
			local_results[local_result] = array_local[i];
			local_result++;
		}
	}

	if (id == source) {
		global_result = (int *) malloc(p * sizeof(int));
	}

	MPI_Gather(&local_result, 1, MPI_INT, global_result, 1, MPI_INT, source, MPI_COMM_WORLD);

	if (id == source)
	{
		for (i = 0; i < p; i++)
		{
			printf("%d ", global_result[i]);
		}
	}
	if (id == source) {
		recv_displacements = (int *) malloc(p * sizeof(int));
		recv_displacements[0] = 0;
		for (i = 1; i < p; i++) {
			recv_displacements[i] = recv_displacements[i-1] + global_result[i-1];
			recv_total += global_result[i-1];
		}
		recv_total += global_result[p-1];
		global_results = (int *) malloc(recv_total * sizeof(int));
	}

	MPI_Gatherv(local_results, local_result, MPI_INT, global_results, global_result, recv_displacements, MPI_INT, source, MPI_COMM_WORLD);

	if (id == source) {
		printf("Brojot na elementi shto ispolnuvaat po procesi e:\n");
		for (i = 0; i < p; i++) {
			printf("%d) %d,", i, global_result[i]);
		}
		printf("Vkupno takvi se %d.\n", recv_total);
		printf("\nA samite elementi se:\n");
		for (i = 0; i < recv_total; i++) {
			printf("%d) %d,", i, global_results[i]);
		}
		printf("\n");
	}

	if (id == source) {
		free(array);
	}
	free(array_local);
	
	MPI_Finalize();
}


