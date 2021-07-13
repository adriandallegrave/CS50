#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int k;

int main(int argc, string argv[])
{

//Check if there's only one command line
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

//Check if the command line has only numbers    
    int lenght = strlen(argv[1]);

    for (int i = 0; i < lenght; i++)
    {
        if ((char) argv[1][i] > 57 || (char) argv[1][i] < 48)
        {
            printf("Usage: ./caesar key\n");
            return 1;        
        }
    }

//converts the command line to an integer    
    int key = atoi(argv[1]);
    
//code

    string plaintext = get_string("plaintext: ");
    lenght = strlen(plaintext);

    printf("ciphertext: ");
    
    for (int i = 0; i < lenght; i++)
    {
        
        char chars = plaintext[i];
        char test = chars;
        
        if (chars >= 65 && chars <= 90)
        {
            test -= 65;
            test += key;
            test %= 26;
            test += 65;
            
            printf("%c", test);
        }
        
        else
        {
            
            if (chars >= 97 && chars <= 122)
            {
                test -= 97;
                test += key;
                test %= 26;
                test += 97;
            
                printf("%c", test);
            }
            
            else
            {
                printf("%c", chars);
            }
            
        }
        
    
        
    }
 
    printf("\n");
    return 0;
}