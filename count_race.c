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


/*
    What will be written here was supported by the data collected and will
    be included in the submission under Data.xlsx 

    _for sizes 100, 10,000, and 1 million, in addition to not getting the
    right value because of the race condition, we add an overhead to the 
    running time.

    _for 10 million, we get a 40% drop in the running time as soon as we use 
    2 threads, after that, the trend is constant, with an average running
    time of 0.0345 seconds, WE ARE STILL GETTING THE WRONG ANSWER.

    _for 100 million, we obtain a drop in the running time until we reach
    32 threads, but after using 64, we lose the time just to get back to 
    the same time we had when we used 4,8, and 16 threads.

    _for 1 billion, the running time only increases to reach a constant of 
    almost 3.5 seconds.

    _Conclusion: Threading using this specific technique not only gives us 
    the wrong answer, but adds more to the running time than what is desired.

*/