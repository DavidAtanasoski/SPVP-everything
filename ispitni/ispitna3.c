// Да се напише програма која за дадена низа од реални броеви 
// ќе ја пресмета најголемата разлика помеѓу два соседни елементи 
// од низата. Задачата да се подели на сите процеси.

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int comm_sz;
    int my_rank;

    int source_rank = 0;

    int *array;
    int *counts;
    int *offset;

    int array_elements;

    int *array_local;
    int counts_local;

    int local_min_distance = __INT_MAX__;
    int global_min_distance;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == source_rank)
    {
        int ret = scanf("%d", &array_elements);
        array = (int *)malloc(array_elements * sizeof(int));

        printf("array: ");
        for (int i = 0; i < array_elements; i++)
        {
            array[i] = rand() % 20 + 1;
            printf("%d ", array[i]);
        }
        printf("\n");

        counts = (int *)malloc(comm_sz * sizeof(int));
        offset = (int *)malloc(comm_sz * sizeof(int));

        for (int i = 0; i < comm_sz; i++)
        {
            counts[i] = array_elements / comm_sz;
        }

        for (int i = 0; i < array_elements % comm_sz; i++)
        {
            counts[i]++;
        }

        offset[0] = 0;
        for (int i = 1; i < comm_sz + 1; i++)
        {
            offset[i] = offset[i - 1] + counts[i - 1] - 1; // -1 radi overlap da ima
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

    int temp_distance = 0;
    for (int i = 0; i < counts_local; i++)
    {
        if (i + 1 < counts_local) 
        {
            temp_distance = abs(array_local[i] - array_local[i+1]);
        }

        if (temp_distance < local_min_distance)
        {
            local_min_distance = temp_distance;
        }
    }

    MPI_Reduce(&local_min_distance, &global_min_distance, 1, MPI_INT, MPI_MIN, source_rank, MPI_COMM_WORLD);

    if (source_rank == my_rank)
    {
        printf("Min distance: %d\n", global_min_distance);
    }

    MPI_Finalize();

    return 0;
}