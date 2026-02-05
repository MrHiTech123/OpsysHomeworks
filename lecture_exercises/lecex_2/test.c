#include <stdlib.h>
#include <signal.h>


int main(int argc, char const *argv[])
{
	int pidOfChild = fork();
	
	if (pidOfChild == 0) {
		abort();
	}
	else {
		int status;
		waitpid(pidOfChild, &status, 0);
		printf("%d %d\n", status, SIGABRT);
	}
	
	
	
	return 0;
}








