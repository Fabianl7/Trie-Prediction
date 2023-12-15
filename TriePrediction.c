// Fabian Londono
// NID: Fa661057
// Cop 3502, Spring 2022

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TriePrediction.h"

// Modified version of the createNode function in Dr.Szumlanski's notes
TrieNode *createTrieNode(void)
{
	int i;
	TrieNode *root = malloc(sizeof(TrieNode));

	root->count = 0;
	root->subtrie = NULL;
	for (i = 0; i < 26; i++)
		root->children[i] = NULL;

	return root;
}

// This function comes from the Dr. Szumlanski's Tries notes
TrieNode *insertString(TrieNode *root, char *str)
{
	int i, index, length = strlen(str); 
	TrieNode *wizard;
	
	if (root == NULL)
		root = createTrieNode();

	wizard = root;
	
	for (i = 0; i < length; i++)
	{
		if (!isalpha(str[i]))
		{
			fprintf(stderr, "Error: \"%s\" contains non-alpha characters and will not be inserted.\n", str);
			return NULL;
		}
		index = tolower(str[i]) - 'a';

		if (wizard->children[index] == NULL)
			wizard->children[index] = createTrieNode();

		// Now the wizard is able to jump forward.
		wizard = wizard->children[index];
	}
	
	wizard->count++;
	return wizard;
}

// Takes out all the punctuation in the passed string
void stripPunctuators(char *str)
{
	int i, j, len;
	char temp[MAX_CHARACTERS_PER_WORD];

	if(str == NULL)
		return;

	len = strlen(str);

	// Make the temp string a copy of str but without punctuation
	for (i = 0, j = 0; i < len; i++)
	{
		if (isalpha(str[i]))
		{
			temp[j] = (tolower(str[i]));
			j++;
		}
	}

	temp[j] = '\0';
	strcpy(str, temp);
}

// Helper function called by printTrie(). (Credit: Dr. S.)
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0. (Credit: Dr. S.)
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1024];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

// Returns the terminal node in the trie corresponding to the specified string
TrieNode *getNode(TrieNode *root, char *str)
{
	int i, j;
	int len;
	TrieNode *root2 = NULL;
	char str2[MAX_CHARACTERS_PER_WORD];

	if (root == NULL || str == NULL)
		return NULL;

	// Check if the string is an empty string
	if (str[0] == '\0' && root->count > 0)
		return root;
	else if (str[0] == '\0' && root->count == 0)
		return NULL;
	
	strcpy(str2, str);
	stripPunctuators(str2);
	
	len = strlen(str2);
	root2 = root;

	// Loop through the trie finding the characters in the string
	for (i = 0; i < len; i++)
	{
		j = str2[i] - 'a';

		if (root2->children[j] != NULL)
			root2 = root2->children[j];
		else
			return NULL;
	}

	if (root2->count > 0)
		return root2;

	return NULL;
}

// Create a trie with the corpus text in the file passed to the function
TrieNode *buildTrie(char *filename)
{
	char str[MAX_CHARACTERS_PER_WORD + 1], str2[MAX_CHARACTERS_PER_WORD + 1];
	int len;
	TrieNode *temp = NULL, *root = NULL;
	FILE *ifp;
	
	if ((ifp = fopen(filename, "r")) == NULL)
	{
		fprintf(stderr, "Failed to open \"%s\" in buildTrie().\n", filename);
		return NULL;
	}

	root = createTrieNode();
	
	// Loop through the courpus text allocating space for the trie
	while (fscanf(ifp, "%s", str) != EOF)
	{
		strcpy(str2, str);
		stripPunctuators(str);

		len = strlen(str) - 1;

		// Allocate space if the string contains any punctuation
		if (!isalpha(str2[len]))
		{
			stripPunctuators(str);
			if (temp->subtrie == NULL)
				temp->subtrie = createTrieNode();
			
			insertString(temp->subtrie, str);
			temp = insertString(root, str);

		} 

		// Create a subtrie and anode for the string
		else if (temp != NULL) 
		{
			if (temp->subtrie == NULL)
				temp->subtrie = createTrieNode();
			
			insertString(temp->subtrie, str);
			temp = insertString(root, str);

			if(!isalpha(str2[len]))
				temp = NULL;		
		} 

		// Create a node if the current root is null
		else if (temp == NULL) 
		{
			temp = insertString(root, str);
			
			if (!isalpha(str2[len]))
				temp = NULL;
		}
	}
	
	fclose(ifp);

	return root;
}

// Free all of the allocated memory in the trie
TrieNode *destroyTrie(TrieNode *root)
{
	int i;

	if (root == NULL)
		return NULL;

	// Loop through the trie freeing the memory
	for (i = 0; i < 26; i++)
		if(root->children[i] != NULL)
			destroyTrie(root->children[i]);

	if (root->subtrie != NULL)
		destroyTrie(root->subtrie);

	free(root);

	return NULL;
}

