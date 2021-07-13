import sys
import cs50
import csv
import math


def main():

    # Check if program is executed correctly
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")

    # Open files
    seq = open(sys.argv[2], "r")
    seq = seq.read()

    # Create lists to check matches
    genes = ["AGATC", "TTTTTTCT", "AATG", "TCTAG", "GATA", "TATC", "GAAA", "TCTG"]
    genes_seq_count = [0, 0, 0, 0, 0, 0, 0, 0]
    length = [5, 8, 4, 5, 4, 4, 4, 4]

    # Populates genes_seq_count
    for i in range(8):
        genes_seq_count[i] = check(seq, genes, length, i)

    n = results(genes_seq_count, genes)
    print(n)


def check(seq, genes, length, index):

    # Calculate length of genetic sequence and create list of that size
    sequence_length = len(seq)
    repetitions = [0] * sequence_length

    # Put 1 on the location of the list that starts genetic sequence being checked
    for i in range(sequence_length):
        if seq[i:i + length[index]].count(genes[index]) == 0:
            repetitions[i] = 0
        else:
            repetitions[i] = 1

    # Put 5 in all numbers on the list until first 1
    for i in range(sequence_length):
        if repetitions[i] == 0:
            repetitions[i] = 5
        else:
            break

    # Put 5 in all numbers on the list that are in the genetic sequence being checked but doesn't start it
    for i in range(sequence_length):
        if repetitions[i] == 1:
            for j in range(length[index] - 1):
                repetitions[i + j + 1] = 5

    # Get rid of all the 5's
    repetitions[:] = (value for value in repetitions if value != 5)

    # Find largest sequence of the genetic sequence being checked
    largest = 0
    streak = 0
    current = 0

    if len(repetitions) != 0:
        for i in range(len(repetitions)):
            if repetitions[i] == 1:
                streak = 1
                current = current + 1
            else:
                streak = 0
                if current > largest:
                    largest = current
                current = 0
                
    # Returns the result                
    return largest

    
def results(genes_seq_count, genes):   

    # Transform database csv file in a 2d list
    table = list(csv.reader(open(sys.argv[1])))
    
    # Transform string numbers into ints
    for i in range(len(table) - 1):
        for j in range(len(table[0]) - 1):
            table[i + 1][j + 1] = int(table[i + 1][j + 1])
            
    # Create 2d list with zeroes
    cheatsheet = [[0 for i in range(len(table[0]))] for j in range(len(table))]

    # Create index list to know what to check
    gene_index = [0, 0, 0, 0, 0, 0, 0, 0]
    
    # Creates list with 1, 2, 3 and so on to know what order the results should be
    for i in range(len(table[0])):
        for j in range(8):
            if table[0][i] == genes[j]:
                gene_index[j] = i
           
    # Create list with the results in the right order                
    collumn = [0] * len(table[0])
    for i in range(len(table[0]) - 1):
        for j in range(8):
            if gene_index[j] == i + 1:
                collumn[i + 1] = genes_seq_count[j]
                
    # Compare results with database and populate with ones cheatsheet
    for i in range(len(table)):
        for j in range(len(table[0])):
            if table[i][j] == collumn[j]:
                cheatsheet[i][j] = 1

    # Sum cheatsheet in first collumn, if it equals the numbers of checks, the dna matches with this person
    for i in range(len(table)):
        for j in range(len(table[0])):
            cheatsheet[i][0] = cheatsheet[i][0] + cheatsheet[i][j]
    
    # Get the name of the person and send as result
    for i in range(len(table)):
        if cheatsheet[i][0] == len(table[0]) - 1:
            return table[i][0]
            break
    return "No match"


main()