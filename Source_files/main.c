#include <stdio.h>
#include <pthread.h>    // This library is mandatory when working with threads.
#include <stdint.h>
#include <time.h>

//#define BIG_NUM 1000000000UL
#define BIG_NUM 1000000UL
uint32_t counter = 0;

void* count_until_BIG_NUM(void* arg)
{
    for(int i = 0; i < BIG_NUM; i++)
    {
        counter++;
    }
    return NULL;
}

// The variable below is the mutex initializer, which is the variable that
// starts the MUTEX by using the default values.
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* count_until_BIG_NUM_MUTEX(void* arg)
{
    for(int i = 0; i < BIG_NUM; i++)
    {
        // The critical code section starts here, so the mutex gets locked. If another thread is found at this same
        // line of code, it will have to wait here until the mutex gets unlocked.
        pthread_mutex_lock(&lock);
        counter++;
        // Once the operation has been performed, the mutex gets unlocked.
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

/*
@brief Main function. Program's entry point.
*/
int main(int argc, char** argv)
{
    clock_t start_time = clock();
    // Let's say that we want to count until BIG_NUM twice:
    count_until_BIG_NUM(NULL);
    count_until_BIG_NUM(NULL);
    double total_time = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;
    printf("It took the program %f seconds to execute.\r\n", total_time);
    printf("Counter value: %d.\r\n", counter);
    // In this first case, the counter is going to reach 2000000000, as it's going to be constantly
    // incremented by both threads.

    // Let's see what happens if the function is executed twice again, but using two different threads
    // instead of just a single one:
    counter = 0;
    start_time = clock();
    
    pthread_t newThread;
    pthread_create(&newThread, NULL, count_until_BIG_NUM, NULL);
    count_until_BIG_NUM(NULL);
    pthread_join(newThread, NULL);

    total_time = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;
    printf("If two different threads are used, then the programs takes %f seconds to run.\r\n", total_time);
    printf("Counter value: %d.\r\n", counter);
    // In this second case, the counter's value gets too big. On the other hand, the performance
    // dropped dramatically, as it takes much longer to finish. Why does this happen?
    // That's not cool, definitely. On top of that, the answer is different each time the code is executed,
    // which means that is not even consistent whatsoever.

    // What's happening is that the "++" operator does not only increment the variable, so it's not equal to
    // a single assembler operation. Actually, it gets the value stored into the variable to increment, it
    // adds one, and then overwrites the value of the variable, accessing the original variable's memory address.
    // For example, if two threads try to increment the same variable at the same time, they will get the
    // original variable's value, then increment it, and then overwrite the original variable. If this process
    // is performed at the same time in both threads, then the original variable will only be incremented once
    // instead of twice. In the second case shown above, the code is known as "non-thread-safe".

    // What we have just seen is also known as a race condition. It's called like that because it resembles
    // a couple of persons who are looking forward to perform the operation faster than the other. In this case,
    // we will look for a solution that lets only one of those threads modify the variable value each time, thus,
    // if one of them is performing the operation, the other one cannot do it and so on. This issue can be solved
    // by using MUTEX (which stands for MUTual EXclusion). It's a way for a thread to tell the others that it is
    // the only one ruling the system at that time.

    counter = 0;
    start_time = clock();

    pthread_create(&newThread, NULL, count_until_BIG_NUM_MUTEX, NULL);
    count_until_BIG_NUM_MUTEX(NULL);
    pthread_join(newThread, NULL);

    total_time = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;
    printf("If two different MUTEX locked threads are used, then the programs takes %f seconds to run.\r\n", total_time);
    printf("Counter value: %d.\r\n", counter);

    // As we can see, the counter has been properly incremented this time, however, it takes much more time
    // to run. So, why does it take so much time to get to an end? Well, the answer lies in the slight differences
    // between concurrency and parallelism.
    // When two processes or threads run in parallel, they are actually doing work at the same time.
    // Parallelism relies on hardware support, like multiple cores or some kind of special purpose coprocessor.
    // If no additional core exists within the current computer's architecture, then the way multiple threads
    // execute is quite different. Processes or threads cannot run at the same time, so they have to be
    // multiplexed, it's to say, each thread / process is being executed only for a short period of time before
    // switching to the other process / thread. If it switches back and forth fast enough, the multiplexing
    // may not even be noticeable for the user, which does not happen in the current case. To sum up, the way
    // the mutex threads above are executing is concurrent, but it's not parallel at all.
    // This happens because these processes are forced not to be parallel, as only one of them is able to access
    // the memory address where the "counter" variable is found at the same time.

    return 0;
}
