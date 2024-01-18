#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int sendbuf[8] = {0, 1, 2, 3, 4, 5, 6, 7}; // Data to be scattered
    int recvbuf[3]; // Variable to hold received data

    if (size < 2) {
        printf("This example requires at least two processes.\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        printf("Sending data: ");
        for (int i = 0; i < 8; i++) {
            printf("%d ", sendbuf[i]);
        }
        printf("\n");
    }

    MPI_Scatter(sendbuf, 3, MPI_INT, recvbuf, 3, MPI_INT, 0, MPI_COMM_WORLD);

    printf("Process %d received data: ", rank);
    for (int i = 0; i < 3; i++) 
    {
        printf("received %d", recvbuf[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}
