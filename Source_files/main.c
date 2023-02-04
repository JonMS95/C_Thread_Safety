#include <stdio.h>
#include <pthread.h>    // This library is mandatory when working with threads.
#include <stdint.h>
#include <time.h>

#define BIG_NUM 1000000000UL
uint32_t counter = 0;

void* count_until_BIG_NUM(void* arg)
{
    for(int i = 0; i < BIG_NUM; i++)
    {
        counter++;
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
    // a single assembly operation. Actually, it gets the value stored into the variable to increment, it
    // adds one, and then overwrites the value of the variable, accessing the original variable's memory address.
    // For example, if two threads try to increment the same variable at the same time, they will get the
    // original variable's value, then increment it, and then overwrite the original variable. If this process
    // is performed at the same time in both threads, then the original variable will only be incremented once
    // instead of twice.

    return 0;
}
