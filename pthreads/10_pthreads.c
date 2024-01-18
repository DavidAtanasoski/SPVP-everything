#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define num_threads 4

void *work(void *ptr);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int board_height, board_width, figure_height, figure_width;
int *board, *king;
int count = 0, r_count = 0;
int results[2 * 64];

int rows_per_thread, overflow;
int displ[num_threads + 1];

int main(int argc, char *argv[]) {
	int i, j;
	pthread_t threads[num_threads];

	scanf("%d %d", &board_height, &board_width);
	board = (int *) malloc(board_height * board_width * sizeof(int));

	for (i = 0; i < board_height; i++) {
		for (j = 0; j < board_width; j++) {
			board[i * board_width + j] = rand() % 256;
		}
	}
	

	figure_height = board_height / 8;
	figure_width = board_width / 8;
	king = (int *) malloc(figure_height * figure_width * sizeof(int));

	for (i = 0; i < figure_height; i++) {
		for (j = 0; j < figure_width; j++) {
			king[i * figure_width + j] = rand() % 256;
		}
	}

	for (i = 0; i < figure_height; i++) {
		for (j = 0; j < figure_width; j++) {
			king[i * figure_width + j] = board[(5 * figure_height) * board_width + 4 * figure_width + i * board_width + j];
		}
	}

	rows_per_thread = (board_height / figure_height) / num_threads;
	overflow = (board_height / figure_height) - rows_per_thread * num_threads;

	displ[0] = 0;
	for(i = 0; i < num_threads; i++) {
		if (i < overflow) {
			displ[i+1] = displ[i] + rows_per_thread + 1;
		} else {
			displ[i+1] = displ[i] + rows_per_thread;
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_create(&threads[i], NULL, work, (int *)i);
	}	
	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	if (count == 1) {
		for (i = 0; i < r_count; i++) {
			printf("%d, ", results[i]);
		}
	}

	return 0;
}

void *work(void *ptr) {	
	int id = (int) ptr;
	int start = displ[id], end = displ[id + 1];
	int i, j, ii, jj, check;

	for (i = start * figure_height; i < end * figure_height; i += figure_height) {
		for (j = 0; j < board_width; j += figure_width) {
			check = 1;
			for (ii = 0; ii < figure_height && check != 0; ii++) {
				for (jj = 0; jj < figure_width; jj++) {
					if (board[(i + ii) * board_width + j + jj] != king[ii * figure_width + jj]) {
						check = 0;
						break;
					}
				}
			}

			if (check == 1) {
				pthread_mutex_lock(&mutex);
				count++;
				results[r_count] = i; results[r_count + 1] = j;
				r_count += 2;
				pthread_mutex_unlock(&mutex);
			}
		}
	}


	pthread_exit(NULL);
}
