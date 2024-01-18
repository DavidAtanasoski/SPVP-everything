#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define N 50

double total_result = 0.0;

double f(int);
void *approximate(void *ptr);

typedef struct thread_info
{
	int id;
	int t_count;
} thread_arg;

int main (int argc, char *argv[])
{
	int num_threads;
	
	scanf("%d", &num_threads);

	pthread_t *threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
	thread_arg *t_arg = (thread_arg *) malloc(num_threads * sizeof(t_arg));

	clock_t start = clock();
	for (int i = 0; i <num_threads; i++)
	{
		t_arg[i].id = i;
		t_arg[i].t_count = num_threads;

		int ret = pthread_create(&threads[i], NULL, approximate, &t_arg[i]);

		if (ret) 
		{
			printf("ERROR!\n");
			return -1;
		}
	}

	for (int i = 0; i < num_threads; i++)
	{
		pthread_join(threads[i], NULL);
	}
	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
 
	printf("Total result: %13.11f\n", total_result);
	printf("Seconds: %f, threads: %d\n", seconds, num_threads);

	return 0;
}

double f (int i)
{
	double x;
	x = (double) i / (double) N;
	return 4.0 / (1.0 + x*x);
}

void *approximate(void *ptr)
{
	thread_arg *arg = ptr;

	double local_area = 0.0;

	int interval = (N / 2) / arg->t_count;
	int start_interval = interval * arg->id;
	int end_interval = (arg->id == arg->t_count - 1) ? (N / 2) : start_interval + interval;

	local_area += f(start_interval * 2) - f(end_interval * 2);

	for (int i = start_interval + 1; i <= end_interval; i++)
	{
		local_area += 4.0 * f(2 * i - 1) + 2 * f(2 * i);
	}

    local_area /= (3.0 * N);
	total_result += local_area;

	pthread_exit(NULL);
}