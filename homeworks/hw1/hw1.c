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

char* AllocationType_toString(AllocationType type) {
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

char* appendString(char* originalString, char* toAppend) {
	char* toReturn = realloc(
		originalString,
		strlen(originalString) + strlen(toAppend) + 1
	);
	
	strcat(toReturn, toAppend);
	
	return toReturn;
}




void addToken(TokenHolder tokenHolder, char* token) {
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
	int fileDescriptor,
	int maxTokenLength
) {
	TokenHolder toReturn = calloc(
		maxTokenLength,
		sizeof(char*)
	);
	
	char* buffer = calloc(maxTokenLength + 1, sizeof(char));
	
	int currentTokenLength = 0;
	
	while (1) {
		
		if (currentTokenLength > maxTokenLength) {
			// Go to end of token
		}
		
		int bytesRead = read(fileDescriptor, buffer + currentTokenLength, 1);
		
		
		if (isspace(*(buffer + currentTokenLength)) || !bytesRead) {
			*(buffer + currentTokenLength) = '\0';
			addToken(toReturn, buffer);
			currentTokenLength = -1;
		}
		
		
		if (!bytesRead) break;
		
		
		++currentTokenLength;
		
		
	}
	
	free(buffer);
	printf("\n");
	
	return toReturn;
}

void printPhraseTokensAtIndex(int index) {
	printf("Tokens at index %c%d%c:\n", OPEN_BRACKET, index, CLOSED_BRACKET);
}




int main(int argc, char const *argv[])
{
	if (argc == 1) {
		printf("Usage hw1 tokenfile [maxTokenLength]\n");
		return 1;
	}
	if (argc == 2) {
		char const** nextArgv = calloc(3, sizeof(char*));
		*(nextArgv) = *argv;
		*(nextArgv + 1) = *(argv + 1);
		*(nextArgv + 2) = "100";
		
		int toReturn = main(3, nextArgv);
		
		free(nextArgv);
		return toReturn;
	}
	
	const char* fileName = *(argv + 1);
	const int maxTokenLength = atoi(*(argv + 2));
	
	int fileDescriptor = open(fileName, 0, "r");
	
	TokenHolder tokenHolder = callocAndMakeTokenHolder(
		fileDescriptor,
		maxTokenLength
	);
	
	for (int i = 0; i < maxTokenLength; ++i) {
		if (*(tokenHolder + i)) {
			printPhraseTokensAtIndex(i + 1);
			printf("%s\n", *(tokenHolder + i));
		}
	}
	
	
	// printf("%s\n", fileContents);
	
	for (int i = 0; i < maxTokenLength; ++i) {
		if (*(tokenHolder + i)) {
			free(*(tokenHolder + i));
		}
	}
	
	close(fileDescriptor);
	
	free(tokenHolder);
		
	
	
	
	
	
	
	
	return 0;
}


