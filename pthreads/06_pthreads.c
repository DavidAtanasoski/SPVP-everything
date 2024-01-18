#include<stdio.h>
#include<math.h>
#include<pthread.h>

void *work(void *ptr);

struct thread_args {
	int thread_id;
	int thread_number;

	int thread_from;
	int thread_to;

	int thread_sum;
};

int len;
int *arr;

int main(int argc, char *argv[]) {
	int i, ret;
	int num_threads, len_per_thread, overflow;
	int sum = 0;

	scanf("%d", &num_threads);
	scanf("%d", &len);
	arr = (int *) malloc(len * sizeof(int));
	for (i = 0; i < len; i++) {
		arr[i] = i + 1;
	}

	pthread_t *threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
	struct thread_args *args = (struct thread_args *) malloc(num_threads * sizeof(struct thread_args));

	len_per_thread = len / num_threads;
	overflow = len - num_threads * len_per_thread;

	args[0].thread_from = 0;
	for (i = 0; i < num_threads; i++) {
		args[i].thread_id = i;
		args[i].thread_number = num_threads;
		args[i].thread_sum = 0;

		if (i > 0) {
			args[i].thread_from = args[i-1].thread_to;
		}

		if (i < overflow) {
			args[i].thread_to = args[i].thread_from + len_per_thread + 1;
		} else {
			args[i].thread_to = args[i].thread_from + len_per_thread;
		}

		ret = pthread_create(&threads[i], NULL, work, &args[i]);

		if (ret) {
			printf("Error\n");
			return -1;
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
		sum += args[i].thread_sum;
	}

	return 0;
}

void *work(void *ptr) {
	int i;
	struct thread_args *args = ptr;
	
	int id = (int)args->thread_id;

	for (i = (int)args->thread_from; i < (int)args->thread_to; i++) {
		args->thread_sum += arr[i];
	}
	printf("Thread %d says %d.\n", id, args->thread_sum);

	pthread_exit(NULL);
}


