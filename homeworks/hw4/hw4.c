#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <ctype.h>


#include "mystring.h"
#include "linkedlist.h"
#include "linkedmap.h"
DECLARE_LINKEDLIST_TYPE(char);
// DECLARE_LINKEDLIST_TYPE(str);

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
	read(fd, &toReturn, 1);
	return toReturn;
}

char* linkedList_char__toString(LinkedList_char list) {
	str toReturn = calloc(list.length + 1, sizeof(char));
	
	int writingIndex = 0;
	
	LinkedList__foreach(char, currentChar, list) {
		at(toReturn, writingIndex) = currentChar;
		++writingIndex;
	}
	
	LinkedList__append(str, allStringsMarkedForFreeing, toReturn);
	
	return toReturn;
}


char* readWord(int fd) {
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
		}
		else {
			break;
		}
		
	}
	
	str toReturn = linkedList_char__toString(wordSoFar);
	
	LinkedList__free(char, wordSoFar);
	
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
	
	str word = readWord(inputFile);
	printf("%s\n", word);
	
	
	
	
	freeAllStrings();
	return 0;
}
