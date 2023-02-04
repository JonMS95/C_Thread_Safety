#include <stdio.h>
#include <pthread.h>    // This library is mandatory when working with threads.
#include <stdint.h>
#include <time.h>

#define BIG_NUM 1000000000UL
uint32_t counter = 0;

void count_until_BIG_NUM()
{
    for(int i = 0; i < BIG_NUM; i++)
    {
        counter++;
    }
}

/*
@brief Main function. Program's entry point.
*/
int main(int argc, char** argv)
{
    clock_t start_time = clock();
    // Let's say that we want to count until BIG_NUM twice:
    count_until_BIG_NUM();
    count_until_BIG_NUM();
    double total_time = ((double)(clock() - start_time)) / CLOCKS_PER_SEC;
    printf("It took the program %f seconds to execute.\r\n", total_time);
    return 0;
}
