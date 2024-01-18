#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

/*
  Compile: mpicc -O -o 8_mpi 8_mpi.c
  Execute: mpirun -np 4 8_mpi
*/

int main(int argc, char *argv[]) {
	int id, p;
	int i, count, global_count;
	int total_tasks = 65536;
	double elapsed_time;

	int check_circuit(int, int);

	MPI_Init(&argc, &argv);
		
	MPI_Comm_size(MPI_COMM_WORLD, &p); 
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	
	elapsed_time = -MPI_Wtime();

	count = 0;

	for (i = id * (total_tasks / p); i <= ((id != p-1) ? (id + 1) * (total_tasks / p) : total_tasks); i++) { 
		count += check_circuit(id, i);
	}

	MPI_Reduce(&count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	elapsed_time += MPI_Wtime();

	if (id == 0) {	
		printf("Brojot na reshenija e %d\n", global_count);
		printf("Vremeto na izvrshuvanje e %f\n", elapsed_time);
	}	

	MPI_Finalize();
}

#define EXTRACT_BIT(n, i) ((n&(1<<i))?1:0)

int check_circuit(int id, int z){
	int v[16];
	int i, res = 0;

	for (i = 0; i < 16; i++)
		v[i] = EXTRACT_BIT(z, i);
	
	if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
   	   	&& (!v[3] || !v[4]) && (v[4] || !v[5])
   	   	&& (v[5] || !v[6]) && (v[5] || v[6])
   	   	&& (v[6] || !v[15]) && (v[7] || !v[8])
   	   	&& (!v[7] || !v[13]) && (v[8] || v[9])
   	   	&& (v[8] || !v[9]) && (!v[9] || !v[10])
   	   	&& (v[9] || v[11]) && (v[10] || v[11])
   	   	&& (v[12] || v[13]) && (v[13] || !v[14])
   	   	&& (v[14] || v[15])) 
	{
		res++;
		printf ("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", id,
			v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],
			v[10],v[11],v[12],v[13],v[14],v[15]);
		fflush(stdout);

	}

	return res;
}
