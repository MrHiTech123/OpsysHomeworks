#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define at(arr, i) *(arr + i)


void* copy_file(void* arg);

const char* pluralize(int num, const char* pluralization) {
	return num == 1? "" : pluralization;
}

const char* pluralizeS(int num) {
	return pluralize(num, "s");
}

int main(int argc, const char** argv)
{
	pthread_t* threads = calloc(argc - 1, sizeof(pthread_t));
	
	int howManyFiles = argc - 1;
	const char** fileNames = argv + 1;
	
	
	for (int i = 0; i < howManyFiles; ++i) {
		const char* currentFile = at(fileNames, i);
		pthread_create(&at(threads, i), NULL, copy_file, (char*)at(fileNames, i));
		printf("MAIN: Creating thread to copy \"%s\"\n", currentFile);
	}
	
	void** fileLength = calloc(1, sizeof(void*));
	int totalLengths = 0;
	
	for (int i = 0; i < howManyFiles; ++i) {
		pthread_join(at(threads, i), fileLength);
		int bytesCopied = *(int*)*fileLength;
		totalLengths += bytesCopied;
		free(*fileLength);
		printf("MAIN: Thread completed copying %d byte%s for \"%s\"\n", bytesCopied, pluralizeS(bytesCopied), at(fileNames, i));
	}
	
	free(fileLength);
	free(threads);
	
	printf("MAIN: Successfully copied %d byte%s via %d child thread%s\n", totalLengths, pluralizeS(totalLengths), howManyFiles, pluralizeS(howManyFiles));
	
	
	
	
	return 0;
}






