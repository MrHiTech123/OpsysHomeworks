#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define at(arr, i) (*(arr.values + i))
#define nameOfArrayType(typeName) Array_##typeName

#define createArray(type, len) (nameOfArrayType(type)){.values = calloc((len), sizeof(type)), .length = (len)}
#define freeArray(array) free(array.values);
#define copyArray(type, src, start, end) ({int len = ((end) - (start)); nameOfArrayType(type) toReturn = createArray(type, len); memcpy(toReturn.values, src.values + (start), len * sizeof(type)); toReturn;})
#define copyArrayStart(type, src, start) copyArray(type, src, start, src.length)
#define copyArrayEnd(type, src, end) copyArray(type, src, 0, end)
#define copyArrayFull(type, src) copyArray(type, src, 0, src.length)
#define defineArrayType(typeName) typedef struct {typeName* values; int length;} nameOfArrayType(typeName)

defineArrayType(int);
defineArrayType(Array_int);

extern int* numbers;
extern int n;
extern int numthreads;
extern int comparisons;

void printIntArray(Array_int array) {
	for (int i = 0; i < array.length; ++i) {
		printf("%d, ", at(array, i));
	}
	printf("\n");
}

void freeNestedIntArray(Array_Array_int toFree) {
	for (int i = 0; i < toFree.length; ++i) {
		freeArray(at(toFree, i));
	}
	freeArray(toFree);
}

Array_Array_int divide_createArray(Array_int arr) {
	Array_Array_int toReturn = createArray(Array_int, 2);
	
	int divisionPoint = arr.length / 2 + arr.length % 2;
	
	at(toReturn, 0) = copyArrayEnd(int, arr, divisionPoint);
	at(toReturn, 1) = copyArrayStart(int, arr, divisionPoint);
	
	return toReturn;
}

Array_int merge_createArray(const Array_int first, const Array_int second) {
	Array_int toReturn = createArray(int, first.length + second.length);
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
		++toReturnIndex;
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

#define mergeSort(arr) ({void* ptr = voidVoidMergeSort(&(arr)); Array_int toReturn = *(Array_int*)ptr; free(ptr); toReturn;})


void* voidVoidMergeSort(void* arrayVoid) {
	Array_int array = *(Array_int*)arrayVoid;
	Array_int* toReturn = calloc(1, sizeof(Array_int));
	
	
	if (array.length <= 1) {
		*toReturn = copyArrayFull(int, array);
		return toReturn;
	}
	
	Array_Array_int halves = divide_createArray(array);
	
	Array_Array_int sortedHalves = createArray(Array_int, 2);
	
	
	at(sortedHalves, 0) = mergeSort(at(halves, 0));
	at(sortedHalves, 1) = mergeSort(at(halves, 1));
	
	Array_int sorted = merge_createArray(at(sortedHalves, 0), at(sortedHalves, 1));
	
	freeNestedIntArray(halves);
	freeNestedIntArray(sortedHalves);
	
	*toReturn = sorted;
	return toReturn;
}


int hw3() {
	Array_int originalRealArray = {.values = numbers, .length = n};
	Array_int arrayToSort = copyArrayFull(int, originalRealArray);
	
	#ifdef DEBUG
		printIntArray(arrayToSort);
	#endif
	
	Array_int sorted = mergeSort(arrayToSort);
	
	freeArray(arrayToSort);
	freeArray(originalRealArray);
	
	
	#ifdef DEBUG
		printIntArray(sorted);
	#endif
	
	numbers = sorted.values;
	n = sorted.length;
	
	return EXIT_SUCCESS;
}













