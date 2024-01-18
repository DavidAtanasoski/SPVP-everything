/*
    Analogen audio signal - SPVP_2021_02_17
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <pthread.h>

#define NUM_THREADS 4
#define SAMPLES 50  // 1s = 10 samples, therefore 5s * 10 = 50 samples
#define BITRATE 1024

void *generateSignal(void *);

int *signal1;
int *signal2;

pthread_mutex_t mutex;

int main()
{
    srand(time(NULL));
    
    pthread_mutex_init(&mutex, NULL);

    signal1 = (int *) malloc(SAMPLES * sizeof(int));
    signal2 = (int *) malloc(SAMPLES * sizeof(int));

    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        int ret = pthread_create(&threads[i], NULL, generateSignal, (void *)i);

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

    printf("Signal 1: ");
    for (int i = 0; i < SAMPLES; i++)
    {
        printf("%d ", signal1[i]);
    }
    printf("\n");

    printf("Signal 2: ");
    for (int i = 0; i < SAMPLES; i++)
    {
        printf("%d ", signal2[i]);
    }
    printf("\n");

    int check = 1;
    for (int i = 0; i < 5; i++)
    {
        int count = 0;
        #pragma omp parallel for num_threads(NUM_THREADS) reduction(+: count)
        for (int j = 0; j < SAMPLES; j++)
        {
            if (abs(signal1[j] - signal2[j]) <= 20)
            {
                count++;
                // signal1[i] = 0 - signal1[i];
                // signal2[i] = 0 - signal2[i];
            }
        }

        double ratio = (double) count / SAMPLES;

        if (ratio < 0.7)
        {
            check = 0;
            break;
        }
    }

    if (check)
    {
        printf("Slicni\n");
    }
    else 
    {
        printf("Razlicni\n");
    }

    return 0;
}

void *generateSignal(void *ptr)
{
    long thread_id = (long) ptr;

    int elements_per_thread = SAMPLES / NUM_THREADS;
    int remainder_elements = SAMPLES % NUM_THREADS;

    int start = thread_id * elements_per_thread + (thread_id < remainder_elements ? thread_id : remainder_elements);
    int end = start + elements_per_thread + (thread_id < remainder_elements ? 1 : 0);

    for (int i = start; i < end; i++)
    {
        signal1[i] = rand() % BITRATE;
        signal2[i] = rand() % BITRATE;
    }
    pthread_exit(NULL);
}
