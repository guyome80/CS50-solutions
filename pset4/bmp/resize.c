/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes a bitmap image, just because I want too... Ah ah ah ah !!!
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage of program
    if (argc != 4)
    {
        printf("Usage: ./resize factor infile outfile\n");
        return 1;
    }
    
    // remeber factor of resizement
    int f = atoi(argv[1]);
    
    // checks factor proper range
    if ((f < 1) || (f > 100))
    {
        printf("Usage: factor must be an integer between 1 and 100\n");
        return 2;
    }
    
    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }

    // bitmap input file infos 
    int inputWidth = bi.biWidth;
    int inputHeight = bi.biHeight; 
    bi.biWidth = bi.biWidth * f;
    bi.biHeight = bi.biHeight * f;
    
    // padding for scanlines
    int inputPadding = (4 - (inputWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int outputPadding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // calculation of bfSize
    bi.biSizeImage = abs(bi.biHeight) * (bi.biWidth * sizeof(RGBTRIPLE) + outputPadding);
    bf.bfSize = (bi.biSizeImage + 54);
    
    // write new outile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    
    // write new outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(inputHeight); i < biHeight; i++)
    {
        //iterate factor times over scanline
        for (int y = 0; y < f; y++) 
            {    
                // go the beginning of the ith line 
                fseek(inptr, (54 + ((inputWidth * 3 + inputPadding) * i)), SEEK_SET);
                
                // iterate over pixels in scanline
                for (int j = 0; j < inputWidth; j++)
                {
                    // temporary storage
                    RGBTRIPLE triple;
            
                    fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                    
                    // write RGB triple to outfile with a factor loop
                    for (int x = 0; x < f; x++)
                    {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
                }
                
                // add it padding back to new outfile
                for (int k = 0; k < outputPadding; k++)
                {
                    fputc(0x00, outptr);
                }
            }
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
