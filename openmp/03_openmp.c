#include<stdio.h>
#include<math.h>
#include<omp.h>

#define num_threads 4

int main() {
	int *array;
	int i, len, length, pos, global_l, global_p;

	global_l = 0; global_p = 0;

	scanf("%d", &len);
	array = (int *) malloc(len * sizeof(int));
	for (i = 0; i < len; i++) {
		scanf("%d", &array[i]);
	}

	#pragma omp parallel for num_threads(num_threads) private(i, pos, length)
	for (i = 2; i < len; i++) {
		pos = i; length = 0;

		while (array[i] == array[i-1] + array[i-2] && i < len) {
			length++;
			i++;
		}

		#pragma omp critical
		{
			if (length > global_l) {
				global_l = length;
				global_p = pos;
			}
		}
	}
	
	printf("%d %d.\n", global_l + 2, global_p - 2);
}

