/*
    Да се напише програма која ќе одреди дали во матрица постои 
    единечна подматрица со димензии nxn (при што n се внесува од тастатура). 
    Секој процес треба да врати информација до главниот процес за тоа 
    колку подматрици кои го исполнуваат условот се пронајдени. Главниот 
    процес треба да го прикаже вкупниот број на пронајдени матрици.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#define NUM_THREADS 1


typedef struct {
    int **matrix;
    int thread_id;
    int n_rows;
    int n_cols;
    int check;
    int count;
} ThreadInfo;

int matrix_rows;
int matrix_columns;
int n;

int **matrix;

void *checkForIdentityMatrix(void *);

int main()
{
    srand(time(NULL));

    scanf("%d %d %d", &matrix_rows, &matrix_columns, &n);

    matrix = (int **) malloc(matrix_rows * sizeof(int *));

    for (int i = 0; i < matrix_rows; i++)
    {
        matrix[i] = (int *) malloc(matrix_columns * sizeof(int));
    }

    for (int i = 0; i < matrix_rows; i++)
    {
        for (int j = 0; j < matrix_columns; j++)
        {
            matrix[i][j] = rand() % 2;
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    pthread_t threads[NUM_THREADS];
    ThreadInfo args[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        args[i].matrix = matrix;

        args[i].thread_id = i;
        args[i].n_rows = matrix_rows;
        args[i].n_cols = matrix_columns;
        args[i].check = 1;
        args[i].count = 0;

        int ret = pthread_create(&threads[i], NULL, checkForIdentityMatrix, &args[i]);

        if (ret)
        {
            printf("ERROR\n");
            return -1;
        }
    }

    for (int j = 0; j < NUM_THREADS; j++)
    {
        pthread_join(threads[j], NULL);
    }

    int total_count = 0;
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // printf("%d ", args[i].count);
        total_count += args[i].count;
    }
    
    printf("Total: %d\n", total_count);

    return 0;
}

void *checkForIdentityMatrix(void *ptr)
{
    ThreadInfo *args = ptr;

    int rows_per_thread = args->n_rows / NUM_THREADS;
    int remainder_rows = args->n_rows % NUM_THREADS;

    int start = args->thread_id * rows_per_thread + (args->thread_id < remainder_rows ? args->thread_id : remainder_rows);
    int end = start + rows_per_thread + (args->thread_id < remainder_rows ? 1 : 0);

    for (int i = start; i <= end - n; i++)
    {
        for (int j = 0; j < args->n_cols - n + 1; j++)
        {
            int local_check = 1;
            
            //printf("Thread %d: Checking submatrix starting at (%d, %d)\n", args->thread_id, i, j);

            for (int sub_i = 0; sub_i < n; sub_i++)
            {
                for (int sub_j = 0; sub_j < n; sub_j++)
                {
                    if (i + sub_i < args->n_rows && j + sub_j < args->n_cols)
                    {
                        //printf("Thread %d: Checking element (%d, %d) with value %d\n", args->thread_id, i + sub_i, j + sub_j, matrix[i + sub_i][j + sub_j]);
                        
                        if (sub_i == sub_j)
                        {
                            if (matrix[i + sub_i][j + sub_j] != 1)
                            {
                                local_check = 0;
                                break;
                            }
                        }
                        else
                        {
                            if (matrix[i + sub_i][j + sub_j] != 0)
                            {
                                local_check = 0;
                                break;
                            }
                        }
                    }
                    else
                    {
                        local_check = 0;
                        break;
                    }
                }

                if (local_check == 0)
                {
                    break;
                }
            }

            if (local_check == 1)
            {
                args->count++;
            }
        }
    }

    pthread_exit(NULL);
}
