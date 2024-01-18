// Да се напише програма која за даден природен број m ќе ги 
// испише сите броеви во даден опсег за кои сумата на цифрите изнесува m. 
// Опсегот се внесува од тастатура. Проверката да се подели подеднакво 
// помеѓу сите процеси. Дополнително на крај да се прикаже вкупниот 
// број на броеви кои го исполнуваат условот.

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define START 0
#define END 15
#define M 3

int main(int argc, char **argv)
{
    int comm_sz;
    int my_rank;

    int source_rank = 0;

    int local_counter;
    int global_counter;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    local_counter = 0;
    for (int i = my_rank; i < END; i += comm_sz)
    {
        int temp_num = i;
        int digit;
        int sum = 0;

        while (temp_num != 0)
        {
            digit = temp_num % 10;
            sum += digit;
            temp_num = temp_num / 10;
        }

        if (sum == M)
        {
            printf("%d, ", i);
            local_counter++;
        }
    }
    printf("\n");

    MPI_Reduce(&local_counter, &global_counter, 1, MPI_INT, MPI_SUM, source_rank, MPI_COMM_WORLD);

    if (my_rank == source_rank)
    {
        printf("Total numbers: %d \n", global_counter);
    }

    MPI_Finalize();

    return 0;
}