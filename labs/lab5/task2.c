#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

#define NUM_THREADS 4
#define MAX_CHARACTERS 256

int main()
{
    char *first = malloc(MAX_CHARACTERS * sizeof(char));
    char *second = malloc(MAX_CHARACTERS * sizeof(char));

    printf(">> ");
    scanf("%s %s", first, second);

    int len_first = strlen(first);
    int len_second = strlen(second);
    int counter = 0;
    int i;

    #pragma omp parallel num_threads(NUM_THREADS) private(i) reduction(+: counter)
    {
        int thread_id = omp_get_thread_num();
        int threads_num = omp_get_num_threads();

        int block_size = len_first / threads_num;
        int start = block_size * thread_id;
        int end = (thread_id == threads_num - 1) ? len_first : (thread_id + 1) * block_size;

        for (i = start; i < end; i++)
        {
            //#pragma omp critical
            if (!strncmp(first + i, second, len_second))
            {
                counter++;
            }
        }

        //printf("thread: %d, %d\n", thread_id, counter);
    }

    printf("Counter: %d\n", counter);

    return 0;
}