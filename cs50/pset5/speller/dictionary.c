// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <cs50.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1170;
int dictionary_size = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false - xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx check
bool check(const char *word)
{
    int len = strlen(word);
    char lowered[LENGTH + 1];
    for (int i = 0; i < len; i++) 
    {
        lowered[i] = tolower(word[i]);
    }
    
    lowered[len] = '\0';
    int hash_number = hash(lowered);
    node *cursor = table[hash_number];
    
    while(cursor != NULL)
    {
        if (strcasecmp(cursor->word, lowered) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number - xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx hash
unsigned int hash(const char *word)
{

    char current[strlen(word)];
    for (int i = 0; i < strlen(word); i++)
    {
        current[i] = word[i];
    }

    for (int i = 0; i < strlen(word); i++)
    {
        if (current[i] > 64 && current[i] < 91)
        {
            current[i] += 32;
        }
    }

    // Assign number from 1 to 26 to first letter of word
    int first_letter = current[0];
    first_letter -= 96;

    // Assign number from 1 to 45 to lenght of word
    int size = strlen(word);

    // Multiply both numbers to get the index (mas 45 * 26 = 1170)
    int index = size * first_letter;


    return index;
}

// Loads dictionary into memory, returning true if successful else false xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx load
bool load(const char *dictionary)
{

    // Open dictionary and return false if it cannot open
    FILE *file = fopen(dictionary, "r");
    if (!file)
    {
        return false;
    }

    // Create char array to read the word into
    char current_word[LENGTH + 1];

    while (fscanf(file, "%s", current_word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return 1;
        }
        strcpy(n->word, current_word);
        int index = hash(current_word);
        n->next = table[index];
        table[index] = n;
        dictionary_size++;
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx size
unsigned int size(void)
{
    return dictionary_size;
}

// Unloads dictionary from memory, returning true if successful else false xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx unload
bool unload(void)
{

for (int i = 0; i < N; i++)
    {

        node *cursor2 = 0;
        cursor2 = table[i];
//        node *cursor2 = table[i];
        node *tmp = cursor2;

        while(cursor2 != NULL)
        {
            cursor2 = cursor2->next;
            free(tmp);
            tmp = cursor2;

        }
    }

    return true;
}

