#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>


unsigned int fib(unsigned int n) {
	unsigned int first = 0;
	unsigned int second = 1;
	for (unsigned int i = 0; i < n; ++i) {
		unsigned int oldFirst = first;
		first = second;
		second = oldFirst + first;
	}
	return first;
}



char* readFileToString(char* fileName) {
	return NULL;
}

bool contains(const char* haystack, const char needle) {
	for (
		const char* partOfHaystackBeingChecked = haystack; 
		*(partOfHaystackBeingChecked); 
		++partOfHaystackBeingChecked
	) {
		if (*(partOfHaystackBeingChecked) == needle) {
			return true;
		}
	}
	return false;
}

int lengthOfFileBytes(FILE* file) {
	fseek(file, 0, SEEK_END);
	int toReturn = ftell(file);
	rewind(file);
	return toReturn;
}

char* mallocAndReadFile(const char* fileName) {
	FILE* stream = fopen(fileName, "r");
	char* fileContents = (char*)malloc(lengthOfFileBytes(stream) * sizeof(char));
	fread(fileContents, 1, SIZE_MAX, stream);
	fclose(stream);
	return fileContents;
}



// @return The number of removed chars
int removeAllChars(char* toRemoveFrom, const char* charsToRemove) {
	int toReturn = 0;
	char* beingWritten = toRemoveFrom;
	for (char* beingRead = toRemoveFrom; *(beingRead); ++beingRead) {
		bool shouldBeRemoved = contains(charsToRemove, *(beingRead));
		if (shouldBeRemoved) {
			++toReturn;
		}
		else {
			*beingWritten = *beingRead;
			++beingWritten;
		}
	}
	*beingWritten = '\0';
	return toReturn;
}

int isFib(int numToTest) {
	for (int i = 0; i <= numToTest; ++i) {
		if (fib(i) == numToTest) {
			return true;
		}
	}
	return false;
}


int filter(const char* filename, const char* skip) {
	char* fileContents = mallocAndReadFile(filename);
	int removedChars = removeAllChars(fileContents, skip);
	printf("%s\n", fileContents);
	free(fileContents);
	if (isFib(removedChars)) {
		return 1;
	}
	else {
		return 0;
	}
	
}




