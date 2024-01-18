#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

#define n_threads 4

int main() {
	int i, len, key, found = 0, len_per_thread, overflow;
	int *array, *displ;

	scanf("%d", &len);
	array = (int *) malloc(len * sizeof(int));
	for (i = 0; i < len; i++) {
		array[i] = i + 1;
	}

	scanf("%d", &key);

	displ = (int *) malloc((n_threads + 1) * sizeof(int));

	len_per_thread = len / n_threads;
	overflow = len - len_per_thread * n_threads;

	displ[0] = 0;
	for (i = 0; i < n_threads; i++) {
		if (i < overflow) {
			displ[i + 1] = displ[i] + len_per_thread + 1;
		} else {
			displ[i + 1] = displ[i] + len_per_thread;
		}
	}
	
	#pragma omp parallel num_threads(n_threads)
	{
		int id = omp_get_thread_num();
		int low = displ[id], high = displ[id + 1], mid;

		if (key < low || key > high) {
			#pragma omp ompd_bp_thread_end();
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

	if (found) {
		printf("Da.");
	} else {
		printf("Ne.");
	}
}

