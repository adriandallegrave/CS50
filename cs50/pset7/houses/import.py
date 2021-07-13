import sqlite3
import csv
import sys


# Check if there's exactly one command
if (len(sys.argv) != 2):
    sys.exit("Usage: import.py file.csv")

file = sys.argv[1]

# Open DB file
sqlite_file = "students.db"
opened = sqlite3.connect(sqlite_file)

cursor = opened.cursor()

# Open the csv file
with open(file, "r") as characters:

    reader = csv.DictReader(characters)

    for row in reader:
        wizards = []

        for part in row["name"].split(" "):
            wizards.append(part)

        wizards.append(row["house"])
        wizards.append(row["birth"])

        if (len(wizards) == 5):
            cursor.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", wizards[:5])

        if (len(wizards) == 4):
            cursor.execute("INSERT INTO students (first, last, house, birth) VALUES(?, ?, ?, ?)", wizards[:4])

opened.commit()
opened.close()
