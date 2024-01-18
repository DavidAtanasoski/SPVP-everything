#include<stdio.h>
#include<pthread.h>

#define num_threads 4

void *work(void *ptr);

int r, c, c_per_thread, overflow, global_max;
int *matrix;
int displ[num_threads + 1];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
	int i, j, ret;
	pthread_t threads[num_threads];

	scanf("%d %d", &r, &c);
	matrix = (int *) malloc(r * c * sizeof(int));
	
	for (i = 0; i < r; i++) {
		for (j = 0; j < c; j++) {
			scanf("%d", (matrix + i * c + j));
		}
	}

	c_per_thread = c / num_threads;
	overflow = c - c_per_thread * num_threads;

	displ[0] = 0;
	for (i = 0; i < num_threads; i++) {
		if (i < overflow) {
			displ[i + 1] = displ[i] + c_per_thread + 1;
		} else {
			displ[i + 1] = displ[i] + c_per_thread;
		}
	}

	global_max = *matrix;
	
	for (i = 0; i < num_threads; i++) {
		ret = pthread_create(&threads[i], NULL, work, (void *) i);

		if (ret) {
			printf("Error\n");
			return -1;
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Globalniot maksimum e %d.\n", global_max);

	return 0;
}

void *work(void *ptr) {
	int i, j;
	int id = ptr;
	int max;

	for (j = displ[id]; j < displ[id + 1]; j++) {
		max = *(matrix + j);

		for (i = 1; i < r; i++) {
			if (max < *(matrix + i * c + j)) {
				max = *(matrix + i * c + j);
			}
		}

		printf("Vrednosta e %d.\n", max);

		pthread_mutex_lock(&mutex);
		if (max > global_max) {
			global_max = max;
		}
		pthread_mutex_unlock(&mutex);
	}
	

	pthread_exit(NULL);
}


