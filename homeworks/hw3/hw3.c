#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

typedef void* VoidPointer;

#define addr(arr, i) (arr.values + i)
#define at(arr, i) (*addr(arr, i))
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
defineArrayType(pthread_t);
defineArrayType(VoidPointer);

extern int* numbers;
extern int n;
extern int numthreads;
extern int numcomparisons;

pthread_mutex_t mutex_numthreads = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_numcomparisons = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_exiting = PTHREAD_MUTEX_INITIALIZER;


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

// int lessThanAndLogComparison(int a, int b) {
// 	++comparisons;
// 	return a < b;
// }

#define lessThanAndLogComparison(a, b) ({pthread_mutex_lock(&mutex_numcomparisons); ++numcomparisons; pthread_mutex_unlock(&mutex_numcomparisons); (a) < (b);})

Array_int merge_createArray(const Array_int first, const Array_int second) {
	Array_int toReturn = createArray(int, first.length + second.length);
	int firstIndex = 0;
	int secondIndex = 0;
	int toReturnIndex = 0;
	
	
	
	while (firstIndex < first.length && secondIndex < second.length) {
		if (lessThanAndLogComparison(at(first, firstIndex), at(second, secondIndex))) {
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

void error() {
	pthread_mutex_lock(&mutex_exiting);
	fprintf(stderr, "ERROR: Failed to do something\n");
	exit(EXIT_FAILURE);
	pthread_mutex_unlock(&mutex_exiting);
}

#define arrayFromVoidVoidMergeSort(ogPtr) ({void* ptr = ogPtr; Array_int toReturn = *(Array_int*)ptr; free(ptr); toReturn;})
#define mergeSort(arr) arrayFromVoidVoidMergeSort(voidVoidMergeSort(&(arr)))

#define handleThreadTracking ({pthread_mutex_lock(&mutex_numthreads); ++numthreads; pthread_mutex_unlock(&mutex_numthreads);})
#define createThread(functionName, arg) ({handleThreadTracking; pthread_t __to_return__; int __worked__ = pthread_create(&__to_return__, NULL, &functionName, &(arg)); if (__worked__ != 0) error(); __to_return__;})

void* voidVoidMergeSort(void* arrayVoid) {
	Array_int array = *(Array_int*)arrayVoid;
	Array_int* toReturn = calloc(1, sizeof(Array_int));
	
	if (array.length <= 1) {
		*toReturn = copyArrayFull(int, array);
		return toReturn;
	}
	
	Array_Array_int halves = divide_createArray(array);
	
	Array_Array_int sortedHalves = createArray(Array_int, 2);
	
	
	
	Array_pthread_t threads = createArray(pthread_t, 2);
	
	at(threads, 0) = createThread(voidVoidMergeSort, at(halves, 0));
	at(threads, 1) = createThread(voidVoidMergeSort, at(halves, 1));
	
	Array_VoidPointer pointersToMergedArrays = createArray(VoidPointer, 2);
	pthread_join(at(threads, 0), addr(pointersToMergedArrays, 0));
	pthread_join(at(threads, 1), addr(pointersToMergedArrays, 1));
	
	at(sortedHalves, 0) = arrayFromVoidVoidMergeSort(at(pointersToMergedArrays, 0));
	at(sortedHalves, 1) = arrayFromVoidVoidMergeSort(at(pointersToMergedArrays, 1));
	
	
	Array_int sorted = merge_createArray(at(sortedHalves, 0), at(sortedHalves, 1));
	
	freeArray(threads);
	freeArray(pointersToMergedArrays);
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













