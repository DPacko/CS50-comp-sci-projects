// Implements a dictionary's functionality.
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dictionary.h"

#define HASHSIZE 6000

// create hash function
// source: https://cs50.stackexchange.com/questions/18879/problems-with-hash-function-in-pset5
int hash(const char *word)
{
    unsigned int index = 0;
    for (int i=0; i < strlen(word); i++)
    {
        index = (index << 2) ^ word[i];
    }
    return index % HASHSIZE;
}

// defining struct node for linked list
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// // keep track of dictionary size
int counter = 0;

// keep track of loaded dictionary
bool loaded = false;

// size of hashtable
node *hashtable[HASHSIZE];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // declare new word string and copy text word into string
    char new_word[strlen(word) + 1];
    strcpy(new_word, word);

    // put the words in lowercase form
    for (int i = 0; new_word[i] != '\0'; i++)
    {
        new_word[i] = tolower(new_word[i]);

        // check if the characters are non-alphabetical
        if (!isalpha(word[i]) && word[i] != '\'')
        {
            return false;
        }
    }

    // create node cursor that points to the beginning of a hashtable index
    int head = hash(new_word);
    node *cursor = hashtable[head];

    // go through linked list until NULL and compare words
    while (cursor != NULL)
    {
        if (strcmp(cursor->word, new_word) == 0)
        {
            return true;
        }
            // check next node
            cursor = cursor->next;
    }
    return false;
}


// Loads dictionary into memory. Returns true if successful else false.
bool load(const char *dictionary)
{

    // initialize all arrays in dictionary to NULL
    for (int i = 0; i < HASHSIZE; i++)
    {
        hashtable[i] = NULL;
    }

    // open/read dictionary file
    FILE *file = fopen(dictionary, "r");
    char word[LENGTH + 1];


    while(fscanf(file, "%s", word) != EOF)
    {

        // malloc for each new node/word
        node *new_node = malloc(sizeof(node));

        // check to make sure node isn't NULL
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        // set value to node
        strcpy(new_node->word, word);

        // set head to hash value
        int head = hash(new_node->word);

        // set new node pointing to head
        new_node->next = hashtable[head];

        // set head pointing to new node
        hashtable[head] = new_node;

        if (feof(file))
        {
            break;
        }

        // count up the words
        counter++;

    }

    // close dictionary
    fclose(file);

    // dictionary is now loaded
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded.
unsigned int size(void)
{
    if(loaded)
    {
        return counter;
    }
    else
    {
    return 0;
    }
}

// Unloads dictionary from memory. Returns true if successful else false.
bool unload(void)
{
    // go through arrays and free memory
    for (int i = 0; i < HASHSIZE; i++)
    {
        // create node cursor that points to beginning of array
        node *cursor = hashtable[i];

        // free all nodes in linked lists
        while(cursor != NULL)
        {
            // create a temporary node that points to where cursor is
            node *temp = cursor;

            // move cursor to next node in linked list
            cursor = cursor -> next;

            // free the memory from where temp is
            free(temp);
        }
    }
    return true;
}