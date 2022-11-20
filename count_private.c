#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

long *array;
long length = 100000000;
long count = 0;
long Number_Of_Threads;
long *Private_Thread_Sum;

void *count1s(void *thread_ID)
{
    long i;
    long ID = (long)thread_ID;
    long sub_array_per_thread = length / Number_Of_Threads;
    long start_index = ID * sub_array_per_thread;
    long end_index = start_index + sub_array_per_thread;
    long private_sum=0;

    // some entries may be left out because of integer division
    // so we assign them to the last thread.
    if (Number_Of_Threads - ID == 1)
    {
        end_index = length;
    }
    for (i = start_index; i < end_index; i++)
    {
        if (array[i] == 1)
        {
            private_sum++;
        }
    }
    *(Private_Thread_Sum+ID)=private_sum;
}
int main()
{
    Number_Of_Threads = 64;

    int CorrectCount = 0;
    clock_t start, end;
    double total_time;

    array = (long *)malloc(length * sizeof(long));
    Private_Thread_Sum = (long *)malloc(Number_Of_Threads * sizeof(long));


    start = clock();
    for (int i = 0; i < length; i++)
    {
        array[i] = rand() % 5 + 1;
    }
    for (int i = 0; i < length; i++)
    {
        if (array[i] == 1)
            CorrectCount++;
    }
    end = clock();
    total_time = ((double)(end - start)) / CLOCKS_PER_SEC;


    printf("The Squential program took %.3f seconds\n", total_time);
    printf("The Real count is= %ld\n", CorrectCount);


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

    for(int i=0;i<Number_Of_Threads;i++){
        count+=*(Private_Thread_Sum+i);
    }

    end = clock();

    total_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Took %f seconds with %d threads\n", total_time, Number_Of_Threads);
    printf("Count after excuting %ld Threads is= %ld\n", Number_Of_Threads, count);

    free(array);
    free(Private_Thread_Sum);
    return 0;
}

/*
    What will be written here was supported by the data collected and will
    be included in the submission under Data.xlsx 

    _for sizes 100 and 10,000, the running time only went up, which indicates
    that threading for such small sizes is not worth the overhead.

    _for size 1,000,000, we notice a drop in the running time up to 8 threads,
    but when we increase further, we get the same pattern as the one we got 
    for the previous sizes, we include that adding more threads doesn't 
    guarantee a better running time, we have to carefully choose the number of
    threads to ensure that we get an improved performance.

    _ for 10 million, 100 million and 1 billion, we successfully lower the 
    running time until we reach 32 threads, where increasing the number of 
    threads whon't really make a difference.
*/