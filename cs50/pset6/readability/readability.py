import cs50

# Asking for input text and transforiming it in all lowers
text = cs50.get_string("Text: ")
text = text.lower()

# Counting sentences
period = "."
exclamation = "!"
question = "?"

sentences = text.count(period) + text.count(exclamation) + text.count(question)

# Counting words
spaces = " "

words = text.count(spaces) + 1

# Countig letters
letters = 0

for char in text:
    if char.isalpha():
        letters += 1

# Calculating grade
letters = (100 * letters) / words
sentences = (100 * sentences) / words
grade = 0.0588 * letters - 0.296 * sentences - 15.8
roundgrade = round(grade)

# Printing results
if roundgrade < 1:
    print("Before Grade 1")
elif roundgrade >= 16:
    print("Grade 16+")
else:
    print(f"Grade {roundgrade}")