// Helper function for the getMostFrequentWord function
void mostFrequentHelper(TrieNode *root, char *recursiveStr, char *str, int index, int *count)
{   
	int i;  

	if (root == NULL || str == NULL)
		return;
	

	// Copies the current string if it has the current highest count
	if (root->count > *count)
	{
		*count = root->count;
		strcpy(str, recursiveStr);
	}

	// Search the subtries or the highest count
	for (i = 0; i < 26; i++)
	{
		recursiveStr[index] = i + 'a';
		mostFrequentHelper(root->children[i], recursiveStr, str, ++index, count);
		index--;
	}

	recursiveStr[index] = '\0';
}

// Use a helper function to fnid the string with the highest count in the trie
void getMostFrequentWord(TrieNode *root, char *str)
{
	int count = 0, index = 0;
	char recursiveStr[MAX_CHARACTERS_PER_WORD];

	if (root == NULL || str == NULL)
	{
		strcpy(str, "");
		return;
	}
	
	mostFrequentHelper(root, recursiveStr, str, index, &count);
}

// Calls another function to determine if a string is in the trie
int containsWord(TrieNode *root, char *str)
{
	if (root == NULL || str == NULL)
		return 0;

	if (getNode(root, str) != NULL)
		return 1;

	return 0;
}

// Helper function to find how many times a string is in a trie
int prefixCountHelper(TrieNode *root)
{
	int i, sum = 0;

	if (root == NULL)
		return 0;
	
	for (i = 0; i < 26; i++)
	{
		sum += prefixCountHelper(root->children[i]);
	}
	
	return sum + root->count;
	
}

// Find how many strings begin with the passed string
int prefixCount(TrieNode *root, char *str)
{
	int count, len, sum;
	int i;
	TrieNode *temp;

	if (root == NULL || str == NULL)
		return 0;
	
	len = strlen(str);
	temp = root;

	// Loop through the trie to find the passed string
	for (i = 0; i < len; i++)
	{
		temp = temp->children[str[i] - 'a'];

		if (temp == NULL)
			return 0;
	}

	return prefixCountHelper(temp);
}

// Find how many nodes would need to be created if the passed string is inserted into the trie
int newNodeCount(TrieNode *root, char *str)
{
	int len, index, newNodesCount = 0;
	int i;
	TrieNode *root2;
	
	if (str == NULL)
		return 0;
	
	len = strlen(str);
	
	if (root == NULL)
		return len + 1;
	
	root2 = root;
	
	// Count how many letters in the beginning of the string are already in the trie
	for (i = 0; i < len; i++)
	{
		index = tolower(str[i]) - 'a';
		if (root2->children[index] != NULL){
			newNodesCount++;
			root2 = root2->children[index];
		}
	}

	return len - newNodesCount;
}

// Process the corpus text of the file in a trie whith command inputs
int processInputFile(TrieNode *root, char *filename)
{
	char strInput[MAX_CHARACTERS_PER_WORD], command[MAX_CHARACTERS_PER_WORD];
	char temp[MAX_CHARACTERS_PER_WORD];
	int number, i;
	TrieNode *root2;
	FILE *ifp;
	
	ifp = fopen(filename, "r");
		
	// Verifies if the filename passed exists
	if(ifp == NULL || root == NULL)
		return 1;
		
	// Loop through the corpus text
	while(fscanf(ifp, "%s", command) != EOF)
	{   
		// The following section calls other functions to appropriately execute the functionality
		// of the commands
		if(command[0] == '!')
		{
			printTrie(root, 0);
		}
		else if(command[0] == '@')
		{
			fscanf(ifp, "%s", strInput);
			fscanf(ifp, "%d", &number);

			printf("%s", strInput);

			strcpy(temp, strInput);
			stripPunctuators(strInput);
			
			root2 = getNode(root, strInput);
			if(root2 == NULL)
			{
				printf("\n");
				break;       
			}

			for(i = 0; i < number; i++)
			{
				root2 = getNode(root, temp);

				if(root2 == NULL || root2->subtrie == NULL)
					break;
				
				getMostFrequentWord(root2->subtrie, temp);
				printf(" %s", temp);
			}

			printf("\n");

		} else {
			printf("%s\n", command);

			if(getNode(root, command) == NULL){
				printf("(INVALID STRING)\n");
				continue;
			}

			root2 = getNode(root, command);
			
			if(root2->subtrie == NULL)
				printf("(EMPTY)\n");
			else
				printTrie(root2->subtrie, 1);
		}
	}
	
	fclose(ifp);
	return 0;
}

double difficultyRating(void)
{
	return 3.5;
}

double hoursSpent(void)
{
	return 15.3;
}

int main(int argc, char **argv)
{
	TrieNode *root = buildTrie(argv[1]);
	
	processInputFile(root, argv[2]);

	root = destroyTrie(root);
	
	return 0;
}
