/* 
Implements a dictionary's functionality. In particular, the functions:
hash: Hashes a word.
load: Loads a dictionary .txt file into memory as a hash table.
size: Returns the number of words in the dictionary.
check: Checks if a given word is in the dictionary hash table.
unload: Frees the dictionary hash table from memory.

Functions are used in speller.c

by Matt Fergoda
*/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

/*
 Represents number of bins in the hash table for hash function
 below, based on a polynomial rolling hash as described here:
 https://cp-algorithms.com/string/string-hashing.html (last retrieved 2/12/20)
 Source suggests a large prime number for number of bins.
 This one is from Pietro Cataldi (https://en.wikipedia.org/wiki/Largest_known_prime_number)
 This number of bins gives a load factor (# entries / # of bins) that is
 less than 1 (143091 / 524287 = 0.27), which helps ensure the
 constant time property of the hash table. Though the load factor
 here is a bit low, indicating wasted memory,the next smallest prime number (131,071)
 would yeild a load factor greater than 1.
 In the interest of prioritizing speed over memory usage, I chose 524,287.
*/
const int HBINS = 524287;

bool loaded = false; // Will keep track of whether a dictionary has been loaded.

unsigned int words = 0; // Will count the number of words in the dictionary, used in size() function.

/*
A node in a linked list, containing a string representing a word in the dictionary
and a pointer to the next word in the dictionary. Will use this to form linked lists 
in the hash table.
*/
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

node *hashtable[HBINS] = { NULL }; // Will store the dictionary

/* 
Function: hash
--------------
A polynomial rolling hash function, as described:
https://cp-algorithms.com/string/string-hashing.html (last retrieved 2/12/20)

word: The word we want to hash.

Returns: An integer hash code. The same word will always return the same hash code.
*/
unsigned int hash(const char *word)
{
    unsigned int hash = 0;
    unsigned int p = 31; // Prime number generally ideal for English alphabet in consistent case, according to above source.
    unsigned int p_power = 1; // Powers are hard in C. This will simplify it.
    for (int i = 0; i < strlen(word) - 1; i++)
    {
        hash += tolower(word[i]) * p_power;
        p_power = p_power * p;
    }
    hash = hash % HBINS;
    return hash;
}

/* 
Function: load
--------------
Loads dictionary (list of correctly spelled words in .txt format) into memory.

dictionary: String containing the dictionary .txt file name.

Returns: Bool indicating whether or not the dictionary was loaded successfully.
*/
bool load(const char *dictionary)
{
    // Open dictionary, check that it opens correctly otherwise return false.
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }
 
    char word[LENGTH + 1];  // Buffer for a word.

    // Insert words into hash table.
    while (fscanf(file, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));

        // Check that new_node was allocated successfully.
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        else
        {
            memset(new_node, 0, sizeof(node)); // Initialize the temporary node.
            strcpy(new_node -> word, word); // Copy the word into the word attribute of the new node.
            unsigned int hashcode = hash(new_node -> word); // Hash the new word.

            // Insert the new word into the hash table.
            new_node -> next = hashtable[hashcode];
            hashtable[hashcode] = new_node;
         
            words++; // Increment words for size()
        }
    }

    fclose(file);

    loaded = true; // Load successful. Change loaded to true.
    return true;
}

/* 
Function: size
--------------
Returns: Number of words in dictionary if loaded, otherwise 0 if not yet loaded.
*/
unsigned int size(void)
{
    if (loaded)
    {
        return words;
    }
    else
    {
        return 0;
    }
}

/* 
Function: check
--------------
Checks whether a word is in the dictionary hash table.

word: The word we want to find in the dictionary.

Returns: True if word is in dictionary, false otherwise.
*/
bool check(const char *word)
{
    unsigned int hashword = hash(word); // Hash the word we're checking for.
    node *cursor = hashtable[hashword]; // Start the cursor at the corresponding place in the hash table.
    while (cursor != NULL)
    {
        if (strcasecmp(cursor -> word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor -> next;
        }
    }
    return false;
}

/* 
Function: unload
--------------
Unloads dictionary hash table from memory.

Returns: True if memory is successfully freed, false otherwise.
*/
bool unload(void)
{
    for (int i = 0; i < HBINS; i++)
    {
        node *cursor = hashtable[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor -> next;
            free(temp);
        }
    }
    return true;
}
