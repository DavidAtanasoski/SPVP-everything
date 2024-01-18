// Да се напише програма која ќе одреди дали во матрица постои единечна 
// подматрица со димензии nxn (при што n се внесува од тастатура). 
// Секој процес треба да врати информација до главниот процес за 
// тоа колку подматрици кои го исполнуваат условот се пронајдени. 
// Главниот процес треба да го прикаже вкупниот број на пронајдени матрици.

// not finished!

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    MPI_Status status;

    int my_rank;
    int comm_sz;

    int source_rank = 0;

    int **matrix;
    int m1;
    int m2;
    int n;

    int count;
    int offset;
    int overflow;
    int rows;
    
    int **submatrix;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == source_rank)
    {
        int ret = scanf("%d %d %d", &m1, &m2, &n);

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
                if (i == j)
                {
                    matrix[i][j] = 1;
                }
                else 
                {
                    matrix[i][j] = 0;
                }
                printf("%3d ", matrix[i][j]);
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
            MPI_Send(&rows, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);

            for (int j = 0; j < rows; j++)
            {
                MPI_Send(&matrix[j + offset][0], m2, MPI_INT, i, 1, MPI_COMM_WORLD);
            }

            offset += rows;
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, source_rank, MPI_COMM_WORLD);

    if (my_rank > source_rank)
    {
        MPI_Recv(&m2, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);

        matrix = (int **) malloc(rows * sizeof(int *));
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

    printf("process: %d n: %d\n", my_rank, n);
    for (int i = 0; i <= rows - n; i++)
    {
        for (int j = 0; j <= m2 - n; j++)
        {
            int checkDiagonal = 0;

            for (int k = 0; k < n; k++)
            {
                for (int z = 0; z < n; z++)
                {
                    // if (i == j && matrix[i + k][j + z] == 1)
                    printf("%3d ", matrix[i+k][j+z]);
                }
                printf("\n");
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return 0;
}