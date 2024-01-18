#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS 4

int main() 
{
    char *input = NULL;
    size_t input_len = 0;
    ssize_t input_size = 0;

    int isPalindrome = 1;
    int i;

    #pragma omp parallel num_threads(NUM_THREADS) reduction(&&:isPalindrome) private(i)
    {
        int thread_id = omp_get_thread_num();
        int threads_number = omp_get_num_threads();

        #pragma omp master
        {
            printf(">> ");
            input_size = getline(&input, &input_len, stdin);
            input[strcspn(input, "\n")] = '\0';
        }

        #pragma omp barrier

        int str_len = strlen(input);
        int block_size = str_len / threads_number;
        int start = thread_id * block_size;
        int end = (thread_id == threads_number - 1) ? str_len - 1 : start + block_size - 1;

        //printf("block = %d start = %d end = %d\n", block_size, start, end);

        for (i = start; i < end; i++)
        {
            //#pragma omp critical
            if (input[i] != input[str_len - i - 1])
            {
                isPalindrome = 0;
            }
        }
    }

    if (isPalindrome)
    {
        printf("Palindrome\n");
    }
    else 
    {
        printf("Not a palindrome\n");
    }

    return 0;
}

