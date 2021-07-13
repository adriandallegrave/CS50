#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, string argv[])
{

//Check if there's only one command line
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

//Check if the command line has only 26 letters    
    int lenght = strlen(argv[1]);

    for (int i = 0; i < lenght; i++)
    {
        if ((char) argv[1][i] > 122 || (char) argv[1][i] < 65)
        {
            printf("Usage: Key must only contain letters.\n");
            return 1;        
        }
        if ((char) argv[1][i] > 90 && (char) argv[1][i] < 97)
        {
            printf("Usage: Key must only contain letters.\n");
            return 1;  
        }
        if (strlen(argv[1]) != 26)
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
    }

//Converts key to upper case    
    for (int i = 0; i < 26; i++)
    {
        argv[1][i] = toupper(argv[1][i]);
    }
    
//Checks if key repeats any letter
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    for (int i = 0; i < 26; i++)
    {
        int j = argv[1][i];
        j -= 65;
        if (alphabet[j] == 33)
        {
            printf("Key must not repeat any letter.\n");
            return 1;
        }
        alphabet[j] = 33;
    }
    
//Adjust key to relative value    

    int n = 0;
    int key[26];
    char alphabet2[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    for (int i = 0; i < 26; i++)
    {
        n = argv[1][i] - alphabet2[i];
        key[i] = n;

    }

//Ask for text to be changed
    string plaintext = get_string("plaintext: ");
    printf("ciphertext: ");
    
//Transform it acording to key    
    int print;
    int printkey;
    
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (plaintext[i] < 65 || plaintext[i] > 122)
        {
            printf("%c", plaintext[i]);
        }
        else
        {
            if (plaintext[i] < 97 && plaintext[i] > 90)
            {
                printf("%c", plaintext[i]);
            }
            else
            {
                print = plaintext[i];
                printkey = toupper(plaintext[i]);
                printkey -= 65;
                print += key[printkey];
                printf("%c", print);
                print = 0;
            }
        }
        
        
        
        
    }
    
    printf("\n");
    return 0;
}