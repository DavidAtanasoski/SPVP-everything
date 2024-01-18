// Да се напише програма во која се пополнуваат елементи од низа. 
// Од низата треба да се отфрлат непарните елементи и променетата низа 
// да се испечати на екран. 
// Задачата да се распредели рамномерно помеѓу повеќе процеси.

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int my_rank;
    int comm_sz;

    int source_rank = 0;
    
    int elements_count;

    int *array;
    int *counts;
    int *offset;
    int *recv_offset;

    int counts_local;
    int recv_total_elements = 0;

    int *array_local;
    int *even_elements_local;
    int *even_elements_global;
    int *even_elements_count;

    
    int even_elements_pos;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == source_rank)
    {
        int ret = scanf("%d", &elements_count);
        array = (int *)malloc(elements_count * sizeof(int));

        printf("array: ");
        for (int i = 0; i < elements_count; i++)
        {
            array[i] = rand() % 20 + 1;
            printf("%d ", array[i]);
        }
        printf("\n");

        counts = (int *)malloc(comm_sz * sizeof(int));
        offset = (int *)malloc(comm_sz * sizeof(int));

        for (int i = 0; i < comm_sz; i++)
        {
            counts[i] = elements_count / comm_sz;
        }

        for (int i = 0; i < elements_count % comm_sz; i++)
        {
            counts[i]++;
        }

        offset[0] = 0;
        for (int i = 1; i < comm_sz; i++)
        {
            offset[i] = offset[i - 1] + counts[i - 1];
        }
    }

    MPI_Scatter(counts, 1, MPI_INT, &counts_local, 1, MPI_INT, source_rank, MPI_COMM_WORLD);
    array_local = (int *)malloc(counts_local * sizeof(int));
    MPI_Scatterv(array, counts, offset, MPI_INT, array_local, counts_local, MPI_INT, source_rank, MPI_COMM_WORLD);

    even_elements_local = (int *)malloc(counts_local * sizeof(int));
    even_elements_pos = 0;
    for (int i = 0; i < counts_local; i++)
    {
        if (array_local[i] % 2 == 0)
        {
            even_elements_local[even_elements_pos] = array_local[i];
            even_elements_pos++;
        }
    }

    if (my_rank == source_rank)
    {
        // koj proces kolku parni ima najdeno
        even_elements_count = (int *)malloc(comm_sz * sizeof(int));
    }

    MPI_Gather(&even_elements_pos, 1, MPI_INT, even_elements_count, 1, MPI_INT, source_rank, MPI_COMM_WORLD);

    if (my_rank == source_rank)
    {
        recv_offset = (int *)malloc(comm_sz * sizeof(int));
        recv_offset[0] = 0;

        for (int i = 1; i < comm_sz; i++)
        {
            recv_offset[i] = recv_offset[i-1] + even_elements_count[i-1];
            recv_total_elements += even_elements_count[i-1];
        }

        recv_total_elements += even_elements_count[comm_sz-1];
        even_elements_global = (int *)malloc(recv_total_elements * sizeof(int));
    }

    MPI_Gatherv(even_elements_local, even_elements_pos, MPI_INT, even_elements_global, even_elements_count, recv_offset, MPI_INT, source_rank, MPI_COMM_WORLD);

    for (int i =  0; i < recv_total_elements; i++)
    {
        printf("%d ", even_elements_global[i]);
    }

    MPI_Finalize();

    return 0;
}