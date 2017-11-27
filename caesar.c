#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    if(argc != 2)
    {
        printf("not desired count of arguments\n");
        return 1;
    }
    int k = atoi(argv[1]);

    if(k < 0)
    {
        printf("can't be a negative number\n");
        return 1;
    }
    else
    {

        printf("plaintext: ");
        string p = get_string();
        printf("ciphertext: ");

        for(int i = 0; i < strlen(p); i++)
        {
            if(isalpha(p[i]))
            {
                if(isupper(p[i]))
                {
                    printf("%c", (((p[i] + k) - 65) % 26) + 65);
                }
                else if(islower(p[i]))
                {
                    printf("%c", (((p[i] + k) - 97) % 26) + 97);

                }
                else
                {
                    printf("%c", p[i]);
                }
            }
        }
    }
    printf("\n");
    return 0;
}