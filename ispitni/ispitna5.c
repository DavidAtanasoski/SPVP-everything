// Да се напише програма која ќе одреди дали во матрица
// постои силен број (силен број е број за кој сумата од 
// факториелите на неговите цифри е еднаква со самиот број). 
// Проверката треба да се подели рамномерно помеѓу сите процеси. 
// Притоа, броевите кои го исполнуваат условот во секој процес 
// треба да се пратат за печатење до нултиот процес.

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define STRONG_NUMBERS_COUNT 10

int factorial(int num)
{
    int result = 1;
    for (int i = 1; i <= num; i++)
    {
        result *= i;
    }
    return result;
}

int main(int argc, char **argv)
{
    int my_rank;
    int comm_sz;
    MPI_Status status;

    int source_rank = 0;

    int **matrix;
    int m1;
    int m2;

    int count;
    int overflow;
    int offset;
    int rows;

    int strong_numbers_local[STRONG_NUMBERS_COUNT];
    int strong_numbers_pos;

    int *strong_numbers_global;
    int *strong_numbers_global_count;
    int strong_numbers_pos_global = 0;

    int *recv_offset;
    int recv_total = 0;

    MPI_Init(&argc, &argv);
    
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == source_rank)
    {
        int ret = scanf("%d %d", &m1, &m2);

        matrix = (int **)malloc(m1 * sizeof(int *));
        for (int i = 0; i < m1; i++)
        {
            matrix[i] = (int *)malloc(m2 * sizeof(int));
        }

        printf("Matrix:\n");
        for (int i = 0; i < m1; i++)
        {
            for (int j = 0; j < m2; j++)
            {
                matrix[i][j] = rand() % 146 + 1;
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
            MPI_Send(&rows, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            
            for (int j = 0; j < rows; j++)
            {
                MPI_Send(&matrix[offset + j][0], m2, MPI_INT, i, 1, MPI_COMM_WORLD);
            }

            offset += rows;
        }
    }

    if (my_rank > source_rank)
    {
        MPI_Recv(&m2, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&offset, 1, MPI_INT, source_rank, 1, MPI_COMM_WORLD, &status);
    
        matrix = (int **) malloc(rows * sizeof(int *));
        for (int i = 0; i < rows; i++)
        {
            matrix[i] = (int *) malloc(m2 * sizeof(int *));
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

    strong_numbers_pos = 0;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < m2; j++)
        {
            int temp_num = matrix[i][j];
            int sum = 0;

            while (temp_num != 0)
            {
                int digit = temp_num % 10;
                sum += factorial(digit);
                temp_num /= 10;
            }

            if (sum == matrix[i][j])
            {
                // add to array and return it to source process
                strong_numbers_local[strong_numbers_pos] = matrix[i][j];
                strong_numbers_pos++;
            }
        }
    }

    if (source_rank == my_rank)
    {
        // koj process kolku jaki broevi ima najdeno
        strong_numbers_global_count = (int *)malloc(comm_sz * sizeof(int));
    }

    MPI_Gather(&strong_numbers_pos, 1, MPI_INT, strong_numbers_global_count, 1, MPI_INT, source_rank, MPI_COMM_WORLD);

    if (source_rank == my_rank)
    {
        recv_offset = (int *) malloc(comm_sz * sizeof(int));
        recv_offset[0] = 0;

        for (int i = 1; i < comm_sz; i++)
        {
            recv_offset[i] = recv_offset[i-1] + strong_numbers_global_count[i - 1]; 
            recv_total += strong_numbers_global_count[i-1];
        }

        recv_total += strong_numbers_global_count[comm_sz - 1];
        strong_numbers_global = (int *) malloc(recv_total * sizeof(int));
    }

    MPI_Gatherv(strong_numbers_local, strong_numbers_pos, MPI_INT, strong_numbers_global, strong_numbers_global_count, recv_offset, MPI_INT, source_rank, MPI_COMM_WORLD);

    if (source_rank == my_rank)
    {
        for (int i = 0; i < recv_total; i++)
        {
            printf("strong number: %d\n", strong_numbers_global[i]);
        }
    }

    MPI_Finalize();

    return 0;
}