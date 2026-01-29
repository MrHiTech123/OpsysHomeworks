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


typedef char** TokenHolder;

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
	
	char** pointerToTokenStringToAppendTo = tokenHolder + tokenLength - 1;
	
	
	if (*pointerToTokenStringToAppendTo == NULL) {
		*pointerToTokenStringToAppendTo = calloc(tokenLength + 2, sizeof(char));
	}
	else {
		int holderAlreadyHasWord = strstr(*pointerToTokenStringToAppendTo, token) != NULL;
		if (holderAlreadyHasWord) {
			return;
		}
		
		*pointerToTokenStringToAppendTo = realloc(
			*pointerToTokenStringToAppendTo,
			strlen(*pointerToTokenStringToAppendTo) + tokenLength + 2
		);
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
		
		if (!bytesRead) break;
		
		if (isspace(*(buffer + currentTokenLength))) {
			*(buffer + currentTokenLength) = '\0';
			printf("%s\n", buffer);
			addToken(toReturn, buffer);
			currentTokenLength = -1;
		}
		
		
		
		++currentTokenLength;
		
		
	}
	
	
	
	// for (
	// 	char* currentChar = fileContents;
	// 	*(currentChar);
	// 	++currentChar
	// ) {
		
	// 	int tokenLength = 0;
		
		
	// 	while (*(currentChar + tokenLength) && !isspace(*(currentChar + tokenLength))) {
	// 		++tokenLength;
	// 	}
		
		
	// 	addToken(toReturn, currentChar, tokenLength);
	// 	currentChar += tokenLength;
		
	// }
	
	return toReturn;
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


