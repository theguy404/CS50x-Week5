// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Number of words currently stored in memory
int wordCounter = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int hashNum = hash(word);
    
    node *tempNode = table[hashNum];
    
    while (tempNode)
    {
        int test = strcasecmp(tempNode->word, word);
        
        if (test == 0)
        {
            return true;
        }
        tempNode = tempNode->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hashNum = 0;
    
    for (int i = 0; word[i] != '\0'; i++)
    {
        hashNum = hashNum + toupper(word[i]);
    }
    
    return hashNum % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // make sure dictionary file exists and load it
    FILE *fileToRead = fopen(dictionary, "r");
    if (!fileToRead) 
    {
        return false;
    }
    
    // load first word into memory
    char thisWord[LENGTH + 1];
    int readComplete = fscanf(fileToRead, "%s\n", thisWord);
    
    // hash the word and place it in its list location, then load the next word
    while (readComplete != EOF)
    {
        // build the node
        node *thisNode = malloc(sizeof(node));
        if (thisNode == NULL)
        {
            return false;
        }
        
        // fill the node with its word
        strcpy(thisNode->word, thisWord);
        thisNode->next = NULL;
        
        // get hash for this word, then set the current hash bucket
        int hashKey = hash(thisWord);
        thisNode->next = table[hashKey];
        table[hashKey] = thisNode;
        
        wordCounter++;
        readComplete = fscanf(fileToRead, "%s\n", thisWord);
    }
    
    fclose(fileToRead);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return wordCounter;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *tempNode = table[i];
        
        while (tempNode)
        {
            node *toDelete = tempNode;
            tempNode = tempNode -> next;
            free(toDelete);
        }
        table[i] = NULL;
    }
    
    return true;
}
