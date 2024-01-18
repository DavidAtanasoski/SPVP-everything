#include<stdio.h>
#include<pthread.h>

#define num_threads 5

void *print_m(void *ptr);

int main() {
	pthread_t thread[num_threads];
	int ret; int i;

	for(i = 0; i < num_threads; i++) {
		ret = pthread_create(&thread[i], NULL, print_m, (void *) i);

		if (ret) {
			printf("ERROR\n");
			return -1;
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(thread[i], NULL);
	}

	return 0;
}

void *print_m(void *ptr) {
	printf("Thread %d\n", (int *)ptr);
}

