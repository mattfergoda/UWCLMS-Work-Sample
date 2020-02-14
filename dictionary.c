// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

/*
 Represents number of bins in the hash table for hash function
 below, a polynomial rolling hash as described here:
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


// Initialize a bool to store whether a dictionary has been loaded
bool loaded = false;

// Initialize an int to to count the number of words in the dictionary,
// used in size() function.
unsigned int words = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Instantiate the hash table as an array of nodes
node *hashtable[HBINS] = { NULL };


// A polynomial rolling hash function, as described:
// https://cp-algorithms.com/string/string-hashing.html
//
// I found this generally performs check() in about 0.75s on holmes.txt
// whereas a simple hash summing the ascii values of a word
// performs check() in about 3s on holmes.txt.

unsigned int hash(const char *word)
{
    unsigned int hash = 0;
    unsigned int p = 31; // Prime number generally ideal for English alphabet in one case (upper or lower), according to above source.
    unsigned int p_power = 1; // Powers are hard in C. This will simplify it.
    for (int i = 0; i < strlen(word) - 1; i++)
    {
        hash += tolower(word[i]) * p_power;
        p_power = p_power * p;
    }
    hash = hash % HBINS;
    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary, check that it opens correctly otherwise return false
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
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
            // initialize the temporary node
            memset(new_node, 0, sizeof(node));

            // Copy the word into the word attribute of the new node.
            strcpy(new_node -> word, word);

            // Hash the new word.
            unsigned int hashcode = hash(new_node -> word);

            // Insert the new word into the hash table
            new_node -> next = hashtable[hashcode];
            hashtable[hashcode] = new_node;

            // Increment words so we know the number of words
            // in the dictionary for the size() function
            words++;
        }
    }

    // Close dictionary
    fclose(file);

    // Load successful. Change loaded to true and
    // return true
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
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

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int hashword = hash(word); // Hash the word we're checking for
    node *cursor = hashtable[hashword]; // Start the cursor at the corresponding place in the hash table
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

// Unloads dictionary from memory, returning true if successful else false
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
