#include "helpers.h"
#include <math.h>

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

// Detect edges xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
void edges(int height, int width, RGBTRIPLE image[height][width])
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



    for (int i = 1; i < bh - 1; i++)
    {
        for (int j = 1; j < bw - 1; j++)
        {
            float gxR = 0;
            float gxG = 0;
            float gxB = 0;
            float gyR = 0;
            float gyG = 0;
            float gyB = 0;
            float R = 0;
            float G = 0;
            float B = 0;
            
            gxB = -1 * bordered[i - 1][j - 1].rgbtBlue;
            gxB += -2 * bordered[i][j - 1].rgbtBlue;
            gxB += -1 * bordered[i + 1][j - 1].rgbtBlue;
            gxB += 1 * bordered[i - 1][j + 1].rgbtBlue;
            gxB += 2 * bordered[i][j + 1].rgbtBlue;
            gxB += 1 * bordered[i + 1][j + 1].rgbtBlue;
            gxB = gxB * gxB;

            gxR = -1 * bordered[i - 1][j - 1].rgbtRed;
            gxR += -2 * bordered[i][j - 1].rgbtRed;
            gxR += -1 * bordered[i + 1][j - 1].rgbtRed;
            gxR += 1 * bordered[i - 1][j + 1].rgbtRed;
            gxR += 2 * bordered[i][j + 1].rgbtRed;
            gxR += 1 * bordered[i + 1][j + 1].rgbtRed;
            gxR = gxR * gxR;

            gxG = -1 * bordered[i - 1][j - 1].rgbtGreen;
            gxG += -2 * bordered[i][j - 1].rgbtGreen;
            gxG += -1 * bordered[i + 1][j - 1].rgbtGreen;
            gxG += 1 * bordered[i - 1][j + 1].rgbtGreen;
            gxG += 2 * bordered[i][j + 1].rgbtGreen;
            gxG += 1 * bordered[i + 1][j + 1].rgbtGreen;
            gxG = gxG * gxG;
            
            gyB = -1 * bordered[i - 1][j - 1].rgbtBlue;
            gyB += -2 * bordered[i - 1][j].rgbtBlue;
            gyB += -1 * bordered[i - 1][j + 1].rgbtBlue;
            gyB += 1 * bordered[i + 1][j - 1].rgbtBlue;
            gyB += 2 * bordered[i + 1][j].rgbtBlue;
            gyB += 1 * bordered[i + 1][j + 1].rgbtBlue;
            gyB = gyB * gyB;

            gyR = -1 * bordered[i - 1][j - 1].rgbtRed;
            gyR += -2 * bordered[i - 1][j].rgbtRed;
            gyR += -1 * bordered[i - 1][j + 1].rgbtRed;
            gyR += 1 * bordered[i + 1][j - 1].rgbtRed;
            gyR += 2 * bordered[i + 1][j].rgbtRed;
            gyR += 1 * bordered[i + 1][j + 1].rgbtRed;
            gyR = gyR * gyR;

            gyG = -1 * bordered[i - 1][j - 1].rgbtGreen;
            gyG += -2 * bordered[i - 1][j].rgbtGreen;
            gyG += -1 * bordered[i - 1][j + 1].rgbtGreen;
            gyG += 1 * bordered[i + 1][j - 1].rgbtGreen;
            gyG += 2 * bordered[i + 1][j].rgbtGreen;
            gyG += 1 * bordered[i + 1][j + 1].rgbtGreen;
            gyG = gyG * gyG;
            
            R = gxR + gyR;
            G = gxG + gyG;
            B = gxB + gyB;
            
            R = sqrt(R);
            G = sqrt(G);
            B = sqrt(B);
            
            if (R > 255)
            {
                R = 255;
            }
            if (R < 0)
            {
                R = 0;
            }
            if (G > 255)
            {
                G = 255;
            }
            if (G < 0)
            {
                G = 0;
            }
            if (B > 255)
            {
                B = 255;
            }
            if (B < 0)
            {
                B = 0;
            }
            
            
            
            
            
            image[i - 1][j - 1].rgbtRed = round(R);
            image[i - 1][j - 1].rgbtGreen = round(G);
            image[i - 1][j - 1].rgbtBlue = round(B);
            
        }
    }
    
    return;
}
