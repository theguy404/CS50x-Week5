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
    
    node* tempNode = table[hashNum];
    
    while(!tempNode)
    {
        if(strcasecmp(tempNode -> word, word) == 0)
        {
            return true;
        }
        tempNode = tempNode -> next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hashNum = 0;
    
    for(int i = 0; word[i] != '\0'; i++)
    {
        hashNum += tolower(word[i]);
    }
    
    return hashNum % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE* fileToRead = fopen(dictionary, "r");
    if(!fileToRead) 
    {
        return false;
    }
    
    for(int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    
    char thisWord[LENGTH + 1];
    
    while(fscanf(fileToRead, "%s\n", thisWord) != EOF)
    {
        node* thisNode = malloc(sizeof(node));
        
        strcpy(thisNode -> word, thisWord);
        
        int hashKey = hash(thisWord);
        
        if(!table[hashKey])
        {
            thisNode -> next = NULL;
            table[hashKey] = thisNode;
        }
        else
        {
            thisNode -> next = table[hashKey];
            table[hashKey] = thisNode;
        }
        
        wordCounter++;
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
    for(int i = 0; i < N; i++)
    {
        node* tempNode = table[i];
        
        while(tempNode)
        {
            node* toDelete = tempNode;
            tempNode = tempNode -> next;
            free(toDelete);
        }
        table[i] = NULL;
    }
    
    return true;
}
