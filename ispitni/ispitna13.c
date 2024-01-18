/*
    Zamena i mirror na koloni
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define NUM_THREADS 4

typedef struct {
    int sum;
    int pos;
} ColumnInfo;

void swap(int *, int *);

int main()
{
    srand(time(NULL));

    int **matrix;

    int n_rows;
    int m_columns;

    scanf("%d %d", &n_rows, &m_columns);
    matrix = (int **) malloc(n_rows * sizeof(int *));

    for (int i = 0; i < n_rows; i++)
    {
        matrix[i] = (int *) malloc(m_columns * sizeof(int));
    }

    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            matrix[i][j] = rand() % 10;
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");


    int i;
    int j;

    ColumnInfo c1;
    c1.sum = matrix[0][0];

    ColumnInfo c2;
    c2.sum = matrix[0][0];

    #pragma omp parallel for private(i, j) num_threads(NUM_THREADS)
    for (i = 0; i < m_columns; i++)
    {
        int local_sum = 0;

        for (j = 0; j < n_rows; j++)
        {
            #pragma omp atomic
            local_sum += matrix[j][i];
        }

        #pragma omp critical
        {
            if (local_sum > c1.sum)
            {
                c2.sum = c1.sum;
                c1.sum = local_sum;

                c2.pos = c1.pos;
                c1.pos = i;
            }
            else if (local_sum > c2.sum && local_sum < c1.sum)
            {
                c2.sum = local_sum;
                c2.pos = i;
            }
        }
    }

    printf("max1: %d max2: %d\n", c1.sum, c2.sum);
    printf("pos1: %d pos2: %d\n", c1.pos, c2.pos);

    #pragma omp parallel for private(i) num_threads(NUM_THREADS)
    for (i = 0; i < n_rows; i++)
    {
        int temp = matrix[i][c1.pos];
        matrix[i][c1.pos] = matrix[i][c2.pos];
        matrix[i][c2.pos] = temp;
    }

    #pragma omp parallel for private(i) num_threads(NUM_THREADS)
    for (i = 0; i < n_rows / 2; i++)
    {
        swap(&matrix[i][c1.pos], &matrix[n_rows - i - 1][c1.pos]);
        swap(&matrix[i][c2.pos], &matrix[n_rows - i - 1][c2.pos]);
    }
    printf("\n");

    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

void swap(int *first, int *second)
{
    int temp = *first;
    *first = *second;
    *second = temp;
}