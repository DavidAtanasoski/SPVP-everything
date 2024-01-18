#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<omp.h>

#define n_threads 4

/*
Да се напише програма во која во нулта нитка ќе се прочита 
квадратна матрица со димензии n x n, каде n исто така се внесува 
од тастатура. Програмата треба да провери дали внесената матрица 
е единечна матрица. Доколку матрицата е единечна, тогаш главната 
нитка треба да ја отпечати таа информација. Во спротивно, доколку 
матрицата не е единечна, тогаш секоја нитка треба својот дел од матрицата 
да го промени така што на крајот ќе се добие единечна матрица. 
Проверката на матрицата и промената на матрицата (доколку тоа е потребно) 
треба да бидат распределени рамномерно помеѓу нитките. Единечна матрица 
претставува матрица во која сите елементи се нули, освен елементите сместени 
по должина на главната дијагонала кои се единици.

За проверка на единечност на матрица да се користи pthread, а за промена на 
матрицата во единечна да се користи openmp библиотеката. Замена на библиотеките не е дозволена.

*/

int **mat;
int n, p = 1;
int rows_per_thread, overflow;
int displ[n_threads + 1];

void *work(void *ptr);

int main() {
	int i, j, ret;
	pthread_t threads[n_threads];

	scanf("%d", &n);

	mat = (int **) malloc(n * sizeof(int *));
	for (i = 0; i < n; i++) {
		mat[i] = (int *) malloc(n * sizeof(int));
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			mat[i][j] = rand() % 10;
			printf("%5d", mat[i][j]);
		}
		printf("\n");
	}

	// dali e edinechna
	rows_per_thread = n / n_threads;
	overflow = n - rows_per_thread * n_threads;

	displ[0] = 0;
	for(i = 0; i < n_threads; i++) {
		if (i < overflow) {
			displ[i+1] = displ[i] + rows_per_thread + 1;
		} else {
			displ[i+1] = displ[i] + rows_per_thread;
		}
	}

	for (i = 0; i < n_threads; i++) {
		pthread_create(&threads[i], NULL, work, (int *)i);
	}	
	for (i = 0; i < n_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	if (p == 0) {
		printf("Ne e.\n");
		
		#pragma omp parallel for private(i, j) num_threads(n_threads)
		for (i = 0; i < n; i++) {
			for (j = 0; j < n; j++) {
				if (i == j && mat[i][j] != 1) {
					mat[i][j] = 1;
				}
				if (i != j && mat[i][j] != 0) {
					mat[i][j] = 0;
				}
			}
		} 
	} else {
		printf("Da e.\n");
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			printf("%5d", mat[i][j]);
		}
		printf("\n");
	}
}

void *work(void *ptr) {	
	int id = (int) ptr;
	int i, j;

	for (i = displ[id]; i < displ[id + 1]; i++) {
		for (j = 0; j < n; j++) {
			if (i == j && mat[i][j] != 1) {
				p = 0;
			}
			if (i != j && mat[i][j] != 0) {
				p = 0;
			}
		}
	} 

	pthread_exit(NULL);
}

