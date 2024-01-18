/*
    Vtor kolokvium, razlika na 2 sosedi pogolema od X
*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

typedef struct {
    int thread_id;
    int n_rows;
    int m_columns;
    double x;
} ThreadInfo;

#define NUM_THREADS 4

void *work(void *);

double **matrix;

int main()
{
    srand(time(NULL));

    int n_rows;
    int m_columns;
    double x;

    pthread_t threads[NUM_THREADS];
    ThreadInfo args[NUM_THREADS];

    scanf("%d %d", &n_rows, &m_columns);

    matrix = (double **) malloc(n_rows * sizeof(double *));
    for (int i = 0; i < n_rows; i++)
    {
        matrix[i] = (double *) malloc(m_columns * sizeof(double));
    }

    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            matrix[i][j] = ((double)rand() / RAND_MAX) * 100;
            printf("%.2f ", matrix[i][j]);
        }
        printf("\n");
    }
    
    scanf("%lf", &x);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        args[i].n_rows = n_rows;
        args[i].m_columns = m_columns;
        args[i].thread_id = i;
        args[i].x = x;

        int ret = pthread_create(&threads[i], NULL, work, &args[i]);

        if (ret)
        {
            printf("Error\n");
            return -1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

void *work(void *ptr)
{
    ThreadInfo *args = ptr;

    int rows_per_thread = args->n_rows / NUM_THREADS;
    int remainder_rows = args->n_rows % NUM_THREADS;

    int start = args->thread_id * rows_per_thread + (args->thread_id < remainder_rows ? args->thread_id : remainder_rows);
    int end = start + rows_per_thread + (args->thread_id < remainder_rows ? 1 : 0);

    for (int i = start; i < end; i++)
    {
        printf("redica: %d, ", i);
        for (int j = 0; j < args->m_columns; j++)
        {
            int result = abs(matrix[i][j] - matrix[i][j + 1]);

            if (result > args->x)
            {
                printf("kolona: %d", args->thread_id, i, j);
            }
        }
    }

    pthread_exit(NULL);
}