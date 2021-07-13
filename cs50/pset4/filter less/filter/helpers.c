#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float avg = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            avg = avg / 3;
            image[i][j].rgbtBlue = round(avg);
            image[i][j].rgbtGreen = round(avg);
            image[i][j].rgbtRed = round(avg);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float sfblue = 0.272 * image[i][j].rgbtRed;
            sfblue += 0.534 * image[i][j].rgbtGreen;
            sfblue += 0.131 * image[i][j].rgbtBlue;
            int siblue = round(sfblue);
            if (siblue > 255)
            {
                siblue = 255;
            }

            float sfgreen = 0.349 * image[i][j].rgbtRed;
            sfgreen += 0.686 * image[i][j].rgbtGreen;
            sfgreen += 0.168 * image[i][j].rgbtBlue;
            int sigreen = round(sfgreen);
            if (sigreen > 255)
            {
                sigreen = 255;
            }

            float sfred = 0.393 * image[i][j].rgbtRed;
            sfred += 0.769 * image[i][j].rgbtGreen;
            sfred += 0.189 * image[i][j].rgbtBlue;
            int sired = round(sfred);
            if (sired > 255)
            {
                sired = 255;
            }

            image[i][j].rgbtBlue = siblue;
            image[i][j].rgbtGreen = sigreen;
            image[i][j].rgbtRed = sired;
        }
    }
    return;

}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    // Create a copy of the image called oldimage
    RGBTRIPLE oldimage[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            oldimage[i][j].rgbtBlue = image[i][j].rgbtBlue;
            oldimage[i][j].rgbtGreen = image[i][j].rgbtGreen;
            oldimage[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }

    // Get values from copied image and apply them in the correct inverted position
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int k = 0;
            k = width - j;
            k -= 1;

            image[i][j].rgbtBlue = oldimage[i][k].rgbtBlue;
            image[i][j].rgbtGreen = oldimage[i][k].rgbtGreen;
            image[i][j].rgbtRed = oldimage[i][k].rgbtRed;
        }
    }



    return;
}

// Blur image
// The new value of each pixel would be the average of the values of all of the pixels that are within 1 row and column of the original pixel (forming a 3x3 box)

//  typedef struct
//  {
//      BYTE  rgbtBlue;
//      BYTE  rgbtGreen;
//      BYTE  rgbtRed;
//  } __attribute__((__packed__))
//  RGBTRIPLE;

void blur(int height, int width, RGBTRIPLE image[height][width])
{

    int bw = width + 2;
    int bh = height + 2;

    // Create a copy of the image called bordered with a black border
    RGBTRIPLE bordered[bh][bw];

    // Painting new image black
    for (int i = 0; i < bh; i++)
    {
        for (int j = 0; j < bw; j++)
        {
            bordered[i][j].rgbtBlue = 0;
            bordered[i][j].rgbtGreen = 0;
            bordered[i][j].rgbtRed = 0;
        }
    }

    // Painting middle of the black image with image
    for (int i = 1; i < bh - 1; i++)
    {
        for (int j = 1; j < bw - 1; j++)
        {
            bordered[i][j].rgbtBlue = image[i - 1][j - 1].rgbtBlue;
            bordered[i][j].rgbtGreen = image[i - 1][j - 1].rgbtGreen;
            bordered[i][j].rgbtRed = image[i - 1][j - 1].rgbtRed;
        }
    }


    // Create a copy of the image called borderedbase with a black border
    RGBTRIPLE borderedbase[bh][bw];

    for (int i = 0; i < bh; i++)
    {
        for (int j = 0; j < bw; j++)
        {
            borderedbase[i][j].rgbtBlue = bordered[i][j].rgbtBlue;
            borderedbase[i][j].rgbtGreen = bordered[i][j].rgbtGreen;
            borderedbase[i][j].rgbtRed = bordered[i][j].rgbtRed;
        }
    }


    // Bluring the whole boarded picture
    for (int i = 1; i < bh - 1; i++)
    {
        for (int j = 1; j < bw - 1; j++)
        {
            float avgred = 0;
            float avggreen = 0;
            float avgblue = 0;
            int division = 9;
            int careful = 0;

            if (i == 1 || i == bh - 2)
            {
                division = division - 3;
                careful++;
            }
            if (j == 1 || j == bw - 2)
            {
                division = division - 3;
                careful++;
            }
            if (careful == 2)
            {
                division++;
            }


            avgred +=  borderedbase[i - 1][j - 1].rgbtRed + borderedbase[i - 1][j].rgbtRed + borderedbase[i - 1][j + 1].rgbtRed;
            avgred +=  borderedbase[i + 1][j - 1].rgbtRed + borderedbase[i + 1][j].rgbtRed + borderedbase[i + 1][j + 1].rgbtRed;
            avgred +=  borderedbase[i][j - 1].rgbtRed + borderedbase[i][j].rgbtRed + borderedbase[i][j + 1].rgbtRed;
            avgred = avgred / division;

            avggreen +=  borderedbase[i - 1][j - 1].rgbtGreen + borderedbase[i - 1][j].rgbtGreen + borderedbase[i - 1][j + 1].rgbtGreen;
            avggreen +=  borderedbase[i + 1][j - 1].rgbtGreen + borderedbase[i + 1][j].rgbtGreen + borderedbase[i + 1][j + 1].rgbtGreen;
            avggreen +=  borderedbase[i][j - 1].rgbtGreen + borderedbase[i][j].rgbtGreen + borderedbase[i][j + 1].rgbtGreen;
            avggreen = avggreen / division;

            avgblue +=  borderedbase[i - 1][j - 1].rgbtBlue + borderedbase[i - 1][j].rgbtBlue + borderedbase[i - 1][j + 1].rgbtBlue;
            avgblue +=  borderedbase[i + 1][j - 1].rgbtBlue + borderedbase[i + 1][j].rgbtBlue + borderedbase[i + 1][j + 1].rgbtBlue;
            avgblue +=  borderedbase[i][j - 1].rgbtBlue + borderedbase[i][j].rgbtBlue + borderedbase[i][j + 1].rgbtBlue;
            avgblue = avgblue / division;

            bordered[i][j].rgbtBlue = round(avgblue);
            bordered[i][j].rgbtGreen = round(avggreen);
            bordered[i][j].rgbtRed = round(avgred);
        }
    }


    // Making image the same as the center of boardered blured picture
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = bordered[i + 1][j + 1].rgbtBlue;
            image[i][j].rgbtGreen = bordered[i + 1][j + 1].rgbtGreen;
            image[i][j].rgbtRed = bordered[i + 1][j + 1].rgbtRed;
        }
    }


    return;
}
