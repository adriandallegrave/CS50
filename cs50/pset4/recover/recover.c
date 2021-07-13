#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    // Check if it has exactly one command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Check if file opens
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("File cannot open.\n");
        return 1;
    }

    // Creates a buffer to read chunks of 512 bytes and needed integers
    unsigned char buffer[512];
    int jpgcounter = 0;
    int jpgfound = 0;

    // Creates img and assign NULL so it can be closed on the loop
    FILE *img = NULL;
    
    // Read chunks
    while (fread(&buffer, 512, 1, file) == 1) 
    {
        // If found start of JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If this is not the first JPEG, close the last one that was being written
            if (jpgfound == 1)
            {
                fclose(img);
            }
            
            // If it is the first one, change int 
            if (jpgfound == 0)
            {
                jpgfound = 1;
            }
            
            // Create a file with the right name and open it
            char filename[8];
            sprintf(filename, "%03i.jpg", jpgcounter);
            img = fopen(filename, "w");
            jpgcounter++;
        }

        // Write in the file unless it hasn't found the first JPEG yet
        if (jpgfound == 1)
        {
            fwrite(&buffer, 512, 1, img);
        }
    }
}






