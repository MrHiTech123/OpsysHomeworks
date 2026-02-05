#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <limits.h>

// Taken from https://stackoverflow.com/questions/22705751/cannot-open-include-file-unistd-h-no-such-file-or-directory
// Replaces the line #include <unistd.h>
#ifdef _WIN32
#include <io.h>
typedef size_t pid_t;
#define O_RDONLY _A_RDONLY
#define access _access

#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#endif



/* implement these functions separately in lecex2-q1.c */
int lecex2_child( const char * filename ) {
	int fileHandle = open(filename, O_RDONLY);
	if (fileHandle < 0) {
		fprintf(stderr, "CHILD: Error: File \"input.txt\" not found\n");
		abort();
	}
	int currentIndex = 0;
	int indexOfLastUpper = -1;
	char lastUpper = 0;
	char whatWasRead;
	while (read(fileHandle, &whatWasRead, 1)) {
		if (isupper(whatWasRead)) {
			lastUpper = whatWasRead;
			indexOfLastUpper = currentIndex;
		}
		++currentIndex;
	}
	close(fileHandle);
	
	if (!lastUpper) {
		fprintf(stderr, "ERROR: No uppercase letters found\n");
		abort();
	}
	
	printf("CHILD: found character '%c' at byte position %d\n", lastUpper, indexOfLastUpper);
	
	return (int)lastUpper;
	
}
int lecex2_parent() {
	int status = 0;
	(void)waitpid(-1, &status, 0);
	
	// Turn the int back into a char (???),
	// done in such a way to ensure that if it's already little endian it won't be truncated further. 
	while (status > CHAR_MAX) { 
		status >>= 8;
	}
	
	if (status == SIGABRT) {
		fprintf(stderr, "PARENT: child process terminated abnormally\n");
		return EXIT_FAILURE;
	}
	else {
		printf("PARENT: child process exited with status '%c'\n", status);
		return EXIT_SUCCESS;
	}
}
