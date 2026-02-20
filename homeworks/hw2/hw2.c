#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


// Taken from https://stackoverflow.com/questions/22705751/cannot-open-include-file-unistd-h-no-such-file-or-directory
// Replaces the line #include <unistd.h>
#ifdef _WIN32
#include <io.h>
#define access _access

#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/resource.h>
#endif

void rlimitSettings() {
	struct rlimit r1;
	
	
}

int main(int argc, char const *argv[])
{
	rlimitSettings();
	return 0;
}



