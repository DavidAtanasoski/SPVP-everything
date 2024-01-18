#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define STRINGS_NUMBER 3
#define MAX_STRING_LENGTH 100
#define MAX_POSITION_COUNT 100

int main(int argc, char** argv)
{
    int my_rank;
    int comm_sz;
    int source_rank = 0;

    int *counts;
    int *offset;

    char strings[STRINGS_NUMBER][MAX_STRING_LENGTH];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int count_local;
    int offset_local;
    int ret;

    char *buffer = 0;
    char *buffer_local;

    long length;
    
    if (my_rank == source_rank)
    {
        printf("Insert 3 strings: \n");
        for (int i = 0; i < STRINGS_NUMBER; i++)
        {
            ret = scanf("%s", strings[i]);
        }

        FILE* file = fopen("tekst.txt", "rb");

        if (file)
        {
            fseek(file, 0, SEEK_END);
            length = ftell(file);
            fseek(file, 0, SEEK_SET);
            buffer = malloc(length);

            if (buffer)
            {
                fread(buffer, 1, length, file);
            }
            fclose(file);
        }

        counts = (int *) malloc(comm_sz * sizeof(int));
        offset = (int *) malloc(comm_sz * sizeof(int));

        for (int i = 0; i < comm_sz; i++)
        {
            counts[i] = length / comm_sz;
        }

        for (int i = 0; i < length % comm_sz; i++)
        {
            counts[i]++;
        }

        offset[0] = 0;
        for (int i = 1; i < comm_sz; i++)
        {
            offset[i] = offset[i - 1] + counts[i - 1];
        }
    }

    for (int i = 0; i < STRINGS_NUMBER; i++)
    {
        MPI_Bcast(&strings[i], MAX_STRING_LENGTH, MPI_CHAR, source_rank, MPI_COMM_WORLD);
    }

    MPI_Scatter(counts, 1, MPI_INT, &count_local, 1, MPI_INT, source_rank, MPI_COMM_WORLD);
    buffer_local = (char *) malloc((count_local + 1) * sizeof(char));
    MPI_Scatterv(buffer, counts, offset, MPI_CHAR, buffer_local, count_local, MPI_CHAR, source_rank, MPI_COMM_WORLD);

    buffer_local[count_local] = '\0';

    MPI_Scatter(offset, 1, MPI_INT, &offset_local, 1, MPI_INT, source_rank, MPI_COMM_WORLD);
    
    int localPositions[STRINGS_NUMBER][MAX_POSITION_COUNT];
    int locPosCount[STRINGS_NUMBER] = {0};
    for (int i = 0; i < STRINGS_NUMBER; i++) {
        int buffLength = strlen(buffer_local);
        int searchLength = strlen(strings[i]);

        for (int j = 0; j <= buffLength - searchLength; ++j) {
            if (strncmp(buffer_local + j, strings[i], searchLength) == 0) {
                localPositions[i][locPosCount[i]] = j + offset_local;
                // printf("String %d is on pos: %d\n", i, localPositions[i][locPosCount[i]]);
                locPosCount[i]++;
            }
        }
    }

    int *gatheredLocalPositions = NULL;
    int *gatheredLocPosCount = NULL;

    if (my_rank == source_rank) {
        gatheredLocalPositions = (int *)malloc(STRINGS_NUMBER * MAX_POSITION_COUNT * comm_sz * sizeof(int));
        gatheredLocPosCount = (int *)malloc(STRINGS_NUMBER * comm_sz * sizeof(int));
    }

    MPI_Gather(locPosCount, STRINGS_NUMBER, MPI_INT, gatheredLocPosCount, STRINGS_NUMBER, MPI_INT, source_rank, MPI_COMM_WORLD);

    for (int i = 0; i < STRINGS_NUMBER; i++)
    {
        for (int j = 0; j < locPosCount[i]; j++)
        {
            printf("Process: %d found string: %s at %d \n", my_rank, strings[i], localPositions[i][j]);
        }
    }

    MPI_Finalize();

    return 0;
}