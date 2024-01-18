/*
Да се напише програма во која во нулта нитка ќе се прочита 
квадратна матрица со димензии n x n, каде n исто така се внесува 
од тастатура. Програмата треба да провери дали внесената матрица 
е единечна матрица. Доколку матрицата е единечна, тогаш главната 
нитка треба да ја отпечати таа информација. Во спротивно, доколку 
матрицата не е единечна, тогаш секоја нитка треба својот дел од матрицата 
да го промени така што на крајот ќе се добие единечна матрица. 
Проверката на матрицата и промената на матрицата (доколку тоа е потребно) 
треба да бидат распределени рамномерно помеѓу нитките. Единечна матрица 
претставува матрица во која сите елементи се нули, освен елементите сместени 
по должина на главната дијагонала кои се единици.

За проверка на единечност на матрица да се користи pthread, а за промена на 
матрицата во единечна да се користи openmp библиотеката. Замена на библиотеките не е дозволена.

*/

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <omp.h>

#define NUM_THREADS 4

void *checkIdentity(void *);

int **matrix;
int matrix_dim;

int flag = 1;

int main()
{
    int i;
    int j;

    pthread_t threads[NUM_THREADS];

    scanf("%d", &matrix_dim);
    matrix = (int **) malloc (matrix_dim * sizeof(int *));

    for (int i = 0; i < matrix_dim; i++)
    {
        matrix[i] = (int *) malloc(matrix_dim * sizeof(int));
    }

    for (int i = 0; i < matrix_dim; i++)
    {
        for (int j = 0; j < matrix_dim; j++)
        {
            // scanf("%d", &matrix[i][j]);
            matrix[i][j] = rand() % 15 + 1;
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n"); 

    for (int i = 0; i < NUM_THREADS; i++)
    {
        int ret = pthread_create(&threads[i], NULL, checkIdentity, (void *)i);

        if (ret)
        {
            printf("ERROR\n");
            return -1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    if (flag == 1)
    {
        printf("It is identity\n");
    }
    else 
    {
        #pragma omp parallel for private(i, j) num_threads(NUM_THREADS)
        for (i = 0; i < matrix_dim; i++)
        {
            for (j = 0; j < matrix_dim; j++)
            {
                if (i == j && matrix[i][j] != 1)
                {
                    matrix[i][j] = 1;
                }
                else if (i != j && matrix[i][j] != 0)
                {
                    matrix[i][j] = 0;
                }
            }
        }
    }

    for (i = 0; i < matrix_dim; i++)
    {
        for (j = 0; j < matrix_dim; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}

void *checkIdentity(void *ptr)
{
    long thread_id = (long) ptr;

    // here I will divide the matrix rows

    int rows_per_thread = matrix_dim / NUM_THREADS;
    int remainder_rows = matrix_dim % NUM_THREADS;

    int start = thread_id * rows_per_thread + (thread_id < remainder_rows ? thread_id : remainder_rows);
    int end = start + rows_per_thread + (thread_id < remainder_rows ? 1 : 0);

    for (int i = start; i < end; i++)
    {
        for (int j = 0; j < matrix_dim; j++)
        {
            if (i == j && matrix[i][j] != 1)
            {
                flag = 0;
            }
            else if (i != j && matrix[i][j] != 0)
            {
                flag = 0;
            }
        }
    }

    pthread_exit(NULL);
}