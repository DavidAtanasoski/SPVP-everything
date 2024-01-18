#include <stdio.h>
#include <stdlib.h>
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

    int *global_results;
    int *global_result;
    int *local_results;

    int local_pos = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int counts_local;
    int recv_total = 0;

    int *array_local;
    int *recv_offset;
    

    if (my_rank == source_rank)
    {
        int ret = scanf("%d", &elements_count);
        array = (int *)malloc(elements_count * sizeof(int));

        for (int i = 0; i < elements_count; i++)
        {
            array[i] = rand() % 20;
            printf("%d ", array[i]);
        }
        printf(" end array\n");

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

    printf("Process %d got: ", my_rank);
    for (int i = 0; i < counts_local; i++)
    {
        printf("%d ", array_local[i]);
    }
    printf("\n");

    local_results = (int *)malloc(counts_local * sizeof(int));
    for (int i = 0; i < counts_local; i++)
    {
        if (array_local[i] % 2 == 0)
        {
            local_results[local_pos] = array_local[i]; // elementi koi go ispolnuvaat uslovot
            local_pos++;
        }
    }  

    if (source_rank == my_rank)
    {
        // global result e kolku parni ima sekoj proces najdeno
        global_result = (int *)malloc(comm_sz * sizeof(int));
    }

    MPI_Gather(&local_pos, 1, MPI_INT, global_result, 1, MPI_INT, source_rank, MPI_COMM_WORLD);
    
    if (source_rank == my_rank)
    {
        recv_offset = (int *) malloc(comm_sz * sizeof(int));
        recv_offset[0] = 0;

        for (int i = 1; i < comm_sz; i++)
        {
            recv_offset[i] = recv_offset[i-1] + global_result[i-1];
            recv_total += global_result[i-1];
        }

        recv_total += global_result[comm_sz - 1];
        global_results = (int *) malloc(recv_total * sizeof(int));
    }

    MPI_Gatherv(local_results, local_pos, MPI_INT, global_results, global_result, recv_offset, MPI_INT, source_rank, MPI_COMM_WORLD);

    if (my_rank == source_rank)
    {
        printf("Broj na elementi shto ispolnuvaat po procesi e: \n");
        for (int i = 0; i < comm_sz; i++)
        {
            printf("process: %d elementi: %d \n", i, global_result[i]);
        }

        printf("Vkupno takvi elementi: %d \n", recv_total);

        for (int i = 0; i < recv_total; i++)
        {
            printf("process: %d elemnt: %d\n", i, global_results[i]);
        }

        printf("\n");
    }

    MPI_Finalize();

    return 0;
}