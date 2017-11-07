#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float change;
    int count = 0;

    do
    {
        printf("How much change is owed?: ");
        change = get_float();
        if(change == 0)
        {
            return 0;
        }
    }
    while(change <0);

    change = round(change*100);

    while(change >= 25)
    {
        count++;
        change = (change - 25);
    }

    while(change >= 10)
    {
        count++;
        change = (change - 10);
    }

    while(change >= 05)
    {
        count++;
        change = (change - 05);
    }

    while(change >= 01)
    {
        count++;
        change = (change - 01);
    }

printf("You get back %i coins\n", count);
return 0;
}

