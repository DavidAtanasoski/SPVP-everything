#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_THREADS 4

int main()
{
    // printf("%d\n", getpid());
    
    char *input;
    size_t input_len = 0;
    ssize_t input_size = 0;
    
    int i;
    int isPalindrome = 1;

    printf(">> ");
    input_size = getline(&input, &input_len, stdin);
    input[strcspn(input, "\n")] = '\0';
    
    int str_sz = strlen(input);

    #pragma omp parallel for num_threads(NUM_THREADS) private(i) reduction(&&: isPalindrome)
    for (i = 0; i < str_sz; i++)
    {
        //#pragma omp critical
        if (input[i] != input[str_sz - i - 1])
        {
            isPalindrome = 0;
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