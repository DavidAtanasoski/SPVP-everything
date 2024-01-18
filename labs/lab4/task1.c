#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
    
#define MAX_LINES 20
#define MAX_CHARACTERS 512

char *buffer;
long length;
int line_counter;

char lines[MAX_LINES][MAX_CHARACTERS];

void *countIntegersPerLines(void *ptr);
void readFile(char *);
void getLines();

struct thread_args
{
    int thread_id;
    int thread_number;
};

int main(int argc, char **argv)
{
    if (argc != 2) 
    {
        printf("Not satisfying arguments!\n");
        return -1;
    }

    int num_threads = atoi(argv[1]);

    readFile("numbers.txt");
    //printf("%s", buffer);
    getLines();

    pthread_t *threads = (pthread_t *) malloc(num_threads * sizeof(pthread_t));
    struct thread_args *args = (struct thread_args *) malloc(num_threads * sizeof(struct thread_args));

    for (int i = 0; i < num_threads; i++)
    {
        args[i].thread_id = i;
        args[i].thread_number = num_threads;

        int ret = pthread_create(&threads[i], NULL, countIntegersPerLines, &args[i]);

        if (ret)
        {
            printf("ERROR!\n");
            return -1;
        }
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

void readFile(char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        printf("Error opening the file!\n");
        return;
    }
    else 
    {
        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = malloc(length);

        if (buffer)
        {
            fread(buffer, 1, length, file);
        }
        fclose(file);
    }
}

void getLines()
{
    char* line = strtok(buffer, "\n");
    line_counter = 0;

    while (line != NULL) 
    {
        strcpy(lines[line_counter], line);
        line_counter++;
        line = strtok(NULL, "\n");
    }
}

void *countIntegersPerLines(void *ptr)
{
    struct thread_args *args = ptr;

    int total_lines = line_counter;

    int lines_per_thread = total_lines / args->thread_number;
    int remainder_lines = total_lines % args->thread_number;

    int start_line = args->thread_id * lines_per_thread + (args->thread_id < remainder_lines ? args->thread_id : remainder_lines);
    int end_line = start_line + lines_per_thread + (args->thread_id < remainder_lines ? 1 : 0);

    // printf("thread %d start %d end %d\n", args->thread_id, start_line, end_line);
    for (int i = start_line; i < end_line; i++)
    {
        char *line_ptr = lines[i];
        // printf("thread: %d line: %s\n", args->thread_id, lines[i]);
        
        int number;        
        int offset = 0;
        int move = 0;
        int integers_count = 0;

        while (sscanf(line_ptr + move, "%*[^0-9]%d%n", &number, &offset) == 1)
        {
            integers_count++;
            move += offset;
        }

        if (integers_count > 0)
        {
            printf("Thread %d found %d integer(s) on line %d\n", args->thread_id, integers_count, i + 1);
        }
    }

    pthread_exit(NULL);
}