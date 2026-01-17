#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* reverse(char* s) {
	int length = strlen(s);
	char* buffer = (char*)malloc((length) * sizeof(char));
	for (int i = 0; i < length; ++i) {*(buffer + i) = *(s + (length - i - 1));}
	for (int i = 0; i < length; ++i) {*(s + i) = *(buffer + i);}
	free(buffer);
	return s;
}

char* alphabetize(char* s) {
	int len = strlen(s);
	for (int indexOfToBeInserted = 1; indexOfToBeInserted < len; ++indexOfToBeInserted) {
		for (int indexOfInsertion = indexOfToBeInserted; indexOfInsertion > 0; --indexOfInsertion) {
			if (*(s + indexOfInsertion) < *(s + indexOfInsertion - 1)) {
				// Swap the two indices
				*(s + indexOfInsertion) += *(s + indexOfInsertion - 1);
				*(s + indexOfInsertion - 1) = *(s + indexOfInsertion) - *(s + indexOfInsertion - 1);
				*(s + indexOfInsertion) -= *(s + indexOfInsertion - 1);
			}
			else {
				break;
			}
		}
	}
	return s;
}