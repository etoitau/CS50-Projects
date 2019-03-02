// CS50 Problem Set 3
// recover
// scans a file full of raw data and creates files of any jpgs it finds
//
// by: Kyle Chatman
// on: 190227
//
// Assumes:
// jpg will always start at beginning of 512 byte block
// each new jpg starts right after the last (start of next block)
// only jpgs in data

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];

    // initialize count of how many have been found
    int jpgcount = -1;

    // size oo block to read at a time, spec says 512
    int block = 512;

    // create buffer for holding blocks of data
    unsigned char *buffer;
    buffer = malloc(block);

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // set up to open output files
    char filename[8];
    FILE *img;

    // loop through data blocks
    while (1)
    {
        // read a block
        int nbytes = fread(buffer, 1, block, inptr);

        // checks for jpg signature
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // if one is already open, close it
            if (jpgcount >= 0)
            {
                fclose(img);
            }

            jpgcount++;

            // open new jpg file
            sprintf(filename, "%03i.jpg", jpgcount);
            img = fopen(filename, "w");
            if (img == NULL)
            {
                fprintf(stderr, "Could not open %s.\n", filename);
                return 1;
            }

        }

        // if at least one jpg has been found, we're in write mode
        if (jpgcount >= 0)
        {
            // write block to current jpg
            fwrite(buffer, nbytes, 1, img);
        }

        // Check if at end of file
        if (nbytes < block)
        {
            if (feof(inptr))
            {
                break;
            }
            else
            {
                fprintf(stderr, "Failed to read full block\n");
                return 1;
            }
        }

    }

    // close jpg when done
    fclose(img);

    // close infile
    fclose(inptr);

    // return memory
    free(buffer);

    // success
    return 0;
}
