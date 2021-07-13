import sqlite3
import csv
import sys


if (len(sys.argv) != 2):
    sys.exit("Usage: roster.py house_name")

housename = sys.argv[1].lower()

houses = ["slytherin", "gryffindor", "ravenclaw", "hufflepuff"]

if housename.lower() not in houses:
    sys.exit("provide house name: Gryffindor, Hufflepuff, Slytherin or Ravenclaw.")

sqlite_file = "students.db"
opened = sqlite3.connect(sqlite_file)

cursor = opened.cursor()

cursor.execute('SELECT first, middle, last, birth FROM students WHERE lower(house) = "{}" ORDER BY last, first;'.format(housename))

houseroster = cursor.fetchall()

for row in houseroster:

    if not row[1]:
        print("{} {}, born {}".format(row[0], row[2], row[3]))
    else:
        print("{} {} {}, born {}".format(row[0], row[1], row[2], row[3]))

opened.close()