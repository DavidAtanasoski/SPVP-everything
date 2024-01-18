#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define INTERVALS 1000000

int main (int argc, char *argv[])
{
	int p_id;
	int p_number;

	double area;
	double global_ysum;
	double xi;
	int i;
	int root_p = 0;

	double elapsed_time;
	
	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &p_number);
	MPI_Comm_rank(MPI_COMM_WORLD, &p_id);

	elapsed_time -= MPI_Wtime();

	double local_ysum = 0.0;
	
	for (i = p_id; i < INTERVALS; i += p_number)
	{
		xi = (1.0/INTERVALS)*(i+0.5);
		local_ysum += 4.0/(1.0+xi*xi);
	}

	//printf("process: %d sum: %f\n", p_id, local_ysum);

	MPI_Reduce(&local_ysum, &global_ysum, 1, MPI_DOUBLE, MPI_SUM, root_p, MPI_COMM_WORLD);
	
	elapsed_time += MPI_Wtime();

	if (p_id == root_p)
	{
		area = global_ysum * (1.0/INTERVALS);
		printf("Area is %13.11f\n", area);
		printf("Elapsed time: %f\n", elapsed_time);
	}

	MPI_Finalize();

	return 0;
}

// table:

//	No. processes	Elapsed time
//		1				0.008492				
//		2				0.004540
//		4				0.002284
//		16				0.001307