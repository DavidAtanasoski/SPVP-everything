#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

/* 6. Да се напише програма која ќе одреди дали во матрица постои единечна подматрица со
димензии nxn (при што n се внесува од тастатура). Секој процес треба да врати информација
до главниот процес за тоа колку подматрици кои го исполнуваат условот се пронајдени.
Главниот процес треба да го прикаже вкупниот број на пронајдени матрици. */

#define num_threads 1

int **matrix;
int m1, m2, n;
int *displ;
// int global_num_matrices = 0;

void *work(void *ptr);

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char const *argv[])
{
    pthread_t threads[num_threads];
    int i, j, ret;
    int rows_per_thread, overflow;
    int num_identity_matrices = 0;
    // scanf("%d %d", &m1, &m2);
    // scanf("%d", &n);

    m1 = m2 = 6;
    n = 5;

    matrix = (int **)malloc(m1 * sizeof(int *));
    displ = (int *)malloc((num_threads + 1) * sizeof(int));

    printf("Original matrix:\n");
    for (i = 0; i < m1; i++)
    {
        matrix[i] = (int *)malloc(m2 * sizeof(int));
        for (j = 0; j < m2; j++)
        {
            if (i == j)
            {
                matrix[i][j] = 1;
            }
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    displ[0] = 0;
    rows_per_thread = m1 / num_threads;
    overflow = m1 % num_threads;
    for (i = 0; i < num_threads; i++)
    {
        if (i < overflow)
        {
            displ[i + 1] = rows_per_thread + displ[i] + 1;
        }
        else
        {
            displ[i + 1] = rows_per_thread + displ[i];
        }
    }

    for (i = 0; i < num_threads; i++)
    {
        ret = pthread_create(&threads[i], NULL, work, (void *)i);
        if (ret)
        {
            printf("Error\n");
            return -1;
        }
    }

    int *help_count = 0;
    for (i = 0; i < num_threads; i++)
    {
        // pthread_join(threads[i], NULL);
        pthread_join(threads[i], (void **)&help_count);
        num_identity_matrices += *help_count;
        free(help_count);
    }
    
    printf("Matrices found: %d\n", num_identity_matrices);
    free(displ);

    return 0;
}

void *work(void *ptr)
{
    int id = (int *)ptr;
    int i, j, k, z;
    // int remainder_rows = displ[i + 1] - displ[i] - 1;
    int remainder_rows = n - 1;
    int num_local_matrices = 0;
    bool check = true;

    for (i = displ[id]; i + n <= displ[id + 1] + remainder_rows && i + n <= m1 ; i++)
    {
        for (j = 0; j + n <= m2; j++)
        {
            check = true;
            for (k = 0; k < n; k++)
            {
                for (z = 0; z < n; z++)
                {
                    if (k == z)
                    {
                        if (matrix[i + k][j + z] != 1)
                            check = false;
                    }
                    else if (matrix[i + k][j + z] != 0)
                        check = false;
                }
            }
            if (check)
            {
                printf("Id %d found@ %d,%d\n", id, i, j);
                // pthread_mutex_lock(&mutex);
                // global_num_matrices++;
                // pthread_mutex_unlock(&mutex);
                num_local_matrices++;
            }
        }
    }

    int *res_ptr = malloc(sizeof(int));
	*res_ptr = num_local_matrices;
    // printf("Id %d local matrices found: %d\n", id, num_local_matrices);
    pthread_exit(res_ptr);
}