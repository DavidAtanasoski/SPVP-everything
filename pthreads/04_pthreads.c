#include<stdio.h>
#include<pthread.h>

#define num_threads 4

void *work(void *ptr);

int main(int argc, char *argv[]) {
	int i, ret;
	int help_count, global_count = 0;
	pthread_t thread[num_threads];

	for(i = 0; i < num_threads; i++) {
		ret = pthread_create(&thread[i], NULL, work, (void *) i);

		if (ret) {
			printf("ERROR\n");
			return -1;
		}
	}

	for (i = 0; i < num_threads; i++) {
		pthread_join(thread[i], &help_count);
		global_count += help_count;
	}
	printf("The result is %d.\n", global_count);

	return 0;
}

void *work(void *ptr) {
	int id = (int *)ptr;
	int i, count = 0;
	for (i = id; i < 65536; i += num_threads) {
		count += check_circuit(id, i);
	}
	return (void *) count;
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
