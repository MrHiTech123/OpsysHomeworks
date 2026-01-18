#include <stdio.h>

#include "reverse.h"
#include "fib.h"

int main() {
	char myWord[] = "hello";
	// strcpy(myWord, "Hello");
	printf("%s\n", myWord);
	reverse(myWord);
	printf("%s\n", myWord);
	reverse(myWord);
	printf("%s\n", myWord);
	
	
	printf("%s\n", alphabetize(myWord));
	
	int current = 0;
	for (int i = 0; current >= 0; ++i) {
		current = fib(i);
		printf("%d %d\n", i, current);
	}
	
	// char myAlsoWord[] = "My password is: 1233";
	// removeAllChars(myAlsoWord, "pass3");
	// printf("%s\n", myAlsoWord);
	
	// FILE* stream = fopen("input.txt", "r");
	// int length = lengthOfFileBytes(stream);
	// int length2 = lengthOfFileBytes(stream);
	// printf("Length: %d -> %d\n", length, length2);
	
	const char* fileName = "input.txt";
	const char* skip = "Hwl";
	int fib = filter(fileName, skip);
	printf("%d\n", fib);
	
	
	return 0;
}

