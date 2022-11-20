#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

long *array;
long length = 100000000;
long count = 0;
long Number_Of_Threads;
pthread_mutex_t mutex;

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
    pthread_mutex_lock(&mutex);
    for (i = start_index; i < end_index; i++)
    {
        if (array[i] == 1)
        {
            count++;
        }
    }
    pthread_mutex_unlock(&mutex);
}
int main()
{
    Number_Of_Threads = 12;

    int CorrectCount = 0;
    clock_t start, end;
    double total_time;

    array = (long *)malloc(length * sizeof(long));


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
    pthread_mutex_init(&mutex,NULL);

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

    free(array);
    return 0;
}

/*
    What will be written here was supported by the data collected and will
    be included in the submission under Data.xlsx 

    _For sizes 100 and 10,000: Threading using locks did nothing but add to 
    the running time, and this was the case for 1,2,4,8,16,32 and 64 threads
    we may conclude that threading is simply not worth it for data this 
    small.

    _For sizes 1,10, and 100 million length arrays, we notice an interesting pattern, the 
    trend that we saw in the previous sizes continued when using up to 16 
    threads, after that, we notice a drop by approximately 65% in the 
    running time, but as we increase the number of threads more, the trend 
    becomes almost constant, and is still at a higher running time than 
    using no threads.

    _For 1 billion, as we use more threads, it only gets worse, So we can inply 
    that threading and using locks is inefficient for such large sizes and with 
    so many threads, note that I didn't include the results for 32 and 64 threads
    because my computer was getting too slow. 

    Conclusion: Threading with locking added nothing but and overhead to the 
    performance, unlike private sums that was done in count_private.
*/