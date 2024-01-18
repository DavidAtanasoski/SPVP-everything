/*
    Да се напише програма во која се пополнуваат елементи од низа. 
    Од низата треба да се отфрлат непарните елементи и променетата 
    низа да се испечати на екран. Задачата да се распредели 
    рамномерно помеѓу повеќе процеси.
*/

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

#define NUM_THREADS 1

omp_lock_t lock;

void deleteElement(int* array, int* size, int index) {
    omp_set_lock(&lock);

    for (int i = index; i < *size - 1; i++) {
        array[i] = array[i + 1];
    }

    (*size)--;

    omp_unset_lock(&lock);
}

int main()
{
    srand(time(NULL));

    int *array;

    int n;

    scanf("%d", &n);

    array = (int *) malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        array[i] = rand() % 50;
        printf("%d ", array[i]);
    }
    printf("\n");

    int temp_len = n;

    #pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < n; i++)
    {
        if (array[i] % 2 != 0)
        {
            deleteElement(array, &n, i);
        }
    }

    omp_destroy_lock(&lock);

    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }

    return 0;
}