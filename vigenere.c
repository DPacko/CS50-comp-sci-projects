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

    string k = (argv[1]);


    for(int i = 0; i < strlen(k); i++)
    {
        if(!isalpha(k[i]))
        {
            printf("not alphabetical\n");
            return 1;
        }
    }
            printf("plaintext: ");
            string p = get_string();
            printf("ciphertext: ");

            for(int j = 0; j < strlen(p); j++)
            {
                if(isalpha(p[j]))
                {
                    if(isupper(p[j]))
                    {
                        printf ("%c", ((((p[j] - 65) + (toupper(k[j % strlen(k)]) - 65) ) % 26 ) + 65));
                    }

                    if(islower(p[j]))
                    {
                        printf ("%c", ((((p[j] - 97) + (tolower(k[j % strlen(k)]) - 97) ) % 26 ) + 97));
                    }
                }
                else {
                    printf("%c", p[j]);
                }
            }

            printf("\n");
            return 0;
}