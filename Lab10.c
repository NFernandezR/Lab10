// Lab10.c
/*
Norman Fernandez
Lab Assignment 10
*/

// Include called functions from header files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Define ALPHABET to be replaced with the integer 26 at compile time, for the 26 letters of the alphabet.
#define ALPHABET 26

// Trie structure
struct Trie
{
    struct Trie* nextLetter[ALPHABET];
    bool wordEnd;
    int wordFrequency;
};

void insert(struct Trie *pTrie, char *word);
int numberOfOccurances(struct Trie *pTrie, char *word);
struct Trie *deallocateTrie(struct Trie *pTrie);
struct Trie *createTrie();

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie* tmp = pTrie;
    // For every node except that which represents the null pointer...
    for (int i = 0; word[i+1] != '\0'; i++) {
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
    FILE *txt;
    txt = fopen(filename, "r");
    // First line of input gives the number of words
    int lineNum; // The number of lines in the dictionary.
    fscanf(txt, "%d", &lineNum);
    int wordNum = 0; // The number of unique words in the dictionary.
    // Read the words in the dictionary to the word array.
    for (int i = 0; i < lineNum; i++) {
        // Initialize the char pointer, setting it to NULL...
        pInWords[i] = NULL;
        // Then, alloc mem for the string.
        pInWords[i] = malloc(100*sizeof(char));
        // Read a line from the dictionary file into the dynamically allocated string in the word array.
        fgets(pInWords[i], 90, txt);
        // Remove the trailing newline
        pInWords[i][strlen(pInWords[i] - 1)] = '\0';
    }
    // Close the dictionary file.
    fclose(txt);
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