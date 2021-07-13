#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float owed = -1;
    
    while (owed < 0)
    {
        owed = get_float("Change owed:");
    }
    
    int cents = round(owed * 100);
    
    int quarters = 0;
    int dimes = 0;
    int nickles = 0;
    int pennies = 0;
    int rest = 0;
    
    quarters = cents / 25;
    rest = cents % 25;
    dimes = rest / 10;
    rest = rest % 10;
    nickles = rest / 5;
    rest = rest % 5;
    pennies = rest;
    
    
//    printf("change = %i | quarters = %i | dimes = %i | nickles = %i | pennies = %i | rest = %i\n", cents, quarters, dimes, nickles, pennies, rest);
    
    int coins = quarters + dimes + nickles + pennies;
        
    printf("%i\n", coins);
    
}