#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define num_threads 4

void *work(void *ptr);

int len, key, found = 0;
int *array, *displ;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	int i, ret, len_per_thread, overflow;
	pthread_t threads[num_threads];

	scanf("%d", &len);
	array = (int *) malloc(len * sizeof(int));
	for (i = 0; i < len; i++) {
		array[i] = i + 1;
	}

	displ = (int *) malloc((num_threads + 1) * sizeof(int));

	len_per_thread = len / num_threads;
	overflow = len - len_per_thread * num_threads;

	displ[0] = 0;
	for (i = 0; i < num_threads; i++) {
		if (i < overflow) {
			displ[i + 1] = displ[i] + len_per_thread + 1;
		} else {
			displ[i + 1] = displ[i] + len_per_thread;
		}
	}

	scanf("%d", &key);

	for (i = 0; i < num_threads; i++) {
		ret = pthread_create(&threads[i], NULL, work, (void *)i);

		if (ret) {
			return -1;
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	if (found) {
		printf("Da.");
	} else {
		printf("Ne.");
	}

	return 0;
}

void *work(void *ptr) {	
	int id = (int *) ptr;
	int low = displ[id], high = displ[id + 1], mid;

	if (key < low || key > high) {
		pthread_exit(NULL);
	} else {
		while (low <= high && !found) {
			mid = (high - low) / 2 + low;
			if (array[mid] == key) {
				found = 1;
				break;
			} else if (array[mid] > key) {
				high = mid - 1;
			} else {
				low = mid + 1;
			}
		}
	}
}


