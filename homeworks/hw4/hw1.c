#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


// Taken from https://stackoverflow.com/questions/22705751/cannot-open-include-file-unistd-h-no-such-file-or-directory
// Replaces the line #include <unistd.h>
#ifdef _WIN32
#include <io.h>
#define access _access

#else
#include <unistd.h>
#include <fcntl.h>
#endif

const char OPEN_BRACKET = 91;
const char CLOSED_BRACKET = 93;

typedef char** TokenHolder;

typedef enum {
	CALLOC,
	REALLOC,
	DUPLICATE
} AllocationType;

int largestTokenLengthInFile(const char* fileName) {
	int fileHandle = open(fileName, 0);
	if (fileHandle < 0) {
		return -1;
	}
	
	int current = 0;
	int longest = 0;
	
	char currentChar = 0;
	int charactersRead = 100;
	while (charactersRead) {
		charactersRead = read(fileHandle, &currentChar, 1);
		if (isspace(currentChar)) {
			if (current > longest) {
				longest = current;
			}
			current = 0;
		}
		else {
			++current;
		}
	}
	
	return longest;
	
	
	
}

int logBase10RoundedUp(const int n) {
	int toReturn = 0;
	int total = 1;
	while (total < n) {
		toReturn += 1;
		total *= 10;
	}
	return toReturn;
}

char* callocIToA(int i) {
	char* toReturn = calloc(logBase10RoundedUp(i) + 1, sizeof(char));
	sprintf(toReturn, "%d", i);
	return toReturn;
}

char* callocAndStringifiedLongestTokenInFile(const char* fileName) {
	int longest = largestTokenLengthInFile(fileName);
	if (longest == -1) {
		return NULL;
	}
	return callocIToA(longest);
}


char* AllocationType_toString(const AllocationType type) {
	switch (type) {
		case CALLOC:
			return "calloc";
		case REALLOC:
			return "realloc";
		case DUPLICATE:
			return "duplicate";
	}
	return "ERROR";
}

char* appendString(char* originalString, const char* toAppend) {
	char* toReturn = realloc(
		originalString,
		strlen(originalString) + strlen(toAppend) + 1
	);
	
	strcat(toReturn, toAppend);
	
	return toReturn;
}




void addToken(TokenHolder tokenHolder, const char* token) {
	int tokenLength = strlen(token);
	
	if (!tokenLength) return;
	
	int holderAlreadyHasWord = 0;
	
	char** pointerToTokenStringToAppendTo = tokenHolder + tokenLength - 1;
	AllocationType howMemoryWasAllocatedHere;
	
	if (*pointerToTokenStringToAppendTo == NULL) {
		*pointerToTokenStringToAppendTo = calloc(tokenLength + 2, sizeof(char));
		howMemoryWasAllocatedHere = CALLOC;
	}
	else {
		holderAlreadyHasWord = strstr(*pointerToTokenStringToAppendTo, token) != NULL;
		
		
		*pointerToTokenStringToAppendTo = realloc(
			*pointerToTokenStringToAppendTo,
			strlen(*pointerToTokenStringToAppendTo) + tokenLength + 2
		);
		
		if (holderAlreadyHasWord) {
			howMemoryWasAllocatedHere = DUPLICATE;
		}
		else {
			howMemoryWasAllocatedHere = REALLOC;
		}
	}
	
	printf("%s (length %d) (%s)\n", token, tokenLength, AllocationType_toString(howMemoryWasAllocatedHere));
	
	if (holderAlreadyHasWord) {
		return;
	}
	
	strcat(*pointerToTokenStringToAppendTo, token);
	strcat(*pointerToTokenStringToAppendTo, "\n");
	
	
}


TokenHolder callocAndMakeTokenHolder(
	const int fileHandle,
	const int maxTokenLength
) {
	TokenHolder toReturn = calloc(
		maxTokenLength,
		sizeof(char*)
	);
	
	char* buffer = calloc(maxTokenLength + 1, sizeof(char));
	
	int currentTokenLength = 0;
	
	while (1) {
		
		if (currentTokenLength > maxTokenLength) {
			while (1) {
				int bytesRead = read(fileHandle, buffer, 1);
				if (!bytesRead || isspace(*buffer)) {
					currentTokenLength = 0;
					break;
				}
			}
		}
		
		int bytesRead = read(fileHandle, buffer + currentTokenLength, 1);
		
		
		if (isspace(*(buffer + currentTokenLength)) || !bytesRead) {
			*(buffer + currentTokenLength) = '\0';
			addToken(toReturn, buffer);
			currentTokenLength = -1;
		}
		
		
		if (!bytesRead) break;
		
		
		++currentTokenLength;
		
		
	}
	
	free(buffer);	
	return toReturn;
}

void printPhraseTokensAtIndex(const int index) {
	printf("Tokens at index %c%d%c:\n", OPEN_BRACKET, index, CLOSED_BRACKET);
}

void printFileDoesntExistError(const char* fileName) {
	fprintf(stderr, "ERROR: Invalid file %s\n", fileName);
}


int main(int argc, char const** argv)
{
	if (argc == 1) {
		printf("ERROR: Usage hw1 tokenfile [maxTokenLength]\n");
		return 1;
	}
	if (argc == 2) {
		char const** nextArgv = calloc(3, sizeof(char*));
		*(nextArgv) = *argv;
		*(nextArgv + 1) = *(argv + 1);
		
		char* longestTokenArg = callocAndStringifiedLongestTokenInFile(*(argv + 1));
		
		if (longestTokenArg == NULL) {
			printFileDoesntExistError(*(argv + 1));
			return 1;
		}
		
		*(nextArgv + 2) = longestTokenArg;
		
		// int largestTokenLength = largestTokenLengthInFile(*(argv + 1));
		
		// sprintf(*(nextArgv + 2), "%d", )
		
		int toReturn = main(3, nextArgv);
		
		free(longestTokenArg);
		free(nextArgv);
		return toReturn;
	}
	
	const char* fileName = *(argv + 1);
	const int maxTokenLength = atoi(*(argv + 2));
		
	if (maxTokenLength <= 0) {
		fprintf(stderr, "ERROR: entered maximum length was 0 or negative.\n");
		return 1;
	}
	
	int fileHandle = open(fileName, 0);
	
	if (fileHandle < 0) {
		printFileDoesntExistError(fileName);
		return 1;
	}
	
	TokenHolder tokenHolder = callocAndMakeTokenHolder(
		fileHandle,
		maxTokenLength
	);
	
	for (int i = 0; i < maxTokenLength; ++i) {
		if (*(tokenHolder + i)) {
			printf("\n");
			printPhraseTokensAtIndex(i + 1);
			printf("%s", *(tokenHolder + i));
		}
	}
		
		
	for (int i = 0; i < maxTokenLength; ++i) {
		if (*(tokenHolder + i)) {
			free(*(tokenHolder + i));
		}
	}
	
	close(fileHandle);
	
	free(tokenHolder);
		
	
	
	
	
	
	
	
	return 0;
}


