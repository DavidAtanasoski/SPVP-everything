#include<stdio.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 1_mpi 1_mpi.c
  Execute: mpirun -np 2 1_mpi
*/

int main(int argc, char *argv[]) {
	int id, p;
	int tag, source, destination;
	int buffer, count, t;
	MPI_Status status;

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
	}
	
	MPI_Finalize();
}


