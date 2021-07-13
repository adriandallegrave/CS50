import cs50


while True:
    height = cs50.get_int("Height: ")
    if height <= 8 and height >= 1:
        break
    
spaces = height - 1

for i in range(height):
    print(f"{spaces * ' '}{(i + 1) * '#'}")
    spaces -= 1