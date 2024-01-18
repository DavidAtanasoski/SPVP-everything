// Да се напише програма која ќе ја промени содржината на одредена матрица, 
// така што секој парен елемент ќе биде заменет со нула. 
// Задачата да се подели рамномерно помеѓу сите процеси. 
// Содржината на променетата матрица треба да се испечати во главниот процес.

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Status status;

    int comm_sz;
    int my_rank;

    int source_rank = 0;

    int m1;
    int m2;

    int **matrix;

    int count;
    int overflow;
    int offset;
    int rows;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == source_rank)
    {
        int ret = scanf("%d %d", &m1, &m2);

        matrix = (int **) malloc(m1 * sizeof(int *));
        for (int i = 0; i < m1; i++)
        {
            matrix[i] = (int *) malloc(m2 * sizeof(int));
        }

        printf("Matrix: \n");
        for (int i = 0; i < m1; i++)
        {
            for (int j = 0; j < m2; j++)
            {
                matrix[i][j] = rand() % 25 + 1;
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }

        count = m1 / comm_sz;
        overflow = m1 % comm_sz;
        offset = (my_rank < overflow) ? count + 1 : count;

        for (int i = 1; i < comm_sz; i++)
        {
            rows = (i < overflow) ? count + 1 : count;

            MPI_Send(&m2, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, i, 1, MPI_COMM_WORLD);

            for (int j = 0; j < rows; j++)
            {
                MPI_Send(&matrix[j + offset][0], m2, MPI_INT, i, 1, MPI_COMM_WORLD);
            }

            offset += rows;
        }
    }

    if (my_rank > source_rank)
    {
        MPI_Recv(&m2, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);

        matrix = (int **) malloc(rows * sizeof(int));
        for (int i = 0; i < rows; i++)
        {
            matrix[i] = (int *) malloc(m2 * sizeof(int));
        }

        for (int i = 0; i < rows; i++)
        {
            MPI_Recv(&matrix[i][0], m2, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        }
    }

    if (my_rank == source_rank)
    {
        rows = (my_rank < overflow) ? count + 1 : count;
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            if (matrix[i][j] % 2 == 0)
            {
                matrix[i][j] = 0;
            }
        }
    }


    if (my_rank > source_rank)
    {
        MPI_Send(&offset, 1, MPI_INT, source_rank, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, source_rank, 2, MPI_COMM_WORLD);
        MPI_Send(&matrix[0][0], rows * m2, MPI_INT, source_rank, 2, MPI_COMM_WORLD);
    }
    else
    {
        for (int i = 1; i < comm_sz; i++)
        {
            MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&matrix[offset][0], rows * m2, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
        }

        printf("Result matrix: \n");
        for (int i = 0; i < m1; i++)
        {
            for (int j = 0; j < m2; j++)
            {
                printf("%6d ", matrix[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}