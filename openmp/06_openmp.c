#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

#define n_threads 4

struct Node {
	int val;
	struct Node *l, *r;
};

void insertNode(struct Node **root, int val) {
	struct Node *el = (struct Node *) malloc(sizeof(struct Node));
	el->val = val; el->l = el->r = 0;

	if (! *root) {
		*root = el;
		return;
	}

	if (val < (*root)->val) {
		if ((*root)->l) {
			insertNode(&((*root)->l), val);
		} else {
			(*root)->l = el;
		}
		return;
	}

	if (val > (*root)->val) {
		if ((*root)->r) {
			insertNode(&((*root)->r), val);
		} else {
			(*root)->r = el;
		}
	}
}

void printInorder(struct Node **root) {
	if (! *root) return;
	printInorder(&(*root)->l);
	printf("%d ", (*root)->val);
	printInorder(&(*root)->r);
}

int inTree(struct Node **root, int el) {
	printf("Thread %d\n", omp_get_thread_num());
	if (! *root) return 0;
	if ((*root)->val == el) return 1;

	int i = 0, j = 0;

	#pragma omp task shared(i)
	if (el < (*root)->val) {
		i = inTree(&(*root)->l, el);
	}
	#pragma omp task shared(j)
	j = inTree(&(*root)->r, el);
	#pragma omp taskwait

	return i || j;
}

int main() {
	int i, n, e, val;
	struct Node *tree = 0;

	scanf("%d", &n);
	for (i = 0; i < n; i++) {
		// scanf("%d", &val); insertNode(&tree, val);
		val = (int) random() % 100;
		insertNode(&tree, val);
		if (i == 2) {
			e = val;
		}
	}

	#pragma omp parallel num_threads(n_threads)
	{
		// Problemot so taskovi - spomnat od chas
		int ret = inTree(&tree, e);
		printf("%d\n", ret);
		
	}

	return 0;
}


























