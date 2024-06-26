// Lab10.c
/*
Norman Fernandez
COP3502C-24Spring 0039
Computer Science 1 Lab Assignment #10
This program will implement a trie data structure to efficiently store the words in a dictionary text file,
and furthermore record the number of times the same word recurred in the dictionary.
Functions to create the trie, read words from the dictionary, insert those words into the trie, deallocate the trie,
and return the number of times, if any, a given word was inserted into the trie.
*/

// Include called functions from header files.
#include <stdio.h> // Include standard I/O to read from file, fopen(), fscanf(), fclose().
#include <stdlib.h> // Include standard memory allocation and deallocation, malloc() and free().
#include <string.h> // Include string functions.
#include <stdbool.h> // Include bool type, true and false definitions.

// Define ALPHABET to be replaced with the integer 26 at compile time, for the 26 letters of the alphabet.
#define ALPHABET 26

// Trie structure
struct Trie
{
    struct Trie* nextLetter[ALPHABET]; // An array of pointers to trie nodes, one pointer for each letter of the alphabet.
    bool wordEnd; // When True, the trie node marks the end of a word. When false, the node does not mark the end of a word.
    int wordFrequency; // The number of times the word this node marks the end of, if it exists, occurs in the dictionary.
};

// Function Declarations
void insert(struct Trie *pTrie, char *word);
int numberOfOccurances(struct Trie *pTrie, char *word);
struct Trie *deallocateTrie(struct Trie *pTrie);
struct Trie *createTrie();

// Function Definitions
// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie* tmp = pTrie;
    // For every node except that which represents the null pointer...
    for (int i = 0; word[i] != '\0'; i++) {
        // Compute the index for the next letter's node...
        int letterIndex = word[i] - 'a';
        // Check if that node exists...
        if (tmp->nextLetter[letterIndex] == NULL) {
            // If it does not exist, create that node and link it appropriately.
            tmp->nextLetter[letterIndex] = createTrie();
        }
        // Else the node does exist, so traverse to the next node.
        tmp = tmp->nextLetter[letterIndex];
    }

    // If the word was all ready entered into the trie, increment the word frequency counter.
    if (tmp->wordEnd == true) {
        tmp->wordFrequency++;
    }

    // Else, mark this node as the end of a word, and increment the word frequency counter.
    else {
        tmp->wordEnd = true;
        tmp->wordFrequency++;
    }
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    // Search the trie for the given word using a traversal pointer.
    struct Trie *searcher = pTrie;

    // Navigate to the node associated with the last letter of the word.
    for (int i = 0; word[i] != '\0'; i++) {
        // Compute the index for the node address of the next letter in the word.
        int tIndex = word[i] - 'a';

        // If the node does not exist, the word cannot be stored in the trie at all, so return 0 occurences found.
        if (searcher->nextLetter[tIndex] == NULL) {
            return 0;
        }

        // Else, the next letter in the word was found, so keep traversing.
        searcher = searcher->nextLetter[tIndex];
    }

    // If the word is found in the trie, return that node's word frequency component.
    if (searcher->wordEnd = true) {
        return searcher->wordFrequency;
    }

    // Else, return 0 occurences found.
    return 0;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    // For all 26 letters in the alphabet...
    for (int i = 0; i < ALPHABET; i++) {
        // If one of the letters' associated node exists...
        if (pTrie->nextLetter[i] != NULL) {
            // Recursively deallocate that associated node.
            deallocateTrie(pTrie->nextLetter[i]);
        }

        // Else, there are no child nodes to deallocate, so continue the loop.
        else {
            continue;
        }
    }

    // Free the current node.
    free(pTrie);

    // Return NULL to indicate that the trie deallocation operation was a success.
    return NULL;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    // Declare a pointer to a trie node...
    struct Trie* newTrie;
    // Dynamically allocate memory for that new node...
    newTrie = malloc(sizeof(struct Trie));

    // Initialize the members of the trie node.
    // Mark every pointer-to-trie-node in the trie node's array of pointers to be NULL pointers.
    for (int i = 0; i < ALPHABET; i++) {
        newTrie->nextLetter[i] = NULL;
    }
    // Flag the node as not being the end of a word.
    newTrie->wordEnd = false;
    // Set the new node's word frequency component to 0.
    newTrie->wordFrequency = 0;

    // Return the newly created node.
    return newTrie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    // Open the dictionary file, access the contents with a file pointer, fileCursor.
    FILE *fileCursor;
    fileCursor = fopen(filename, "r");
    // First line of input gives the number of words
    int lineNum; // The number of lines in the dictionary.

    if (fileCursor != NULL) { // If the file pointer is not NULL...
        fscanf(fileCursor, "%d", &lineNum); // Read the first line as the number of lines in the dictionary.
        if (lineNum != 0) { // If that number of lines is non-zero...
            for (int i = 0; i < lineNum; i++) { // For every line...
                pInWords[i] = malloc(100*sizeof(char)); // Allocate memory for a string in the array...
                fscanf(fileCursor, "%s", pInWords[i]); // Store the string in the array.
            }
        }
    }

    // Close the dictionary file.
    fclose(fileCursor);

    // Return the number of unique words found in the array, and thus the dictionary file.
    return lineNum;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}