#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/wait.h>


#define arraySet(arr, index, value) *(arr + index) = value
#define arrayGet(arr, index) *(arr + index)
#define matrixSet(mat, row, col, value) *(*(mat + row) + col) = value
#define matrixGet(mat, row, col) *(*(mat + row) + col)

#define MAKE_HAS_NON_ZEROES_FUNCTION(name, class) bool name(class* arr, int length) {for (int i = 0; i < length; ++i) {if (arrayGet(arr, i)) {return true;}} return false;}

MAKE_HAS_NON_ZEROES_FUNCTION(hasNonZeroes__int, int);

int main(int argc, char const *argv[])
{
	int* arr = calloc(3, sizeof(int));
	
	printf("%d\n", hasNonZeroes__int(arr, 3));
	
	arr[0] = 1;
	
	printf("%d\n", hasNonZeroes__int(arr, 3));
	
	int status;
	pid_t p = fork();
	if (p == 0) {
		sleep(5);
		exit(0);
	}
	else {
		sleep(1);
		int nothing;
		waitpid(p, &nothing, 0);
	}
	
	waitpid(p, &status, 0);
	printf("%d %d\n", WEXITSTATUS(status), WIFEXITED(status));
	
	
	free(arr);
	
	return 0;
}
