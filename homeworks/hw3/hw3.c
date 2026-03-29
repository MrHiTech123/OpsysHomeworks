#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define at(arr, i) (*(arr.values + i))
#define createArray(type, len) {.values = calloc((len), sizeof(type)), .length = (len)}
#define freeArray(array) free(array.values);
// #define copyArrayStart(dest, src, start) memcpy(dest.values, src.values + start, dest.length)
#define copyArray(dest, src) copyArrayStart(dest, src, 0)
#define copyArray(type, src, start, end) ({int len = (end - start); voidArray toReturn = createArray(type, len); memcpy(toReturn.values, src.values + start, len); return toReturn})
#define copyArrayStart(type, src, start) copyArray(type, src, start, src.length)
#define copyArrayEnd(type, src, end) copyArray(type, src, start, 0)
#define defineArrayType(arrayTypeName, typeName) typedef struct {typeName* values; int length;} arrayTypeName
defineArrayType(voidArray, void);
defineArrayType(IntArray, int);
defineArrayType(IntArrayArray, IntArray);

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

void freeIntArrayArray(IntArrayArray toFree) {
	for (int i = 0; i < toFree.length; ++i) {
		freeArray(at(toFree, i));
	}
	freeArray(toFree);
}

IntArrayArray divide_createArray(IntArray arr) {
	IntArrayArray toReturn = createArray(IntArray, 2);
	
	int arrLen = 0;
	at(toReturn, 0) = (IntArray)createArray(int, arr.length / 2 + arr.length % 2); //{.values = calloc((arr.length / 2 + arr.length % 2), sizeof(int)), .length = (arr.length / 2 + arr.length % 2)}
	at(toReturn, 1) = (IntArray)createArray(int, arr.length / 2);
	
	copyArrayStart(at(toReturn, 0), arr, 0);
	copyArrayStart(at(toReturn, 1), arr, at(toReturn, 0).length);
	
	
	// createArray(int, arr.length / 2 + arr.length % 2);
	// at(toReturn, 1) = createArray
	
	
}

IntArray merge_createArray(const IntArray first, const IntArray second) {
	IntArray toReturn = createArray(int, first.length + second.length);
	int firstIndex = 0;
	int secondIndex = 0;
	int toReturnIndex = 0;
	
	
	
	while (firstIndex < first.length && secondIndex < second.length) {
		if (at(first, firstIndex) < at(second, secondIndex)) {
			at(toReturn, toReturnIndex) = at(first, firstIndex);
			++firstIndex;
		}
		else {
			at(toReturn, toReturnIndex) = at(second, secondIndex);
			++secondIndex;
		}
	}
	
	for (; firstIndex < first.length; ++firstIndex) {
		at(toReturn, toReturnIndex) = at(first, firstIndex);
		++toReturnIndex;
	}
	
	for (; secondIndex < second.length; ++secondIndex) {
		at(toReturn, toReturnIndex) = at(second, secondIndex);
		++toReturnIndex;
	}
	
	return toReturn;
	
	
}

void* mergeSort(void* arrayVoid) {
	IntArray array = *(IntArray*)arrayVoid;
	
	if (array.length <= 1) {
		return arrayVoid;
	}
	
	IntArrayArray halves = divide_createArray();
	
	
	
	
	
}

int hw3() {
	IntArray array = {.values = numbers, .length = n};
	
	#ifdef DEBUG
		printIntArray(array);
	#endif
	
	mergeSort(&array);
	
	#ifndef DEBUG
		
		divide_createArray(array);
		
		
		
		printIntArray(array);
	#endif
	
	
	
	return EXIT_SUCCESS;
}













