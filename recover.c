#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if(argc != 2)
    {
        fprintf(stderr, "Usage: ./recover filename\n");
        return 1;
    }

    // remember filename
    char* brokefile = argv[1];

    // declare string with # of characters for name of each jpg file
    char filename[8];

    // open brokefile for reading
    FILE* img = fopen(brokefile, "r");
    if(img == NULL)
    {
        fprintf(stderr, "Couldn't be opened\n");
        return 2;
    }

    BYTE buffer[512];

    // for (i = 0; i < 512; i++)
    // {
    //     fscanf(img, "%1d", &buffer[i]);
    // }

    FILE* newf = NULL;

    // set counter to keep track of each file that contains 1 block of 512 bytes each
    int count = 0;

    // repeat steps below until end of memory card
        while(fread(&buffer, 512, 1, img) == 1)
        {


            // check to see if 1st 3 bytes of each block are JPEG bytes
            if(buffer[0] == 0xff
            && buffer[1] == 0xd8
            && buffer[2] == 0xff
            && (buffer[3] & 0xf0) == 0xe0)
            {

                // close any previous files
                if(newf != NULL)
                {
                    fclose(newf);
                    count++;
                }
                // create new filename
                sprintf(filename, "%03i.jpg", count);

                // open file w/ new filename where images will go to jpgfiles
                newf = fopen(filename, "w");

                // write block of buffer to new file
                fwrite(buffer, 512, 1, newf);

            }
            else if(newf != NULL)
            {
                // write block of buffer to new file
                fwrite(&buffer, 512, 1, newf);
            }
        }
    fclose(newf);
    fclose(img);
    return 0;
}

