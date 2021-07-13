#include <stdio.h>
#include <cs50.h>

int main(void)

{

    int height = 0;

    while (height > 8 || height < 1)
    {
        height = get_int("What is the height?");
    }



    for (int i = 0; i < height; i++)            //height
    {
        for (int k = 1; k < height - i; k++)     //spaces before hashtags
        {
            printf(" ");
        }
                
        for (int j = 0; j <= i; j++)            //hashtags     
        {
            printf("#");
        }
        
        printf("  ");
        
        for (int m = 0; m <= i; m++)
        {
            printf("#");
        }
    

        printf("\n");
    }    
    
}