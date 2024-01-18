#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 21_mpi 21_mpi.c
  Execute: mpirun -np 4 21_mpi
*/

int main(int argc, char *argv[]) {
	int p, id, i, j, k, t;
	int m1 = 6, m2 = 4, m3 = 3;
	int count, overflow, offset, rows;
	int a[m1][m2], b[m2][m3], c[m1][m3];
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (id == 0) {
		for (i = 0; i < m1; i++) {
			for (j = 0; j < m2; j++) {
				a[i][j] = rand() % 10;
				printf("a[%d][%d] = %d, ", i, j, a[i][j]);
			}
			printf("\n");
		}
		for (i = 0; i < m2; i++) {
			for (j = 0; j < m3; j++) {
				b[i][j] = rand() % 10;
				printf("b[%d][%d] = %d, ", i, j, b[i][j]);
			}
			printf("\n");
		}

		count = m1 / p;
		overflow = m1 % p;
		offset = (id < overflow) ? count + 1 : count;

		for (i = 1; i < p; i++) {
			rows = (i < overflow) ? count + 1 : count; 
			MPI_Send(&m2, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&m3, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&rows, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&a[offset][0], rows * m2, MPI_INT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&b, m2 * m3, MPI_INT, i, 1, MPI_COMM_WORLD);

			offset += rows; 
		}
	}

	if (id > 0) {
		MPI_Recv(&m2, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&m3, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&rows, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&a, rows * m2, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		MPI_Recv(&b, m2 * m3, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
	}
	
	if (id == 0) {
		rows = (id < overflow) ? count + 1 : count;
	}


	for (i = 0; i < rows; i++) {
		for (j = 0; j < m2; j++) {
			printf("%d) a[%d][%d] = %d, ", id, i, j, a[i][j]);
		}
		printf("\n");
	}
	
	for(i = 0; i < m3; i++) {
		for(j = 0; j < rows; j++) {
			c[j][i] = 0.0;
			for(k = 0; k < m2; k++) {
				c[j][i] = c[j][i] + a[j][k] * b[k][i];
			}
		}
	}

	if (id > 0) {
		MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
		MPI_Send(&c, rows * m3, MPI_INT, 0, 2, MPI_COMM_WORLD);
	} else {

		// source processot gi sobira site
		for(i = 1; i < p; i++) {
			MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&rows, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
			MPI_Recv(&c[offset][0], rows * m3, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
		}

		for(i = 0; i < m1; i++) {
			printf("\n");
			for(j = 0; j < m3; j++) {
				printf("%6d ", c[i][j]);
			}
			printf("\n");
		}
	}
	
	MPI_Finalize();
}

