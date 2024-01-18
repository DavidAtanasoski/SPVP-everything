#include <stdio.h>
#include <mpi.h>

#define n 50

double f (int i)
{
	double x;
	x = (double) i / (double) n;
	return 4.0 / (1.0 + x*x);
}

int main(int argc, char** argv)
{
    int my_rank;
    int comm_sz;
    double local_area;
    double global_area;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    

    MPI_Finalize();

    return 0;
}