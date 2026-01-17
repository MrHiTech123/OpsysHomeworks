#include <stdlib.h>


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













