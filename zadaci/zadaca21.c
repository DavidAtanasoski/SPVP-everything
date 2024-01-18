#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char **argv)
{
    int my_rank;
    int comm_sz;
    MPI_Status status;

    int source_rank = 0;

    int m1 = 6;
    int m2 = 4;
    int m3 = 3;

    // 6x4 = 4x3
    // => 6x3
    int matrixA[m1][m2];
    int matrixB[m2][m3];
    int matrixC[m1][m3];

    int count;
    int overflow;
    int offset;
    int rows;


    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == source_rank)
    {
        for (int i = 0; i < m1; i++)
        {
            for (int j = 0; j < m2; j++)
            {
                matrixA[i][j] = rand() % 10 + 1;
            }
        }

        for (int i = 0; i < m2; i++)
        {
            for (int j = 0; j < m3; j++)
            {
                matrixB[i][j] = rand() % 10 + 1;
            }
        }

        count = m1 / comm_sz;
        overflow = m1 % comm_sz;
        // offset na source process
        offset = (my_rank < overflow) ? count + 1 : count;

        for (int i = 1; i < comm_sz; i++)
        {
            // se apdejtira brojot na redici koj go dobiva sekoj proces
            rows = (i < overflow) ? count + 1 : count;

            MPI_Send(&m2, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&m3, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&matrixA[offset][0], rows * m2, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&matrixB, m2 * m3, MPI_INT, i, 1, MPI_COMM_WORLD);
            
            // se dodava brojot na redici na offset za da se znae do kade e!
            offset += rows;
        }
    }

    if (my_rank > source_rank)
    {
        MPI_Recv(&m2, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&m3, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrixA, rows * m2, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&matrixB, m2 * m3, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
    }

    if (source_rank == my_rank)
    {
        rows = (source_rank < overflow) ? count + 1 : count;
    }

    for (int i = 0; i < m3; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            matrixC[j][i] = 0.0;
            for (int k = 0; k < m2; k++)
            {
                matrixC[j][i] = matrixC[j][i] + matrixA[j][k] * matrixB[k][i];
            }
        }
    }

    if (my_rank > source_rank)
    {
        MPI_Send(&offset, 1, MPI_INT, source_rank, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, source_rank, 2, MPI_COMM_WORLD);
        MPI_Send(&matrixC, rows * m3, MPI_INT, source_rank, 2, MPI_COMM_WORLD);
    }
    else 
    {
        // source procesot gi sobira site
        for (int i = 1; i < comm_sz; i++)
        {
            MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&matrixC, rows * m3, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
        }

        for (int i = 0; i < m1; i++)
        {
            for (int j = 0; j < m3; j++)
            {
                printf("%6d ", matrixC[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}