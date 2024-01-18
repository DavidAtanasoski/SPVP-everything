#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4

void *work(void *);

int n;
int *array;

int main()
{
    srand(time(NULL));

    scanf("%d", &n);

    array = (int *) malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        array[i] = rand() % 10 + 1;
        printf("%d ", array[i]);
    }
    printf("\n");

    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        int ret = pthread_create(&threads[i], NULL, work, (void *)i);

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

    return 0;
}

void *work(void *ptr)
{
    long thread_id = (long) ptr;

    int elements_per_thread = n / NUM_THREADS;
    int overflow = n % NUM_THREADS;

    int start = thread_id * elements_per_thread + (thread_id < overflow ? thread_id : overflow);
    int end = start + elements_per_thread + (thread_id < overflow ? 1 : 0);

    printf("thread: %ld, %d, %d = %d\n", thread_id, start, end, end-start);

// int start = args->thread_id * rows_per_thread + (args->thread_id < remainder_rows ? args->thread_id : remainder_rows);
// int end = start + rows_per_thread + (args->thread_id < remainder_rows ? 1 : 0);


    pthread_exit(NULL);
}