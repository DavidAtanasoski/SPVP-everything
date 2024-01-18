/*
Да се напише pthreads програма во која од тастатура се вчитува низа од цели броеви. 
Програмата треба да ја најде најдолгата подниза за која важи правилото: секој број 
е еднаков на збирот од претходните два броја. Пребарувањето да се раздели рамномерно 
помеѓу повеќе нитки, при што на крајот програмата треба да го испечати редниот број на 
елементот од каде започнува бараната подниза.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

void *work(void *);

int *arr;
int num_elements;

int global_len;
int global_pos;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    scanf("%d", &num_elements);
    arr = (int *) malloc(num_elements * sizeof(int));

    for (int i = 0; i < num_elements; i++)
    {
        scanf("%d", &arr[i]);
    }

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

    printf("At pos: %d with len: %d\n", global_pos + 2, global_len - 2);

    return 0;
}

void *work(void *ptr)
{
    long thread_id = (long) ptr;

    int elements_per_thread = num_elements / NUM_THREADS;
    int remainder_elements = num_elements % NUM_THREADS;

    int start = thread_id * elements_per_thread + (thread_id < remainder_elements ? thread_id : remainder_elements);
    int end = start + elements_per_thread + (thread_id < remainder_elements ? 1 : 0);

    for (int i = start + 2; i < end; i++)
    {
        int local_len = 0;
        int start_pos = i;

        while (arr[i] == arr[i-1] + arr[i-2] && i < num_elements)
        {
            local_len++;
            i++;
        }

        pthread_mutex_lock(&mutex);
        if (local_len > global_len)
        {
            global_len = local_len;
            global_pos = start_pos;
        }
		pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}