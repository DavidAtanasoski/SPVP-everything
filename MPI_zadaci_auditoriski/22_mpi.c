#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 22_mpi 22_mpi.c
  Execute: mpirun -np 4 22_mpi
*/

int main(int argc, char *argv[]) {
	int p, id, i, j, t;
	int m1, m2;
	int f, s, c, pom;
	int count, overflow, offset, rows;

	int **a;

	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (id == 0) {
		t = scanf("%d %d", &m1, &m2);
		
		a = (int **) malloc(m1 * sizeof(int *));
		for (i = 0; i < m1; i++) {
			a[i] = (int *) malloc(m2 * sizeof(int));
		}

		for (i = 0; i < m1; i++) {
			for (j = 0; j < m2; j++) {
				a[i][j] = rand() % 10;
				printf("a[%d][%d] = %d, ", i, j, a[i][j]);
			}
			printf("\n");
		}

		count = m1 / p;
		overflow = m1 % p;
		offset = (id < overflow) ? count + 1 : count;

		for (i = 1; i < p; i++) {
			rows = (i < overflow) ? count + 1 : count; 
			MPI_Send(&m2, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			for (j = 0; j < rows; j++) {
				MPI_Send(&a[offset+j][0], m2, MPI_INT, i, 1, MPI_COMM_WORLD);
			}
			offset += rows; 
		}
	}

	if (id > 0) {
		MPI_Recv(&m2, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&rows, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		
		a = (int **) malloc(rows * sizeof(int *));
		for (i = 0; i < rows; i++) {
			a[i] = (int *) malloc(m2 * sizeof(int));
		}

		for (j = 0; j < rows; j++) {
			MPI_Recv(&a[j][0], m2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		}
		
		for (i = 0; i < rows; i++) {
			for (j = 0; j < m2; j++) {
				printf("%d) a[%d][%d] = %d, ", id, i, j, a[i][j]);
			}
			printf("\n");
		}
	}
	
	if (id == 0) {
		rows = (id < overflow) ? count + 1 : count;
	}
	
	for (i = 0; i < rows; i++) {
		for (j = 0; j < m2; j++) {
			if (a[i][j] % 2 == 0) {
				printf("Ispolnuva: %d) %d\n", id, a[i][j]);
			}
		}
	}
	
	for (i = 0; i < (id == 0 ? m1 : rows); i++) {
		free(a[i]);
	}
	free(a);
	
	MPI_Finalize();
}

