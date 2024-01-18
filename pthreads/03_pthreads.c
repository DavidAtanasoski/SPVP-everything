#include<stdio.h>
#include<pthread.h>

struct thread_args {
	int thread_number;
	int thread_id;
};

void *work(void *ptr);

int main(int argc, char *argv[]) {
	int i, ret, num_threads;
	
	scanf("%d", &num_threads);

	pthread_t *threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
	struct thread_args *args = malloc(num_threads * sizeof(struct thread_args));

	for(i = 0; i < num_threads; i++) {
		args[i].thread_number = num_threads;
		args[i].thread_id = i;
		ret = pthread_create(&threads[i], NULL, work, &args[i]);

		if (ret) {
			printf("ERROR\n");
			return -1;
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	return 0;
}

void *work(void *ptr) {
	int i, count = 0;
	struct thread_args *args = ptr;
	int id = (int)args->thread_id;
	int p = (int)args->thread_number;
	for(i = id; i < 65536; i += p) {
		count += check_circuit(id, i);
	}
	printf("Thread %d is done. Solutions %d\n", id, count);
	pthread_exit(NULL);
}

#define EXTRACT_BIT(n, i) ((n&(1<<i))?1:0)

int check_circuit(int id, int z){
	int v[16];
	int i, res = 0;

	for (i = 0; i < 16; i++)
		v[i] = EXTRACT_BIT(z, i);
	
	if ((v[0] || v[1]) && (!v[1] || !v[3]) && (v[2] || v[3])
   	   	&& (!v[3] || !v[4]) && (v[4] || !v[5])
   	   	&& (v[5] || !v[6]) && (v[5] || v[6])
   	   	&& (v[6] || !v[15]) && (v[7] || !v[8])
   	   	&& (!v[7] || !v[13]) && (v[8] || v[9])
   	   	&& (v[8] || !v[9]) && (!v[9] || !v[10])
   	   	&& (v[9] || v[11]) && (v[10] || v[11])
   	   	&& (v[12] || v[13]) && (v[13] || !v[14])
   	   	&& (v[14] || v[15])) 
	{
		res++;
		printf ("%d) %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", id,
			v[0],v[1],v[2],v[3],v[4],v[5],v[6],v[7],v[8],v[9],
			v[10],v[11],v[12],v[13],v[14],v[15]);
		fflush(stdout);

	}

	return res;
}
