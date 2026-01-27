#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

typedef char** TokenHolder;


int fileLength(FILE* file) {
	fseek(file, 0, SEEK_END);
	int toReturn = ftell(file);
	rewind(file);
	return toReturn;
}

char* callocFileContentsBuffer(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	int length = fileLength(file);
	printf("%d\n", length);
	
	
	char* toReturn = calloc(length + 1, sizeof(char));
	fread(toReturn, sizeof(char), length, file);
	fclose(file);
	*(toReturn + length) = '\0';
	
	return toReturn;
	
}

char* appendString(char* originalString, char* toAppend) {
	char* toReturn = realloc(
		originalString,
		strlen(originalString) + strlen(toAppend) + 1
	);
	
	strcat(toReturn, toAppend);
	
	return toReturn;
}




void addToken(TokenHolder tokenHolder, char* token, int tokenLength) {
	if (!tokenLength) return;
	
	char** pointerToTokenStringToAppendTo = tokenHolder + tokenLength - 1;
	
	if (*pointerToTokenStringToAppendTo == NULL) {
		*pointerToTokenStringToAppendTo = calloc(tokenLength + 2, sizeof(char));
	}
	else {
		*pointerToTokenStringToAppendTo = realloc(
			*pointerToTokenStringToAppendTo,
			strlen(*pointerToTokenStringToAppendTo) + tokenLength + 2
		);
	}
	
	strncat(*pointerToTokenStringToAppendTo, token, tokenLength);
	strcat(*pointerToTokenStringToAppendTo, "\n");
	
}

TokenHolder callocAndMakeTokenHolder(
	char* fileContents,
	int longestPossibleToken
) {
	TokenHolder toReturn = calloc(
		longestPossibleToken,
		sizeof(char*)
	);
	
	
	for (
		char* currentChar = fileContents;
		*(currentChar);
		++currentChar
	) {
		
		int tokenLength = 0;
		
		
		while (*(currentChar + tokenLength) && !isspace(*(currentChar + tokenLength))) {
			++tokenLength;
			char* tempToken = malloc(tokenLength + 1);
			strncpy(tempToken, currentChar, tokenLength);
			printf("%s\n", tempToken);
			fflush(stdout);
			free(tempToken);
		}
		
		addToken(toReturn, currentChar, tokenLength);
		currentChar += tokenLength;
		
	}
	
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
	const int longestPossibleToken = atoi(*(argv + 2));
	
	char* fileContents = callocFileContentsBuffer(fileName);
	
	TokenHolder tokenHolder = callocAndMakeTokenHolder(
		fileContents,
		longestPossibleToken
	);
	
	for (int i = 0; i < longestPossibleToken; ++i) {
		if (*(tokenHolder + i)) {
			printf("%s\n", *(tokenHolder + i));
		}
	}
	
	
	// printf("%s\n", fileContents);
	
	for (int i = 0; i < longestPossibleToken; ++i) {
		if (*(tokenHolder + i)) {
			free(*(tokenHolder + i));
		}
	}
	free(tokenHolder);
	
	free(fileContents);
	
	
	
	
	
	
	
	
	return 0;
}


