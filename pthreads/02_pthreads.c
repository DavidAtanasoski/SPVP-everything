#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

void *print_m(void *ptr);

int main() {
	int num_threads;
	int ret; int i;
	
	scanf("%d", &num_threads);

	pthread_t *threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));

	for(i = 0; i < num_threads; i++) {
		ret = pthread_create(&threads[i], NULL, print_m, (void *) i);

		if (ret) {
			printf("ERROR\n");
			return -1;
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}

void *print_m(void *ptr) {
	printf("Thread %d\n", (int *)ptr);
}

