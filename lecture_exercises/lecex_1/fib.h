#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


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

bool contains(char* haystack, char needle) {
	for (
		char* partOfHaystackBeingChecked = haystack; 
		*(partOfHaystackBeingChecked); 
		++partOfHaystackBeingChecked
	) {
		if (*(partOfHaystackBeingChecked) == needle) {
			return true;
		}
	}
	return false;
}

char* removeAllChars(char* toRemoveFrom, char* charsToRemove) {
	char* beingWritten = toRemoveFrom;
	for (char* beingRead = toRemoveFrom; *(beingRead); ++beingRead) {
		bool shouldBeRemoved = contains(charsToRemove, *(beingRead));
		if (!shouldBeRemoved) {
			*beingWritten = *beingRead;
			++beingWritten;
		}
	}
	*beingWritten = '\0';
	return toRemoveFrom;
}






