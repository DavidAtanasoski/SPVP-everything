#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 20_mpi 20_mpi.c
  Execute: mpirun -np 4 20_mpi
*/

int main(int argc, char *argv[]) {
	int p, id, i;
	int *sarr, *rarr;
	int *sdispl, *rdispl, *scounts, *rcounts;
	int ssize, rsize;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	sdispl = (int *) malloc(p * sizeof(int));
	rdispl = (int *) malloc(p * sizeof(int));
	scounts = (int *) malloc(p * sizeof(int));
	rcounts = (int *) malloc(p * sizeof(int));

	for (i = 0; i < p; i++) {
		scounts[i] = i + 1;
	}

	MPI_Alltoall(scounts, 1, MPI_INT, rcounts, 1, MPI_INT, MPI_COMM_WORLD);

	sdispl[0] = 0;
	for (i = 1; i < p; i++) {
		sdispl[i] = sdispl[i-1] + scounts[i-1];
	}

	rdispl[0] = 0;
	for (i = 1; i < p; i++) {
		rdispl[i] = rdispl[i-1] + rcounts[i-1];
	}

	for (i = 0; i < p; i++) {
		printf("%d) %d %d %d %d \n", id, scounts[i], rcounts[i], sdispl[i], rdispl[i]);
	}

	ssize = 0; rsize = 0;
	for (i = 0; i < p; i++) {
		ssize += scounts[i];
		rsize += rcounts[i];
	}

	sarr = (int *) malloc(ssize * sizeof(int));
	rarr = (int *) malloc(rsize * sizeof(int));

	for (i = 0; i < ssize; i++) {
		sarr[i] = id;
	}

	MPI_Alltoallv(sarr, scounts, sdispl, MPI_INT, rarr, rcounts, rdispl, MPI_INT, MPI_COMM_WORLD);

	printf("%d)", id);
	for (i = 0; i < rsize; i++) {
		printf("%d, ", rarr[i]);
	}
	printf("\n");

	MPI_Finalize();
}
 
