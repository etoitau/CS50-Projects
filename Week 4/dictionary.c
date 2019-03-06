// CS50 Problem Set 4
// dictionary
// Functions used by a spellchecker.
//     works together with speller and dictionary.h which were set by problem not to be changed
//
// by: Kyle Chatman
// on: 190227


#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"


// Represents a node in a hash table with a word stored at the node and link to next in list
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// default number of bins
// defaut dictionary is 143091 words, so targeting <10 elements per bin
const unsigned int N = 16384;

// Represents a hash table
node *hashtable[N] = {NULL};

// hash based on djb2 by Dan Bernstein
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    unsigned int c, hsh;

    while ((c = *word++))
    {
        // hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }
    hsh = hash % N;

    return hsh;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table one at a time
    while (fscanf(file, "%s", word) != EOF)
    {
        // get space for new node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        // put current word into new node
        strcpy(new_node->word, word);

        // set next to NULL by default
        new_node->next = NULL;

        // hash word
        unsigned int h = hash(word);

        // if not first in bin, have new node get pointer of current head
        if (hashtable[h] != NULL)
        {
            new_node->next = hashtable[h];
        }

        // set current head to new node
        hashtable[h] = new_node;

    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    unsigned long int wordcount = 0;

    for (unsigned int i = 0; i < N; i++)
    {
        bool end = false;

        // don't try to search in empty bin
        if (hashtable[i] != NULL)
        {
            // set tracking pointer to start of this bin
            node *track = hashtable[i];

            // follow linked list to end counting elements
            do
            {
                if (track == NULL)
                {
                    end = true;
                }
                else
                {
                    wordcount++;
                    track = track->next;
                }
            }
            while (!end);
        }

    }
    return wordcount;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char lowword[LENGTH + 1];

    strcpy(lowword, word);

    // reformat word to all lowercase
    for (int i = 0; lowword[i]; i++)
    {
        lowword[i] = tolower(lowword[i]);
    }

    // get hash for lowercase version of word
    unsigned int h = hash(lowword);

    bool end = false;
    bool found = false;

    // don't try to search in empty bin
    if (hashtable[h] == NULL)
    {
        return false;
    }

    // get tracking pointer and set to start of this bin
    node *track = hashtable[h];

    // check bin has something in it
    if (track == NULL)
    {
        return false;
    }

    // follow linked list to end looking for word
    do
    {
        // if at end of list, didn't find it
        if (track == NULL)
        {
            end = true;
        }
        else
        {
            // compare word in this bin
            if (strcmp(track->word, lowword) == 0)
            {
                found = true;
                end = true;
            }
            // if not found, try next
            else
            {
                track = track->next;
            }

        }

    }
    while (!end);

    return found;
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{

    // free nodes of linked list in each bin
    for (unsigned int i = 0; i < N; i++)
    {
        // don't try to demo at empty bin
        if (hashtable[i] != NULL)
        {
            // free down to end of list
            node *cursor = hashtable[i];
            while (cursor != NULL)
            {
                node *temp = cursor;
                cursor = cursor->next;
                free(temp);
            }
        }

    }
    return true;
}
