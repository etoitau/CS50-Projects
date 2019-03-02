// CS50 Problem Set 3
// resize
// Scales a bitmap image by factor
//
// by: Kyle Chatman
// on: 190226

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int xytobyte(int x, int y, int padding, int width);

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // get factor f
    float f = atof(argv[1]);
    if (f == 0)
    {
        fprintf(stderr, "Invalid factor %s.\n", argv[1]);
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // open output file for write
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 1;
    }


    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER inbf;
    fread(&inbf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER inbi;
    fread(&inbi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (inbf.bfType != 0x4d42 || inbf.bfOffBits != 54 || inbi.biSize != 40 ||
        inbi.biBitCount != 24 || inbi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // Initialize outfile with infile's header, since most will be same
    BITMAPFILEHEADER outbf = inbf;
    BITMAPINFOHEADER outbi = inbi;


    // if f is 1, this should just be a copy function
    if (f == 1)
    {

        // write outfile's BITMAPFILEHEADER
        fwrite(&inbf, sizeof(BITMAPFILEHEADER), 1, outptr);

        // write outfile's BITMAPINFOHEADER
        fwrite(&inbi, sizeof(BITMAPINFOHEADER), 1, outptr);

        // determine padding for scanlines
        int padding = (4 - (inbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

        // iterate over infile's scanlines
        for (int i = 0, biHeight = abs(inbi.biHeight); i < biHeight; i++)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < inbi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }

            // skip over padding, if any
            fseek(inptr, padding, SEEK_CUR);

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding; k++)
            {
                fputc(0x00, outptr);
            }
        }
    }

    // else we're resizing
    else
    {
        // determine new dimensions
        outbi.biWidth = ceil(f * inbi.biWidth) ;
        outbi.biHeight = ceil(f * abs(inbi.biHeight)) * abs(inbi.biHeight) / inbi.biHeight;

        // determine padding
        int inpadding = (4 - (inbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
        int outpadding = (4 - (outbi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

        // set new image size
        outbi.biSizeImage = ((sizeof(RGBTRIPLE) * outbi.biWidth) + outpadding) * abs(outbi.biHeight);

        // set new file size
        outbf.bfSize = outbi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

        // write outfile's BITMAPFILEHEADER
        fwrite(&outbf, sizeof(BITMAPFILEHEADER), 1, outptr);

        // write outfile's BITMAPINFOHEADER
        fwrite(&outbi, sizeof(BITMAPINFOHEADER), 1, outptr);

        // set outpointer to start of image
        fseek(outptr, 54, SEEK_SET);

        // Height can be positive or negative, get abs for loop
        int absht = abs(outbi.biHeight);

        // draw image by iterating over columns and rows
        for (int r = 0; r < absht; r++)
        {
            for (int c = 0; c < outbi.biWidth; c++)
            {
                // temporary storage
                RGBTRIPLE triple;

                int inx = floor(c / f);
                int iny = floor(r / f);

                // x, y in outfile is is c, r. Get corresponding byte location in infile and use to set inptrr location to byte we want to sample
                fseek(inptr, xytobyte(inx, iny, inpadding, inbi.biWidth) + 54, SEEK_SET);

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write to outfile
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            // add padding at end of row
            for (int i = 0; i < outpadding; i++)
            {
                fputc(0x00, outptr);
            }
        }


    }


    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

// returns byte position corresponding to given x, y coordinate in bitmap
int xytobyte(int x, int y, int padding, int width)
{
    int byteloc = y * (width * sizeof(RGBTRIPLE) + padding) + x * sizeof(RGBTRIPLE);
    return byteloc;
}
