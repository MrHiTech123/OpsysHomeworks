#include <stdlib.h>
#include <stdio.h>


#define at(arr, i) *(arr.values + i)


#define defineArrayType(arrayTypeName, typeName) typedef struct {typeName* values; int length;} arrayTypeName;
defineArrayType(IntArray, int);

extern int* numbers;
extern int n;
extern int numthreads;
extern int comparisons;

void printIntArray(IntArray array) {
	for (int i = 0; i < array.length; ++i) {
		printf("%d, ", at(array, i));
	}
	printf("\n");
}

void mergeSort(IntArray array) {
	
}

int hw3() {
	IntArray array = {.values = numbers, .length = n};
	
	#ifdef DEBUG
		printIntArray(array);
	#endif
	
	mergeSort(array);
	
	#ifdef DEBUG
		printIntArray(array);
	#endif
	
	
	
	return EXIT_SUCCESS;
}













