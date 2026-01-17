#include <stdio.h>

#include "reverse.h"

int main() {
	char myWord[] = "hello";
	// strcpy(myWord, "Hello");
	printf("%s\n", myWord);
	reverse(myWord);
	printf("%s\n", myWord);
	reverse(myWord);
	printf("%s\n", myWord);
	
	
	printf("%s\n", alphabetize(myWord));
	
	
	
	
	return 0;
}

