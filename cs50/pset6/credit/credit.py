import cs50
import math

card = cs50.get_string("Card: ")
size = len(card)
evens = 0
odds = 0
algo = 0

for i in range(size):
    current_dig = size - i - 1
    current_num = int(card[current_dig])
    
    if i % 2 == 0:
        evens += current_num
    else:
        current_num *= 2
        if current_num < 10:
            odds += current_num
        elif current_num == 10:
            odds += 1
        else:
            odds += 1
            current_num = current_num / 10
            current_num = current_num - 1
            current_num = current_num * 10
            odds += current_num
            
algo = odds + evens
first = int(card[0])
second = int(card[1])

if algo % 10 != 0:
    print("INVALID")
else:
    if size == 15 and first == 3:
        if second == 4 or second == 7:
            print("AMEX")
        else:
            print("INVALID")
    
    elif size == 16 and first == 5:
        if second > 0 and second < 6:
            print("MASTERCARD")
        else:
            print("INVALID")
    
    elif size == 16 and first == 4:
        print("VISA")
    
    elif size == 13 and first == 4:
        print("VISA")
    
    else:
        print("INVALID")
    
