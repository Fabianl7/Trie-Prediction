# Trie Prediction System

This C program implements a Trie-based text prediction system, providing functionality for building a Trie from a corpus text, inserting strings, searching for words, and offering predictive suggestions based on user input. The program reads commands from an input file and executes corresponding operations on the Trie.

## How it Works

### Trie Data Structure

The core of the Trie Prediction System is the Trie data structure, a tree-like structure that stores a dynamic set of strings. Each node in the Trie represents a single character, and the edges between nodes represent the characters in the strings. The Trie allows for efficient string insertion, retrieval, and predictive suggestions.

### Build Trie from Corpus

The program begins by building a Trie from a given corpus text file. It reads the words from the file, strips punctuation, and inserts each word into the Trie. The Trie organizes the words in a way that facilitates quick retrieval and prediction.

### Command Processing

The system processes commands from an input file, which can include requests to print the main Trie, obtain predictive suggestions, or print subtries. Commands starting with '!' print the main Trie, '@' requests predictive suggestions for a given word, and other strings print subtries.

### Predictive Suggestions

Predictive suggestions are obtained by traversing the Trie based on the characters of the user input. The system finds the node corresponding to the input and explores its subtrie to suggest the most likely next words.

### Additional Functionality

The program offers several additional functionalities, such as checking if a word is present in the Trie, determining the count of words with a common prefix, calculating the number of new nodes required for a string insertion, and more.

## Usage

1. **Compile the Program:**
   - Open a terminal and navigate to the directory containing the program files.
   - Compile the program using the following command:
     ```bash
     gcc TriePrediction.c -o TriePrediction
     ```

2. **Run the Program:**
   - After compilation, run the program with the following command:
     ```bash
     ./TriePrediction corpus.txt commands.txt
     ```
   - Replace `corpus.txt` with the path to the corpus text file and `commands.txt` with the path to the commands file.

3. **View Output:**
   - The program will process commands from the input file and display the corresponding output.

## Example

```bash
gcc TriePrediction.c -o TriePrediction
./TriePrediction corpus.txt commands.txt
