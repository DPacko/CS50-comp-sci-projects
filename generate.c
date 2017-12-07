/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX], one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */

#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // if argument count does not equal 2 and does not equal 3, then leave an error message.
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // convert first array in argv to an integer, naming this new integer, n
    int n = atoi(argv[1]);

    // if user passes argument count of 3, then call srand48 and convert argv[2] to an
    // integer and cast it into a long integer, else call it with time casted to a long integer to get different sequences of #s;
    // This is necessary to seed/initialize drand48
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    // call drand48 n times; multiply value returned by drand48 and cast it to an integer
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
