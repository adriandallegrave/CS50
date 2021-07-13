#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{

// Getting a valid card number
    long inputcard = 0;

    while (inputcard <= 0)
    {
        inputcard = get_long("What's your card number?");
    }


// Executing Luhn’s Algorithm - doubling sum of second to last digits and adding others
    int even = 0;
    int odd = 0;
    int digit = 0;
    long card = inputcard;
    int odd2 = 0;

    for (int i = 0; i < 16; i++)
    {
        digit = card % 10;
        card = card - digit;
        card = card / 10;
        
        if (i % 2 != 0)
        {
            digit = digit * 2;
            odd2 = digit % 10;
            odd = odd + odd2;
            digit = digit - odd2;
            digit = digit / 10;
            odd = odd + digit;
        }
        else
        {
            even = even + digit;
        }
        
    }
    
    int check = odd + even;
    check = check % 10;



// Getting first digits to know which company
    card = inputcard;
    card = round(card / 1000000000000);


// Printing results
    if (check != 0)
    {
        printf("INVALID\n"); 
    }    
 
    else
    {
        if (card == 4)
        {
            printf("VISA\n");
        }
        else
        {
            if (card >= 4000 && card <= 4999)
            {
                printf("VISA\n");
            }
            else 
            {
                if (card >= 5100 && card <= 5599)
                {
                    printf("MASTERCARD\n");
                }
                else
                {
                    if (card >= 340 && card <= 349)
                    {
                        printf("AMEX\n");
                    }
                    else
                    {
                        if (card >= 370 && card <= 379)
                        {
                            printf("AMEX\n");
                        }
                        else
                        {
                            printf("INVALID\n");
                        }
                    }
                }
            }
        }
    }
    
}