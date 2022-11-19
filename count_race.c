#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

long *array;
long length = 100000000;
long count = 0;
int Number_Of_Threads;

void *count1s(void *thread_ID)
{
    long i;
    long ID = (long)thread_ID;
    long sub_array_per_thread = length / Number_Of_Threads;
    long start_index = ID * sub_array_per_thread;
    long end_index = start_index + sub_array_per_thread;

    //some entries may be left out because of integer division
    //so we assign them to the last thread.
    if (Number_Of_Threads - ID == 1)
    {
        end_index = length;
    }
    for (i = start_index; i < end_index; i++)
    {
        if (array[i] == 1)
        {
            count++;
        }
    }
}
int main()
{
    Number_Of_Threads = 10000;

    int CorrectCount = 0;
    clock_t start, end;
    double total_time;

    array = (long *)malloc(length * sizeof(long));

    for (int i = 0; i < length; i++)
    {
        array[i] = rand() % 5 + 1;
    }

    for (int i = 0; i < length; i++)
    {
        if (array[i] == 1)
            CorrectCount++;
    }

    // printf("The real count  took %.3f seconds\n",total_time);
    printf("The real count is= %ld\n", CorrectCount);

    pthread_t Thread[Number_Of_Threads];

    start = clock();

    for (int i = 0; i < Number_Of_Threads; i++)
    {

        pthread_create(&Thread[i], NULL, &count1s, (void *)i);
    }

    for (int i = 0; i < Number_Of_Threads; i++)
    {

        pthread_join(Thread[i], NULL);
    }

    end = clock();

    total_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Took %f seconds with %d threads\n", total_time, Number_Of_Threads);
    printf("Count after excuting %ld Threads is= %ld\n", Number_Of_Threads, count);
}