/*
Да се напише pthread програма која по случаен избор ќе генерира 100 случајни 
комбинации за вредности на три променливи x, y и z. Секоја (x, y, z) комбинација 
од 100-те генерирани комбинации треба да биде уникатна. Генерирањето рамномерно 
да се распредели помеѓу сите активни pthread нитки.

Следно, да се напише OpenMP програма која ќе ги бара целобројните решенија на равенката: 
8x^4 + 2(xz)^3 - 4xy + 8x = -15
Во OpenMP програмата да се испроба секоја од 100-те случајно генерирани комбинации на 
променливите x, y и z. Пребарувањето на решенија рамномерно да се распредели 
помеѓу сите активни openmp нитки.
Најдените решенија, ако постојат, на крај треба да се отпечатат во екран во нулатата нитка.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <omp.h>

#define NUM_THREADS 4
#define MAX_VALUES 100

typedef struct {
    int x;
    int y;
    int z;
} Combination;

void *generateNumbers(void *);
int checkIfExists(Combination);

Combination combinations[MAX_VALUES];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main()
{
    srand(time(NULL));

    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++)
    {
        int ret = pthread_create(&threads[i], NULL, generateNumbers, (void *) i);

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

    // for (int i = 0; i < MAX_VALUES; i++)
    // {
    //     printf("x = %d y = %d z = %d\n", combinations[i].x, combinations[i].y, combinations[i].z);
    // }

    int i;

    #pragma omp parallel for private(i) num_threads(NUM_THREADS)
    for (i = 0; i < MAX_VALUES; i++)
    {
        if ((8 * pow(combinations[i].x, 4) + 2 * pow(combinations[i].x * combinations[i].z, 3) - 4 * combinations[i].x * combinations[i].y + 8 * combinations[i].x) == -15)
        {
            // pragma omp critical za printf
            printf("x = %d y = %d z = %d\n", combinations[i].x, combinations[i].y, combinations[i].z);
        }
    }

    return 0;
}

int checkIfExists(Combination comb)
{
    for (int i = 0; i < MAX_VALUES; i++)
    {
        if (combinations[i].x == comb.x && combinations[i].y == comb.y && combinations[i].z == comb.z)
        {
            return 1;
        }
    }
    return 0;
}

void *generateNumbers(void *ptr)
{
    long thread_id = (long) ptr;

    int start = thread_id * (MAX_VALUES / NUM_THREADS);
    int end = (thread_id + 1) * (MAX_VALUES / NUM_THREADS);

    for (int i = start; i < end; i++)
    {
        Combination temp;

        while (1)
        {
            temp.x = rand() % 100;
            temp.y = rand() % 100;
            temp.z = rand() % 100;

            pthread_mutex_lock(&mutex);
            if (!checkIfExists(temp))
            {
                combinations[i] = temp;
                pthread_mutex_unlock(&mutex);
                break;
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(NULL);
}
