/*
    Да се напише програма која ќе ја промени содржината на одредена матрица, 
    така што секој парен елемент ќе биде заменет со нула. Задачата да се подели 
    рамномерно помеѓу сите процеси. Содржината на променетата матрица треба да 
    се испечати во главниот процес.
    - OPEN MP -
*/
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 4

int main()
{
    int n_rows;
    int m_columns;

    int **matrix;

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
            matrix[i][j] = rand() % 100;
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    #pragma omp parallel for num_threads(NUM_THREADS) shared(matrix)
    for (int i = 0; i < n_rows; i++)
    {
        for (int j = 0; j < m_columns; j++)
        {
            if (matrix[i][j] % 2 == 0)
            {
                matrix[i][j] = 0;
            }
        }
    }


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