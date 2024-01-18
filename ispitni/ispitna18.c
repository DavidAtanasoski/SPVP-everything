/*
    Да се напише програма која за даден природен број m ќе 
    ги испише сите броеви во даден опсег за кои сумата на 
    цифрите изнесува m. Опсегот се внесува од тастатура. 
    Проверката да се подели подеднакво помеѓу сите процеси. 
    Дополнително на крај да се прикаже вкупниот број на броеви 
    кои го исполнуваат условот.
*/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 4

int main()
{
    int m;

    int lower_bound;
    int upper_bound;

    int total = 0;

    scanf("%d %d %d", &m, &lower_bound, &upper_bound);

    for (int i = lower_bound; i < upper_bound; i++)
    {
        printf("%d ", i);
    }
    printf("\n");

    #pragma omp parallel for num_threads(NUM_THREADS) reduction(+:total)
    for (int i = lower_bound; i < upper_bound; i++)
    {
        int number = i;
        int digits_sum = 0;

        while (number != 0)
        {
            int digit = number % 10;
            digits_sum += digit;
            number /= 10; 
        }

        if (digits_sum == m)
        {
            total++;
        }
    }
    printf("Total: %d\n", total);

    return 0;
}