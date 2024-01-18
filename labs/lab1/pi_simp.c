#include <stdio.h>
#include <mpi.h>

#define n 5000000

double f(int i)
{
	double x;
	x = (double) i / (double) n;
	return 4.0 / (1.0 + x*x);
}


int main(int argc, char *argv[]) 
{
    MPI_Init(&argc, &argv);

	int p_id;
	int p_number;

	int root_p = 0;
	double elapsed_time;

	double global_area;

    MPI_Comm_rank(MPI_COMM_WORLD, &p_id);
    MPI_Comm_size(MPI_COMM_WORLD, &p_number);

	elapsed_time -= MPI_Wtime();

	double local_area = 0.0;

	int interval = (n / 2) / p_number;
	int start_interval = interval * p_id;
	int end_interval = (p_id == p_number - 1) ? (n / 2) : start_interval + interval;

	local_area += f(start_interval * 2) - f(end_interval * 2);

	for (int i = start_interval + 1; i <= end_interval; i++)
	{
		local_area += 4.0 * f(2 * i - 1) + 2 * f(2 * i);
	}

    local_area /= (3.0 * n);

	// printf("process: %d local area: %f\n", p_id, local_area);

	MPI_Reduce(&local_area, &global_area, 1, MPI_DOUBLE, MPI_SUM, root_p, MPI_COMM_WORLD);

	elapsed_time += MPI_Wtime();

	if (p_id == root_p)
	{
		printf("Approximation of pi: %13.11f\n", global_area);
		printf("Elapsed time: %f\n", elapsed_time);
	}

    MPI_Finalize();

    return 0;
}


// table:

//	No. processes	Elapsed time
//		1				0.000006				
//		2				0.000172
//		4				0.000201
//		16				0.000237				