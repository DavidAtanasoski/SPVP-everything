#include<stdio.h>
#include<stdlib.h>

int main() {
	int i, j, ii, jj, board_height, board_width, figure_height, figure_width;
	int *board, *king;
	int check, count = 0;
	int r_count = 0;
	int results[2*64];

	scanf("%d %d", &board_height, &board_width);
	board = (int *) malloc(board_height * board_width * sizeof(int));

	for (i = 0; i < board_height; i++) {
		for (j = 0; j < board_width; j++) {
			board[i * board_width + j] = rand() % 256;
		}
	}

	for (i = 0; i < board_height; i++) {
		for (j = 0; j < board_width; j++) {
			printf("%5d", board[i * board_width + j]);
		}
		printf("\n");
	}

	printf("\n\n");
	figure_height = board_height / 8;
	figure_width = board_width / 8;
	king = (int *) malloc(figure_height * figure_width * sizeof(int));

	for (i = 0; i < figure_height; i++) {
		for (j = 0; j < figure_width; j++) {
			king[i * figure_width + j] = board[(5 * figure_height) * board_width + 4 * figure_width + i * board_width + j];
		}
	}

	for (i = 0; i < figure_height; i++) {
		for (j = 0; j < figure_width; j++) {
			printf("%5d", king[i * figure_width + j]);
		}
		printf("\n");
	}

	#pragma omp parallel for shared(board, king, board_height, board_width, figure_height, figure_width) private(i, j, ii, jj, check) reduction(+:count)
	for (i = 0; i < board_height; i += figure_height){
		for (j = 0; j < board_width; j += figure_width) {
			check = 1;
			for (ii = 0; ii < figure_height; ii++) {
				for (jj = 0; jj < figure_width; jj++) {
					if (board[(i + ii) * board_width + j + jj] != king[ii * figure_width + jj]) {
						check = 0;
					}
				}
			}
			if (check == 1) {
				#pragma omp critical 
				{
					results[r_count] = i; results[r_count + 1] = j;
					r_count += 2;
					count++;
				}
			}
		}
	}

	if (count == 1) {
		printf("Correct number of figures found.\n");
		for (i = 0; i < r_count; i++) {
			printf("%d, ", results[i]);
		}
	}

	free(board);
	free(king);

	return 0;
}

