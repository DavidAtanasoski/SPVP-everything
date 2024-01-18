#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 6_mpi 6_mpi.c
  Execute: mpirun -np 4 6_mpi
*/

int main(int argc, char *argv[]) {
	int id, p, t;
	int tag, source;
	int trials = 10;
	double sendrecv_time, bcast_time;
	int *array, count;
	MPI_Status status;

	MPI_Init(&argc, &argv);
		
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id); 
	
	source = 0;
	tag = 1234; 

	if (id == source) { 
		count = 5;
		array = (int *) malloc(count * sizeof(int));
		for (int i = 0; i < count; i++) {
			t = scanf("%d", &array[i]);
		}
	}

	for (int i = 0; i < trials; i++) {
		MPI_Barrier(MPI_COMM_WORLD);
		sendrecv_time -= MPI_Wtime();
		if (id == source) {
			for (int i = 1; i < p; i++) {
				MPI_Send(&count, 1, MPI_INT, i, tag, MPI_COMM_WORLD);
				MPI_Send(array, count, MPI_INT, i, tag, MPI_COMM_WORLD);
			}
		}
		if (id != source) {
			MPI_Recv(&count, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
			array = (int *) malloc(count * sizeof(int));
			MPI_Recv(array, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
		}
		MPI_Barrier(MPI_COMM_WORLD);
		sendrecv_time += MPI_Wtime();

		MPI_Barrier(MPI_COMM_WORLD);
		bcast_time -= MPI_Wtime();

		MPI_Bcast(&count, 1, MPI_INT, source, MPI_COMM_WORLD);
		if (id != source) {
			array = (int *) malloc(count * sizeof(int));
		}

		MPI_Bcast(array, count, MPI_INT, source, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		bcast_time += MPI_Wtime();
	}

	if (id == source) { 
		printf("Time values are: %.6f for send/recv combination, %.6f for implemented broadcast\n", sendrecv_time/trials, bcast_time/trials);
	}
	
	MPI_Finalize();
}


