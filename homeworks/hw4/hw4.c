#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>


#include "mystring.h"
#include "linkedlist.h"
#include "linkedmap.h"
#include "bigram.h"
DECLARE_LINKEDLIST_TYPE(char);




#define at(arr, i) *(arr + i)
#define APOSTROPHE '\''

LinkedList_str allStringsMarkedForFreeing = LinkedList__create;


void settings() {
    // struct rlimit rl;
    // getrlimit(RLIMIT_NPROC, &rl);
    // rl.rlim_cur = 32;
    // setrlimit(RLIMIT_NPROC, &rl);
    setvbuf(stdout, NULL, _IONBF, 0);
}


char readOneChar(int fd) {
	char toReturn;
	int charactersRead = read(fd, &toReturn, 1);
	if (charactersRead == 0) {
		return '\0';
	}
	toReturn = tolower(toReturn);
	return toReturn;
}

char* linkedList_char__toString(LinkedList_char list) {
	str toReturn = calloc(LinkedList__length(char, list) + 1, sizeof(char));
	int writingIndex = 0;
	
	LinkedList__foreach(char, currentChar, list) {
		at(toReturn, writingIndex) = currentChar;
		++writingIndex;
	}
	
	LinkedList__append(str, allStringsMarkedForFreeing, toReturn);
	
	return toReturn;
}


char* readWord(int fd, bool* endOfFileFlag) {
	LinkedList_char wordSoFar = LinkedList__create;
	
	int apostrophesRead = 0;
	while (1) {
		char currentCharBeingRead = readOneChar(fd);
		
		if (isalpha(currentCharBeingRead)) {
			LinkedList__append(char, wordSoFar, currentCharBeingRead);
		}
		else if (currentCharBeingRead == APOSTROPHE) {
			++apostrophesRead;
			if (apostrophesRead > 1) {
				break;
			}
			LinkedList__append(char, wordSoFar, currentCharBeingRead);
		}
		else {
			if (endOfFileFlag && !currentCharBeingRead) {
				*endOfFileFlag = true;
			}
			break;
		}
		
	}
	
	str toReturn = linkedList_char__toString(wordSoFar);
	
	LinkedList__free(char, wordSoFar);
	
	return toReturn;
	
}

LinkedList_str readAllWords(int fd) {
	LinkedList_str toReturn = LinkedList__create;
	
	bool reachedEndOfFile = false;
	while (!reachedEndOfFile) {
		str currentWord = readWord(fd, &reachedEndOfFile);
		if (strlen(currentWord)) {
			LinkedList__append(str, toReturn, currentWord);
		}
	}
	
	return toReturn;
}

void freeAllStrings() {
	LinkedList__foreach(str, toDelete, allStringsMarkedForFreeing) {
		free(toDelete);
	}
	LinkedList__free(str, allStringsMarkedForFreeing);
}




int main(int argc, char** argv)
{
	settings();
	str inputFileName = at(argv, 1);
	int inputFile = open(inputFileName, O_RDONLY);
	
	LinkedList_str words = readAllWords(inputFile);
	
	LinkedList__foreach(str, word, words) {
		printf("String: %s\n", word);
	}
	
	BiGramHolder bigrams = BiGramHolder__create(words);
	
	BiGramHolder__display(bigrams);
	
	printf("%d\n", BiGramHolder__getTimes(bigrams, "are", "they"));
	printf("%d\n", BiGramHolder__getTimes(bigrams, "are", "they'n"));
	printf("%d\n", LinkedMap__containsKey(str, LinkedMap_str_int, (*bigrams), "are", equals_str));
	
	
	LinkedList__free(str, words);
	BiGramHolder__free(bigrams);
	freeAllStrings();
	return 0;
}
