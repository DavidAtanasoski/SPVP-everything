#include<stdio.h>
#include<math.h>
#include<pthread.h>

#define num_threads 4

void *work(void *ptr);

int main(int argc, char *argv[]) {
	int i, ret;
	pthread_t threads[num_threads];

	for (i = 0; i < num_threads; i++) {
		ret = pthread_create(&threads[i], NULL, work, (void *)i);

		if (ret) {
			printf("Error\n");
			return -1;
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}

void *work(void *ptr) {
	int id = (int *) ptr;

	for (int i = -10 + id; i < 10; i += num_threads) {
		for (int j = -10; j < 10; j++) {
			for (int k = -10; k < 10; k++) {
				if (pow(i, 4) * k + 3 * pow(j, 2) * k + 4 * k == 20) {
					printf("%d %d %d\n", i, j, k);
				}
			}
		}
	}
}


