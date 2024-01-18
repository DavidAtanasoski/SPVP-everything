#include<omp.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>

/* 
Да се напише MPI програма која ќе врши компресија на сигнал, при што факторот на компресија 
ќе биде внесен од тастатура (1/2x). Сигналот е низа од случајно генерирани целобројни 
вредности во опсег од 1 до 25. Компресијата ќе се врши со метода на усреднување на вредностите. 
Задачата да се подели рамномерно на сите процеси, при што секој процес да го добие својот дел од сигналот.
Во нултиот процес да се прикаже компресираниот сигнал.
*/

#define n_threads 4

int main() {
	int i, j, n, x, f, sum;
	int *array, *ca;

	scanf("%d", &n);
	scanf("%d", &x);

	f = (int) pow(2, x);

	if (n % (n_threads * f) != 0) {
		printf("Error");
		exit(-1);
	}

	array = (int *) malloc(n * sizeof(int));
	for(i = 0; i < n; i++) {
		array[i] = rand() % 25;
	}
	for(i = 0; i < n; i++) {
		printf("%d, ", array[i]);
	}

	ca = (int *) malloc((n/f) * sizeof(int));

	#pragma omp parallel for private(i, j, sum)
	for (i = 0; i < n; i += f) {
		sum = 0;
		for (j = i; j < i + f; j++) {
			sum += array[j];
		}

		ca[i / f] = sum / f;
	}

	printf("\n");
	for(i = 0; i < n/f; i++) {
		printf("%d, ", ca[i]);
	}
}

