// Да се напише MPI програма во која од тастатура се вчитува низа од цели
// броеви и се проверува дали низата е растечка или опаѓачка. Доколку низата не е
// ниту растечка, ниту опаѓачка, процесите не треба да продолжат со работа. Но,
// доколку се утврди дека низата е или растечка или опаѓачка, процесите треба истата
// да ја конвертираат во строго растечка/опаѓачка (односно, да ги отстранат сите
// елементи кои се еднакви на нивниот претходник).
// Во процесот со id=0 треба да се вчита низата од тастатура, и на крај да се испечати
// низата. Работата да се распредели рамномерно помеѓу повеќе процеси.

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int my_rank;
    int comm_sz;

    int source_rank = 0;

    int *array;
    int *counts;
    int *offset;

    int *array_local;
    int counts_local;
    
    int num_elements;

    int *array_filtered_local;
    int array_filtered_local_pos = 0;

    int *array_filtered_counts;
    int *recv_offset;
    int recv_counts = 0;

    int *array_global;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if (my_rank == source_rank)
    {
        int ret = scanf("%d", &num_elements);
        array = (int *) malloc(num_elements * sizeof(int));

        for (int i = 0; i < num_elements; i++)
        {
            array[i] = i+1;//rand() % 50 + 1;
        }

        for (int i = 0; i < num_elements - 1; i++)
        {
            if (!(array[i] >= array[i+1]) && !(array[i] <= array[i+1]))
            {
                printf("Nikakva e nizata\n");
                MPI_Abort(MPI_COMM_WORLD, 404);
            }
        }

        counts = (int *) malloc(comm_sz * sizeof(int));
        offset = (int *) malloc(comm_sz * sizeof(int));

        for (int i = 0; i < comm_sz; i++)
        {
            counts[i] = num_elements / comm_sz;
        }

        for (int i = 0; i < num_elements % comm_sz; i++)
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
    array_local = (int *) malloc(counts_local * sizeof(int));
    MPI_Scatterv(array, counts, offset, MPI_INT, array_local, counts_local, MPI_INT, source_rank, MPI_COMM_WORLD);

    array_filtered_local = (int *) malloc(counts_local * sizeof(int));
    array_filtered_local_pos = 0;
    for (int i = 0; i < counts_local - 1; i++)
    {
        if (array_local[i] != array_local[i+1])
        {
            array_filtered_local[array_filtered_local_pos] = array_local[i];
            array_filtered_local_pos++;
        }
    }

    if (source_rank == my_rank)
    {
        array_filtered_counts = (int *) malloc(array_filtered_local_pos * sizeof(int));
    }

    MPI_Gather(&array_filtered_local_pos, 1, MPI_INT, array_filtered_counts, 1, MPI_INT, source_rank, MPI_COMM_WORLD);

    if (source_rank == my_rank)
    {
        recv_offset = (int *) malloc(comm_sz * sizeof(int));
        recv_offset[0] = 0;

        for (int i = 1; i < comm_sz; i++)
        {
            recv_offset[i] = recv_offset[i - 1] + array_filtered_counts[i - 1];
            recv_counts += array_filtered_counts[i-1];
        }

        recv_counts += array_filtered_counts[comm_sz - 1];
        array_global = (int *) malloc(recv_counts * sizeof(int));
    }

    MPI_Gatherv(array_filtered_local, array_filtered_local_pos, MPI_INT, array_global, array_filtered_counts, recv_offset, MPI_INT, source_rank, MPI_COMM_WORLD);

    for (int i = 0; i < recv_counts; i++)
    {
        printf("%d ", array_global[i]);
    }
    
    MPI_Finalize();

    return 0;
}