#include <stdio.h>
#include <math.h>
#include <omp.h>

int main()
{
    int i;
    int j;
    int k;

    #pragma omp parallel for private(i, j, k)
    for (i = -32768; i < 32768; i++)
    {
        for (j = -32768; j < 32768; j++)
        {
            for (k = -32768; k < 32768; k++)
            {
                if (2 * i + j + 2 * k == 6)
                {
                    if (pow(i+1, 2) + pow(j-10, 2) + 2*k == 10)
                    {
                        printf("x = %d y = %d z = %d\n", i, j, k);
                    }
                }
            }
        }
    }

    return 0;
}