#include<stdio.h>
#include<math.h>
#include<omp.h>

int main() {
	int i, j, k;
	#pragma omp parallel for private(i, j, k)
	for (i = -10; i < 10; i++) {
		for (j = -10; j < 10; j++) {
			for (k = -10; k < 10; k++) {
				if (pow(i, 4) * k + 3 * pow(j, 2) * k + 4 * k == 20) {
					printf("%d %d %d\n", i, j, k);
				}
			}
		}
	}
}

