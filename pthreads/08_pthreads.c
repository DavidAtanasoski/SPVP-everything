#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define num_threads 4

void *work(void *ptr);

int *array, *displ;
int len, global_p, global_l;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	int i, ret, len_per_thread, overflow;
	pthread_t threads[num_threads];

	scanf("%d", &len);
	array = (int *) malloc(len * sizeof(int));
	for (i = 0; i < len; i++) {
		scanf("%d", &array[i]);
	}

	displ = (int *) malloc((num_threads + 1) * sizeof(int));

	len_per_thread = (len - 2) / num_threads;
	overflow = len - 2 - len_per_thread * num_threads;

	displ[0] = 2;
	for (i = 0; i < num_threads; i++) {
		if (i < overflow) {
			displ[i + 1] = displ[i] + len_per_thread + 1;
		} else {
			displ[i + 1] = displ[i] + len_per_thread;
		}
	}


	global_l = 0; global_p = 0;

	for (i = 0; i < num_threads; i++) {
		ret = pthread_create(&threads[i], NULL, work, (void *)i);

		if (ret) {
			return -1;
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("%d %d.\n", global_l + 2, global_p - 2);

	return 0;
}

void *work(void *ptr) {	
	int id = (int *)ptr;
	int i, length, pos;

	for (i = displ[id]; i < displ[id + 1]; i++) {
		length = 0; pos = i;

		while (array[i] == array[i-1] + array[i-2] && i < len) {
			length++;
			i++;
		}

		pthread_mutex_lock(&mutex);
		if (length > global_l) {
			global_l = length;
			global_p = pos;
		}
		pthread_mutex_unlock(&mutex);
	}

	pthread_exit(NULL);
}


