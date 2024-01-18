#include<stdio.h>
#include<omp.h>

int main() {
	#pragma omp parallel num_threads(16)
	{
		int ncpu, tid, npr, nthr;
		ncpu = omp_get_num_procs();
		tid = omp_get_thread_num();
		npr = omp_get_num_threads();
		nthr = omp_get_max_threads();

		if (tid == 0) {
			printf("%d: %d, %d, %d\n", tid, ncpu, npr, nthr);
		}
		printf("Hello %d.\n", tid);
	}
}

