#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

/* 
Да се напише MPI програма во која во нултиот процес ќе се внесе низа од N цели броеви, 
при што N исто така е цел број внесен од тастатура. Дополнително, се внесува и квадратна матрица 
со димензии N x N. Програмата треба да провери колку пермутации од внесената низа можат да се 
најдат во рамките на квадратната матрица, и по редици и по колони. Притоа, проверката да се 
подели рамномерно на сите процеси, при што секој процес ќе го добие за проверка само својот дел од матрицата. 
Во нултиот процес да се отпечати колку пермутации биле пронајдени од страна на секој процес. 
*/

#define n_threads 4

int sort(int *array, int n) {
	int tmp;
	for (int i = 0; i < n - 1; i++) {
		for (int j = i + 1; j < n; j++) {
			if (array[i] > array[j]) {
				tmp = array[j];
				array[j] = array[i];
				array[i] = tmp;
			}
		}
	}
}

int main() {
	int i, j, n, count = 0;
	int *array, *array_row, *array_column, **matrix;

	scanf("%d", &n);

	array = (int *) malloc(n * sizeof(int));
	matrix = (int **) malloc(n * sizeof(int *));
	for (i = 0; i < n; i++) {
		matrix[i] = (int *) malloc(n * sizeof(int));
	}
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			matrix[i][j] = rand() % 5;
			printf("%d, ", matrix[i][j]);
		}
		printf("\n");
	}

	for (i = 0; i < n; i++) {
		scanf("%d", &array[i]);
	}

	sort(array, n);

	#pragma omp parallel for reduction(+:count) private(array_row, array_column)
	for (i = 0; i < n; i++) {
		array_row = (int *) malloc(n * sizeof(int));
		array_column = (int *) malloc(n * sizeof(int));

		for (j = 0; j < n; j++) {
			array_row[j] = matrix[i][j];
			array_column[j] = matrix[j][i];
		}

		sort(array_row, n);
		sort(array_column, n);

		int p = 1;
		for (j = 0; j < n; j++) {
			if (array_row[j] != array[j]) {
				p = 0;
				break;
			}
		}
		if (p == 1) {
			count++;
		}

		p = 1;
		for (j = 0; j < n; j++) {
			if (array_column[j] != array[j]) {
				p = 0;
				break;
			}
		}
		if (p == 1) {
			count++;
		}
	}

	printf("Count: %d\n", count);
}

