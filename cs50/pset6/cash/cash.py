import cs50
import math

while True:
    owed = cs50.get_float("Change owed: ")
    if owed > 0:
        break
    
cents = round(owed * 100, 0)
quarters = 0
dimes = 0
nickles = 0
pennies = 0
rest = 0

quarters = math.floor(cents / 25)
rest = cents % 25
dimes = math.floor(rest / 10)
rest = rest % 10
nickles = math.floor(rest / 5)
rest = rest % 5
pennies = rest

coins = math.floor(quarters + dimes + nickles + pennies)

print(coins)