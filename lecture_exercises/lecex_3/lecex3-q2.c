#include <ctype.h>
#include <stdlib.h>
int uppercaseCharsAmount(const char* s) {
	int toReturn = 0;
	
	for (const char* currentChar = s; *currentChar; ++currentChar) {
		if (isupper(*currentChar)) {
			++toReturn;
		}
	}
	return toReturn;
}




void * lecex3_q2_child( void * arg ) {
	char* s = arg;
	
	for (char* currentChar = s; *currentChar; ++currentChar) {
		if (islower(*currentChar)) {
			*currentChar = toupper(*currentChar);
		}
		else if (isdigit(*currentChar)) {
			*currentChar = '~';
		}
	}
	
	
	
	int* toReturn = calloc(1, sizeof(int));
	*toReturn = uppercaseCharsAmount(s);
	return toReturn;
}

































