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
	
	return 0;
}

