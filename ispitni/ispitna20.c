#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define NUM_THREADS 4

int main()
{
    srand(time(NULL));

    int n;
    int *array;
    int global_res[10];

    scanf("%d", &n);
    array = (int *) malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        array[i] = rand() % 10;
        printf("%d ", array[i]);
    }
    printf("\n");

    for (int i = 0; i < 10; i++)
    {
        global_res[i] = 0;
    }
    
    #pragma omp parallel for num_threads(NUM_THREADS) 
    for (int i = 0; i < n; i++)
    {
        int thread_id = omp_get_thread_num();
        global_res[thread_id] += array[i];
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        printf("%d ", global_res[i]);
    }

    return 0;
}