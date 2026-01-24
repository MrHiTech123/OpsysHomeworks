#include <stdlib.h>
#include <stdio.h>



int fileLength(FILE* file) {
	fseek(file, 0, SEEK_END);
	int toReturn = ftell(file);
	rewind(file);
	return toReturn;
}

char* callocFileContentsBuffer(const char* fileName) {
	FILE* file = fopen(fileName, "r");
	int length = fileLength(file);
	printf("%d\n", length);
	
	
	char* toReturn = calloc(length + 1, sizeof(char));
	fread(toReturn, sizeof(char), length, file);
	fclose(file);
	*(toReturn + length) = '\0';
	
	return toReturn;
	
}



int main(int argc, char const *argv[])
{
	if (argc == 1) {
		printf("Usage hw1 tokenfile [maxTokenLength]\n");
		return 1;
	}
	if (argc == 2) {
		char const** nextArgv = calloc(3, sizeof(char*));
		*(nextArgv + 1) = *(argv + 1);
		*(nextArgv + 2) = "100";
		
		int toReturn = main(
			3,
			nextArgv
		);
		
		free(nextArgv);
		return toReturn;
	}
	
	char* fileContents = callocFileContentsBuffer(*(argv + 1));
	
	printf("%s\n", argv[1]);
	printf("%s\n", fileContents);
	
	free(fileContents);
	
	
	
	
	
	
	
	return 0;
}


