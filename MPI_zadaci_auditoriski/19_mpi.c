#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <stddef.h>

/*
  Compile: mpicc -O -o 19_mpi 19_mpi.c
  Execute: mpirun -np 4 19_mpi
*/

typedef struct data_s {
        int key;
        char value;
} data;

int main(int argc, char **argv) {
	int p, id;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	const int count = 2;
	int block_lengths[2] = {1, 1}; 
	MPI_Datatype types[2] = {MPI_INT, MPI_CHAR}; 
	MPI_Datatype custom_type; 
	MPI_Aint displacements[2]; 

	displacements[0] = offsetof(data, key);
	displacements[1] = offsetof(data, value);

	MPI_Type_create_struct(count, block_lengths, displacements, types, &custom_type);
	MPI_Type_commit(&custom_type);

	if (id == 0) {
		data to_send;
		to_send.key = 1;
		to_send.value = 'a';
		MPI_Send(&to_send, 1, custom_type, 1, 123, MPI_COMM_WORLD);
	}

	if (id == 1) {
		data to_recv;
		MPI_Recv(&to_recv, 1, custom_type, 0, 123, MPI_COMM_WORLD, &status);
		printf("Rank %d primi: %d %c\n", id, to_recv.key, to_recv.value);
	}

	MPI_Type_free(&custom_type);
	MPI_Finalize();
}
