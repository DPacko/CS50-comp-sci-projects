#include <cs50.h>
#include <stdio.h>

int main(void)
{
    printf("minutes: ");
    int minutes = get_int();
    int bottles = (12*minutes);
    printf("Minutes: %i\n", minutes);
    printf("Bottles: %i\n", bottles);
}
