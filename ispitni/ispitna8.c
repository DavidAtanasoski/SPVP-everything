// Да се напише MPI програма која ќе ги бара целобројните решенија на
// равенката:
//     x^4 * z + 3 * y^2 * z - 4 * z = 20
// Работата да се распредели рамномерно помеѓу повеќе процеси. Да се испробаат сите
// можни комбинации на променливите x, y, z во опсегот [-32768, 32767].
// Најдените решенија да се вратат до главниот процес кој треба да ги прикаже на екран.

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define LOW_END -500
#define HIGH_END 500

#define VARIABLES 3
#define MAX_SOLUTIONS 100

int main(int argc, char **argv)
{
    int my_rank;
    int comm_sz;

    int source_rank = 0;

    int local_positions[MAX_SOLUTIONS];
    int local_pos_count = 0;

    int *solutions_global = NULL;
    int solutions_global_pos = 0;

    int *counts;
    int *offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int range_process = (HIGH_END - LOW_END + 1) / comm_sz;
    int start = my_rank * range_process + LOW_END;
    int end = (my_rank + 1) * range_process + LOW_END - 1;

    for (int x = start; x <= end; x++)
    {
        for (int y = LOW_END; y < HIGH_END; y++)
        {
            for (int z = LOW_END; z < HIGH_END; z++)
            {
                if ((x * x * x * x * z + 3 * y * y * z - 4 * z) == 20) 
                {
                    local_positions[local_pos_count++] = x;
                    local_positions[local_pos_count++] = y;
                    local_positions[local_pos_count++] = z;
                }
            }
        }
    }

    if (my_rank == source_rank)
    {
        solutions_global = (int *) malloc(3 * comm_sz * MAX_SOLUTIONS * sizeof(int));
        counts = (int *) malloc(comm_sz * sizeof(int));
        offset = (int *) malloc(comm_sz * sizeof(int));
    }

    MPI_Gather(&local_pos_count, 1, MPI_INT, counts, 1, MPI_INT, source_rank, MPI_COMM_WORLD);

    if (my_rank == source_rank)
    {
        offset[0] = 0;
        for (int i = 1; i < comm_sz; i++)
        {
            offset[i] = offset[i-1] + counts[i-1];
        }
    }

    MPI_Gatherv(local_positions, local_pos_count, MPI_INT, solutions_global, counts, offset, MPI_INT, source_rank, MPI_COMM_WORLD);

    if (source_rank == my_rank)
    {
        for (int i = 0; i < comm_sz * 1000; i++)
        {
            printf("x = %d y = %d z = %d\n", solutions_global[i], solutions_global[i+1], solutions_global[i+2]);
        }
    }

    MPI_Finalize();

    return 0;
}