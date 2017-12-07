/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm
    if(n < 0)
    {
        return false;
    }
    // define minimum and maximum value in array
    int min = 0;
    int max = n - 1;
    // go into a while loop that finds value; it'll continue to search for value as long as minimum is less than/equal to maximum.
    while(min <= max)
    {
        int mid = (min + max) / 2;
        if(value == values[mid])
        {
            return true;
        }
        else if(value < values[mid])
        {
            max = mid - 1;
        }
        else if(value > values[mid])
        {
            min = mid + 1;
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
   // TODO: implement a sorting algorithm
    if(n < 2)
    {
        return;
    }
    int min;
    // go through each char in array using a for loop
    for(int i = 0; i < (n - 2); i++)
    {
        // declare minimum value to first index in array
        min = i;
        // go through each char in array to see if it's the smallest number using for loop
        for(int k = i + 1; k < n; k++)
        {
            // check to see what # from two numbers beside each other is smaller
            if(values[k] < values[min])
            {
                min = k;
            }
        }
        if(values[min] < values[i])
            {
                int temp = values[min];
                values[min] = values[i];
                values[i] = temp;
            }
    }
    return;
}

