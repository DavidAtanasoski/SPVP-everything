#include<stdio.h>
#include<mpi.h>

// Send/recv; Isend/irecv.
/*
  Compile: mpicc -O -o 9_mpi 9_mpi.c
  Execute: mpirun -np 2 9_mpi
*/

int main(int argc, char *argv[]) {
	int id, p;
	int tag, source, destination;
	int buffer, count, t;
	MPI_Status status;
	MPI_Request request;

	MPI_Init(&argc, &argv); 
		
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id); 
	
	source = 0; destination = 1;
	tag = 1234; count = 1;

	if (id == source) { 
		t = scanf("%d", &buffer);
		MPI_Send(&buffer, count, MPI_INT, destination, tag, MPI_COMM_WORLD);
	}

	if (id == destination) { 
		MPI_Recv(&buffer, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
		printf("Vrednosta e %d\n", buffer); 
		printf("Send/Recv status information. MPI_SOURCE = %d, MPI_TAG = %d, MPI_ERROR = %d\n", status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (id == source) { 
		t = scanf("%d", &buffer);
		MPI_Isend(&buffer, count, MPI_INT, destination, tag, MPI_COMM_WORLD, &request);
	}

	if (id == destination) { 
		MPI_Irecv(&buffer, count, MPI_INT, source, tag, MPI_COMM_WORLD, &request);
		printf("Vrednosta e %d\n", buffer); 
	}
	
	MPI_Wait(&request, &status);
	if (id == destination) {
		printf("Isend/Irecv status information (no coordination). MPI_SOURCE = %d, MPI_TAG = %d, MPI_ERROR = %d\n", status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (id == source) { 
		t = scanf("%d", &buffer);
		MPI_Isend(&buffer, count, MPI_INT, destination, tag, MPI_COMM_WORLD, &request);
	}

	if (id == destination) { 
		MPI_Irecv(&buffer, count, MPI_INT, source, tag, MPI_COMM_WORLD, &request);
	}
	
	MPI_Wait(&request, &status);

	if (id == destination) {
		printf("Vrednosta e %d\n", buffer); 
		printf("Isend/Irecv status information (with coordination). MPI_SOURCE = %d, MPI_TAG = %d, MPI_ERROR = %d\n", status.MPI_SOURCE, status.MPI_TAG, status.MPI_ERROR);
	}

	MPI_Finalize();
}


