#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int main(void)
{

//Getting text input

    string text = get_string("Text: ");

//Counting letters, words and sentences

    int n = 0;
    int letters = 0;
    int words = 1;
    int sentences = 0;

    while (text[n] != '\0')
    {
        if ((int) text[n] >= 65 && (int) text[n] <= 122)
        {
            letters++;
        }
        if ((int) text[n] >= 91 && (int) text[n] <= 96)
        {
            letters--;
        }
        if ((int) text[n] == 32)
        {
            words++;
        }
        if ((int) text[n] == 33 || (int) text[n] == 46 || (int) text[n] == 63)
        {
            sentences++;
        }

        n++;
    }

//Calculating and showing grade

    float L = 100 * (float) letters / (float) words;
    float S = 100 * (float) sentences / (float) words;
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        if (index >= 16)
        {
            printf("Grade 16+\n");
        }

        else

        {
            printf("Grade %i\n", index);
        }

    }

}