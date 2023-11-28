# Project description

This is a C program that aims to resolve the task explained below using efficient algorithms and data structures

# Task

The WordChecker project aims to create a system that, at its core, checks the correspondence between the letters of two words of equal length. Words are considered as sequences of symbols, including lowercase and uppercase alphabetical characters (a-z, A-Z), numeric digits (0-9), and the symbols '-' (hyphen) and '_' (underscore). The system reads input from standard input, including the length of the words (k), a sequence of permissible words, and a series of game rounds marked by the command "+nuova_partita."

During each game round, the system processes a sequence of strings, including a reference word, the maximum number of words to compare with the reference word, and a set of words for comparison. Additionally, the input may contain the command "+stampa_filtrate," which influences the output, and commands "+inserisci_inizio" and "+inserisci_fine" to add new words to the permissible set.

For each word (p) to be compared with the reference word (r), the program outputs a sequence of characters based on the following rules:

If the i-th character of p is equal to the i-th character of r, the corresponding character in the output sequence (res) is '+', indicating the correct position.
If p[i] does not appear in r, res[i] is '/'.
If p[i] appears in r but not in the i-th position, and certain conditions regarding the number and position of occurrences are met, res[i] is '/', otherwise, it is '|'.
The output is written to stdout for each word comparison in the specified format. The input may also include commands to insert new words at the beginning or end of the permissible word set.

When the program reads the "+stampa_filtrate" command during a game, it should generate output that, in lexicographical order, represents the set of permissible words compatible with the learned constraints up to that point in the game, with each word on a new line.

# My implementation

My implemantation uses a red black tree to keep track of the persmissibles worlds and for each game an ordered dynamic array that contains all the words that are compatible with the current constraints. Each time a new word is inserted, a struct that contains all the information given by the costrains is updated. Whenever the player wants to print all the words compatible with the constrains, a cycle updates the dynamic array and prints the correct words at the same time, using the information contained in the constrains struct.

# How to run

Compile the program:
```
gcc script.c
```

run the program with a testcase:
```
a.out < open_testcases/test1.txt > out.txt
```

check if the output is correct:
```
diff out.txt open_testcases/test1.output.txt
```