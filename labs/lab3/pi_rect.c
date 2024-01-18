#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void *approximate(void *ptr);

#define INTERVALS 1000000

int num_threads;
double total_area = 0.0;

int main(int argc, char **argv)
{
	scanf("%d", &num_threads);

	pthread_t *threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));

	clock_t start = clock();
	for (int i = 0; i < num_threads; i++)
	{
		int ret = pthread_create(&threads[i], NULL, approximate, &i);

		if (ret) 
		{
			printf("Error\n");
			return -1;
		}
	}

	for (int i = 0; i < num_threads; i++)
	{
		pthread_join(threads[i], NULL);
	}
	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;

	printf("Total area: %13.11f\n", total_area);
	printf("Seconds: %f, threads: %d\n", seconds, num_threads);

	return 0;
}

void *approximate(void *ptr)
{
	int thread_id = *(int *)ptr;

	double area;
	double ysum;
	double xi;

	ysum = 0.0;
	for (int i = thread_id; i < INTERVALS; i += num_threads)
	{
		xi = (1.0/INTERVALS) * (i+0.5);
		ysum += 4.0/(1.0 + xi*xi);
	}

	area = ysum * (1.0/INTERVALS);
	total_area += area;

	pthread_exit(NULL);
}
